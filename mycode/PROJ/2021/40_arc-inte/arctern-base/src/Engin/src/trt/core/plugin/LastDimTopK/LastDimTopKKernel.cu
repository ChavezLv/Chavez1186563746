#include "LastDimTopKKernel.h"

#include <thrust/sort.h>
#include <thrust/system/cuda/execution_policy.h>

constexpr int kWarpSize = 32;

/// The maximum in-block bitonic sort we support
constexpr int kMaxBitonicSortSize = 4096;

__device__ __forceinline__ int getLaneId()
{
    int laneId;
    asm("mov.s32 %0, %laneid;" : "=r"(laneId));
    return laneId;
}

__device__ __forceinline__ unsigned getLaneMaskLt()
{
    unsigned mask;
    asm("mov.u32 %0, %%lanemask_lt;" : "=r"(mask));
    return mask;
}

// Returns log2(n) for a positive integer type
template <typename T>
__device__ constexpr int IntegerLog2(T n, int p = 0)
{
    return (n <= 1) ? p : IntegerLog2(n / 2, p + 1);
}

// Returns true if the given integer type is a power-of-2 (positive only)
template <typename T>
__device__ constexpr bool integerIsPowerOf2(T v)
{
    return (v && !(v & (v - 1)));
}

template <typename T>
__device__ inline void swapVars(T& t1, T& t2)
{
    T tmp = t1;
    t1 = t2;
    t2 = tmp;
}

template <typename K, typename V>
struct LTComp
{
    __device__ inline bool operator()(const K& kA, const V& vA, const K& kB, const V& vB) const
    {
        // FIXME: adding value comparison is slow
        return (kA < kB) || ((kA == kB) && (vA < vB));
    }
};

template <typename K, typename V>
struct GTComp
{
    __device__ inline bool operator()(const K& kA, const V& vA, const K& kB, const V& vB) const
    {
        // FIXME: adding value comparison is slow
        // FIXME: it's vA < vB because the sorting order for V (aka
        // indices) is different in our use case
        return (kA > kB) || ((kA == kB) && (vA < vB));
    }
};

constexpr size_t getHeapSmemSize(size_t keySize, size_t valueSize, int numThreads, int heapSize)
{
    return (numThreads / kWarpSize) * heapSize * (keySize + valueSize);
}

template <typename Comparator, typename K, typename V>
__device__ inline void bitonicSwap(K& kA, V& vA, K& kB, V& vB, bool dir, const Comparator& comp)
{
    bool swap = comp(kA, vA, kB, vB);
    if (swap == dir)
    {
        swapVars(kA, kB);
        swapVars(vA, vB);
    }
};

template <typename Comparator, typename K, typename V, int Power2SortSize, int ThreadsPerBlock>
__device__ inline void bitonicSort(K* keys, V* values, const Comparator& comp)
{
    static_assert(Power2SortSize <= kMaxBitonicSortSize, "sort size <= 4096 only supported");
    // Assume the sort is taking place in shared memory
    // static_assert(Power2SortSize * (sizeof(K) + sizeof(V)) < 32768,
    //               "sort data too large (>32768 bytes)");
    static_assert(integerIsPowerOf2(Power2SortSize), "sort size must be power of 2");
    static_assert(integerIsPowerOf2(ThreadsPerBlock), "threads in block must be power of 2");

    // If what we are sorting is too small, then not all threads
    // participate
    constexpr int numThreadsForSort = Power2SortSize / 2;
    constexpr bool allThreads = numThreadsForSort >= ThreadsPerBlock;

    // If what we are sorting is too large, then threads must loop more
    // than once
    constexpr int loopPerThread = allThreads ? numThreadsForSort / ThreadsPerBlock : 1;

#pragma unroll
    for (int size = 2; size < Power2SortSize; size *= 2)
    {

#pragma unroll
        for (int stride = size / 2; stride > 0; stride /= 2)
        {

#pragma unroll
            for (int loop = 0; loop < loopPerThread; ++loop)
            {
                int threadId = loop * ThreadsPerBlock + threadIdx.x;
                bool flag = ((threadId & (size / 2)) != 0);

                int pos = 2 * threadId - (threadId & (stride - 1));

                if (allThreads || (threadId < numThreadsForSort))
                {
                    bitonicSwap<Comparator, K, V>(
                        keys[pos], values[pos], keys[pos + stride], values[pos + stride], flag, comp);
                }

                __syncthreads();
            }
        }
    }

#pragma unroll
    for (int stride = Power2SortSize / 2; stride > 0; stride /= 2)
    {

#pragma unroll
        for (int loop = 0; loop < loopPerThread; ++loop)
        {
            int threadId = loop * ThreadsPerBlock + threadIdx.x;

            int pos = 2 * threadId - (threadId & (stride - 1));

            if (allThreads || (threadId < numThreadsForSort))
            {
                bitonicSwap<Comparator, K, V>(
                    keys[pos], values[pos], keys[pos + stride], values[pos + stride], false, comp);
            }

            __syncthreads();
        }
    }
}

// Per-warp heap structure in shared memory:
// [key_0, ..., key_(HeapSize-2)], [empty element] (warp 0)
// ...
// [key_0, ..., key_(HeapSize-2)], [empty element] (warp n-1)
// [value_0, ..., value_(HeapSize-2)], [empty element] (warp 0)
// ...
// [value_0, ..., value_(HeapSize-2)], [empty element] (warp n-1)

// Dir == true means we are selecting the largest values, thus
// the heap is a min-heap
template <typename K, typename V, int HeapSize, bool Dir>
__device__ inline void warpHeapInsert(K k, V v, K* keyHeap, V* valueHeap)
{
    // Replace head if we are < head
    bool valid = Dir ? (k > keyHeap[0]) : (k < keyHeap[0]);

    // Even though this is the single-thread case, another preceding
    // thread in the warp may have inserted in a new element that
    // supersedes our element and thus our attempt at an insert would do
    // nothing.
    if (!valid)
    {
        return;
    }

    // Swap with head if valid
    K currentKey = k;
    V currentValue = v;

    keyHeap[0] = currentKey;
    valueHeap[0] = currentValue;

    // The number of interior nodes in the heap is log2(HeapSize / 2):
    // heap size 8 means there are 7 elements in the heap, indices 0-6
    // (0 12 3456)
    // log2(8 / 2) = 2 levels of interior nodes for heap size 8 (0 and 12)
    int i = 0;
#ifndef __HIP_PLATFORM_HCC__
#pragma unroll
#endif
    for (int levels = 0; levels < IntegerLog2(HeapSize / 2); ++levels)
    {
        int leftChild = i * 2 + 1;
        int rightChild = leftChild + 1;
        K leftKey = keyHeap[leftChild];
        K rightKey = keyHeap[rightChild];

        // What child might we want to swap with (max heap = larger child;
        // min heap = smaller child)
        bool swap = Dir ? (leftKey < rightKey) : (leftKey > rightKey);
        int childToSwap = swap ? leftChild : rightChild;
        K keyChildToSwap = swap ? leftKey : rightKey;

        // If we're bigger than both children (max heap), or smaller than
        // both children (min heap), then we do nothing for the rest of
        // the iterations
        valid = Dir ? !(currentKey < keyChildToSwap) : !(currentKey > keyChildToSwap);

        // Swap with childToSwap if still valid
        keyHeap[i] = valid ? keyChildToSwap : currentKey;
        valueHeap[i] = valid ? valueHeap[childToSwap] : currentValue;

        keyHeap[childToSwap] = valid ? currentKey : keyChildToSwap;
        valueHeap[childToSwap] = valid ? currentValue : valueHeap[childToSwap];

        i = childToSwap;

        // This is our new element to potentially downheap
        currentKey = keyHeap[i];
        currentValue = valueHeap[i];
    }
}

template <typename K, typename V, int HeapSize, bool Dir>
__device__ inline void warpHeap(K k, V v, K& keyHeapHead, K* keyHeap, V* valueHeap)
{
    // All threads in the warp have elements
    bool wantInsert = Dir ? (k > keyHeapHead) : (k < keyHeapHead);

    // Find out all the lanes that have elements to add to the heap
    unsigned int vote = __ballot_sync(__activemask(), wantInsert);

    if (!vote)
    {
        // Everything the warp has is smaller than our heap
        return;
    }

    // Otherwise, we want to serialize execution of the threads
    // that have elements
    int index = __popc(getLaneMaskLt() & vote);
    int total = __popc(vote);

    // FIXME: try switch statement and explicitly handle cases
    // FIXME: how do cases work?
    for (int i = 0; i < total; ++i)
    {
        if (index == i && wantInsert)
        {
            // Insert into our heap
            warpHeapInsert<K, V, HeapSize, Dir>(k, v, keyHeap, valueHeap);

            // Make sure all smem writes are visible
            __threadfence_block();
        }
    }

    // Re-broadcast the new heap head
    // FIXME: consider each updater above will broadcast its value with
    // a shuffle instead?
    keyHeapHead = keyHeap[0];
}

template <typename K, typename V, int ThreadsPerBlock, int HeapSize, bool Dir>
class Heap
{
public:
    static constexpr size_t getSmemSize()
    {
        return getHeapSmemSize(sizeof(K), sizeof(V), ThreadsPerBlock, HeapSize);
    }

    __device__ Heap(void* smem, K initKey, V initVal)
    {
        heapBase = smem;

        int warpId = threadIdx.x / kWarpSize;
        int laneId = getLaneId();

        auto kStart = getKeyStart();
        heapK = &kStart[warpId * HeapSize];
        auto vStart = getValueStart();
        heapV = &vStart[warpId * HeapSize];

        heapHead = initKey;

        if (HeapSize < kWarpSize)
        {
            if (laneId < HeapSize)
            {
                heapK[laneId] = initKey;
                heapV[laneId] = initVal;
            }
        }
        else
        {
#pragma unroll
            for (int i = 0; i < HeapSize / kWarpSize; ++i)
            {
                heapK[laneId + i * kWarpSize] = initKey;
                heapV[laneId + i * kWarpSize] = initVal;
            }
        }
    }

    // Returns a pointer to the start of our block-wide key storage
    inline __device__ K* getKeyStart()
    {
        return (K*) heapBase;
    }

    // Returns a pointer to the start of our block-wide value storage
    inline __device__ V* getValueStart()
    {
        constexpr int warpsPerBlock = ThreadsPerBlock / kWarpSize;
        return (V*) &getKeyStart()[warpsPerBlock * HeapSize];
    }

    // Returns a pointer past the end of our block-wide heap storage
    inline __device__ void* getStorageEnd()
    {
        constexpr int warpsPerBlock = ThreadsPerBlock / kWarpSize;
        return getValueStart() + (warpsPerBlock * HeapSize);
    }

    inline __device__ void add(K k, V v)
    {
        warpHeap<K, V, HeapSize, Dir>(k, v, heapHead, heapK, heapV);
    }

    // Reduce all per-warp heaps to a unified, sorted list
    inline __device__ void reduceHeaps()
    {
        constexpr int allHeapSize = (ThreadsPerBlock / kWarpSize) * HeapSize;

        if (Dir)
        {
            bitonicSort<GTComp<K, V>, K, V, allHeapSize, ThreadsPerBlock>(
                getKeyStart(), getValueStart(), GTComp<K, V>());
        }
        else
        {
            bitonicSort<LTComp<K, V>, K, V, allHeapSize, ThreadsPerBlock>(
                getKeyStart(), getValueStart(), LTComp<K, V>());
        }
    }

private:
    void* heapBase;
    K heapHead;
    K* heapK;
    V* heapV;
};

template <typename V, typename IndexType, typename OutIndexType, int ThreadsPerBlock, int HeapSize,
    bool Dir>
__global__ void selectRowsViaHeap(const V* input, // m x n
    V* outKeys,                                   // m x k
    OutIndexType* outIndices,                     // m x k
    V initVal, IndexType initIndex, int m, int n, int k)
{
    extern __shared__ float smem[];

    Heap<V, IndexType, ThreadsPerBlock, HeapSize, Dir> heap(smem, initVal, initIndex);

    auto inputStart = &input[blockIdx.x * n];

    // FIXME choose desired unroll level
    constexpr int Unroll = 1;
    V vals[Unroll];

    for (int i = threadIdx.x; i < n; i += blockDim.x * Unroll)
    {
#ifndef __HIP_PLATFORM_HCC__
#pragma unroll
#endif
        for (int j = 0; j < Unroll; ++j)
        {
            vals[j] = inputStart[i + j * blockDim.x];
        }

#ifndef __HIP_PLATFORM_HCC__
#pragma unroll
#endif
        for (int j = 0; j < Unroll; ++j)
        {
            heap.add(vals[j], (IndexType) i + j * blockDim.x);
        }
    }

    // When finished, we restructure the heaps in shared memory
    // The heaps are actually of size HeapSize - 1 (e.g., 32 -> 31); the
    // extra element should have remained untouched, so we can still
    // sort things in-place as a power of 2.
    __syncthreads();

    heap.reduceHeaps();

    auto outKeysStart = &outKeys[blockIdx.x * k];
    auto outIndicesStart = &outIndices[blockIdx.x * k];

    auto heapK = heap.getKeyStart();
    auto heapV = heap.getValueStart();

    // Write out the final k-selected values; they should be all
    // together
    for (int i = threadIdx.x; i < n && i < k; i += blockDim.x)
    {
        outKeysStart[i] = heapK[i];
        outIndicesStart[i] = (OutIndexType) heapV[i];
    }
}

template <typename T, int kHeapSize, bool kSelectMax = true>
void RunHeapSelectionImpl(
    const T* input, const int outer_size, const int inner_size, const int k, T* values, T* indices, cudaStream_t stream)
{
    constexpr int kBlockSize = 256;
    constexpr int kNumWarps = kBlockSize / kWarpSize;
    constexpr int smem = kNumWarps * kHeapSize * (sizeof(T) + sizeof(T));
    constexpr T kInitVal = kSelectMax ? std::numeric_limits<T>::lowest() : std::numeric_limits<T>::max();
    selectRowsViaHeap<T, T, T, kBlockSize, kHeapSize, kSelectMax><<<outer_size, kBlockSize, smem, stream>>>(
        input, values, indices, kInitVal, std::numeric_limits<int64_t>::max(), outer_size, inner_size, k);
}

// template <typename T, bool kSelectMax = true>
// void RunRadixSelectionImpl(
//    const T* input,
//    const int64_t outer_size,
//    const int64_t inner_size,
//    const int k,
//    T* values,
//    int64_t* indices,
//    CUDAContext* context) {
//  const int block = std::min(
//      math::RoundUp(static_cast<int>(inner_size), kWarpSize),
//      CAFFE_CUDA_NUM_THREADS);
//  gatherTopK<T, kSelectMax, int64_t>
//  <<<outer_size, block, 0, context->cuda_stream()>>>(
//      input, inner_size, k, outer_size, values, indices);
//  // Unfortunately the output is not currently sorted, and there is no batch
//  // sorting utility available. Iterate over all of the slices and sort them
//  // in-place using Thrust.
//  for (int i = 0; i < outer_size; ++i) {
//    thrust::sort_by_key(
//        thrust::cuda::par.on(context->cuda_stream()),
//        values + i * k,
//        values + i * k + (k <= inner_size ? k : inner_size),
//        indices + i * k,
//        thrust::greater<T>());
//  }
//}

pluginStatus_t LastDimTopKInference(cudaStream_t stream, const int num_threads, void* out_value, void* out_indices,
    const void* in_data, const int K, const int N, const bool is_ascend)
{
    if (is_ascend)
    {
        // If k is small, uses heap selection, otherwise uses radix selection.
        if (K < 32)
        {
            RunHeapSelectionImpl<float, 32, false>(
                (float*) in_data, num_threads, N, K, (float*) out_value, (float*) out_indices, stream);
        }
        else if (K < 128)
        {
            RunHeapSelectionImpl<float, 128, false>(
                (float*) in_data, num_threads, N, K, (float*) out_value, (float*) out_indices, stream);
        }
        else if (K < 512)
        {
            RunHeapSelectionImpl<float, 512, false>(
                (float*) in_data, num_threads, N, K, (float*) out_value, (float*) out_indices, stream);
        }
        else
        {
            //      RunRadixSelectionImpl<float, true>((float *)in_data, num_threads, N,
            //                                              K, (float *)out_value,
            //                                              (float *)out_indices, stream);
        }
    }
    else
    {
        // If k is small, uses heap selection, otherwise uses radix selection.
        if (K < 32)
        {
            RunHeapSelectionImpl<float, 32, true>(
                (float*) in_data, num_threads, N, K, (float*) out_value, (float*) out_indices, stream);
        }
        else if (K < 128)
        {
            RunHeapSelectionImpl<float, 128, true>(
                (float*) in_data, num_threads, N, K, (float*) out_value, (float*) out_indices, stream);
        }
        else if (K < 512)
        {
            RunHeapSelectionImpl<float, 512, true>(
                (float*) in_data, num_threads, N, K, (float*) out_value, (float*) out_indices, stream);
        }
        else
        {
            //      RunRadixSelectionImpl<float, false>((float *)in_data, num_threads, N,
            //                                              K, (float *)out_value,
            //                                              (float *)out_indices, stream);
        }
    }
    return STATUS_SUCCESS;
}