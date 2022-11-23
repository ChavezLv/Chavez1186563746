
//course1 
// 笔记by lcl:
/*
为什么空间申请和对象构建分开, 对象和销毁, 空间的释放? 

new一个point(1,2)并没有这么严格区分.  
因为空间申请的对象有很多, 比如vector, 先申请一大段空间, 然后再在上面
构建对象, 避免频繁的申请. 

比如面试: vector里面 push_back()一万个元素, 会有什么效果? 
答: 从1到1, 2到2, 3到4, 会频繁地扩容, 扩容的allocator, 底层导致
堆空间频繁地申请和释放, 降低堆空间的利用率. 所以可以用reserve预留点
空间. 

1) 如果定义了宏会走一级空间配置器, 走malloc. -----默认? 二级-----

# ifdef __USE_MALLOC  //一级空间配置器
# else  //二级空间配置器, 默认情况下会走二级!

2) 如果申请
static void* allocate(size_t __n)
	{
		if(n > 128)
		{
			malloc(n)
		}
		else
		{
			//内存池 + 16个自由链表
		}
	}
a)----为什么这么划分?  xmind: allocator---------------
所有都用malloc, 很小的内存都由malloc申请,释放, 底层频繁从内核态切换
到用户态, 降低速度. 

内存池, 类比线程池, 进程池, 是个池, 小空间就去内存池切割, 
b) ------原理图------------
64位下面的一个指针为8, 128/16 = 8

比如用户要申请32个字节, 底层会申请n个32个字节(比如40个32个字节), 再切割, 
挂到32对应的位置下标那里.  这就是池子, 很方便下次使用

这样就是老掉牙的经典: 空间换时间

//---------------------lcl: 3)--------------------------------
3)继续看源码:
java的sun太阳公司, HP惠普
# if defined(__SUNPRO_CC) || defined(__GNUC__) || defined(__HP_aCC)
    _NFREELISTS
# else
    __default_alloc_template<__threads, __inst>::_NFREELISTS
# endif

*/




template <class _Tp>
class allocator 
{
	typedef alloc _Alloc; 
public:
	_Tp* allocate(size_type __n, const void* = 0) 
	{
		return __n != 0 ? static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp))) : 0;
    }

   void deallocate(pointer __p, size_type __n)
   { 
		_Alloc::deallocate(__p, __n * sizeof(_Tp));
   }

	void construct(pointer __p, const _Tp& __val) 
	{ 
		new(__p) _Tp(__val);
	}
    
	void destroy(pointer __p) 
	{ 
		__p->~_Tp(); 
	}
};





typedef alloc _Alloc;
//第一分支   一级空间配置器（malloc）
# ifdef __USE_MALLOC  
typedef malloc_alloc alloc;
typedef __malloc_alloc_template<0> malloc_alloc;
template <int __inst>
class __malloc_alloc_template 
{
	public:
	static void* allocate(size_t __n)
   {
	   void* __result = malloc(__n);
       if (0 == __result) __result = _S_oom_malloc(__n);
       return __result;
   }
}；

# ifdef __USE_MALLOC  //一级空间配置器
# else  //二级空间配置器

//第二分支  二级空间配置器
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;

template <bool threads, int inst>
class __default_alloc_template 
{
public:
	static void* allocate(size_t __n)
	{
		void* __ret = 0;
		
		if (__n > (size_t) _MAX_BYTES) //如果空间大于128
		{
			//还是使用malloc进行空间的申请
		}
		
		else 
		{
			//n是小于等于128,
			//
			//1) _Obj联合体     __STL_VOLATILE异变的意思
			//2) volative   mutable  可以改变
			//3) __my_free_list自由链表
			//4) else{}里面:  内存池 + 16个自由链表
//---------------------lcl: 3)-----Step3----------------------
// from ----------lcl: 3) Step2
// _S_freelist_index(__n)  传入32, 得到3, 对应自由链表的下标3  
// 再解引用*__my_free_list 赋值给 __result, 如果等于0, 就申请空间


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$------stl相关图解pptx-----$$$$$$$$$$$$$$$$$$$$$$$$$$$
//----------------goto-----lcl: 3)-----Step4----------------------
//解读方法: ppt里面有个example
			//先申请32字节(malloc  1280, 用640, 存640)
			//再申请64字节(不malloc, 上次存了640, 取出来, 取640)
			//接着申请96字节(内存池为空了, 去malloc吧)
			//再申请72字节, 假设此时堆空间与内存都是空的(不足量了)

			_Obj* __STL_VOLATILE* __my_free_list
			= _S_free_list + _S_freelist_index(__n);
			
			_Obj* __RESTRICT __result = *__my_free_list;
			if (__result == 0)
				__ret = _S_refill(_S_round_up(__n));//--在-lcl: 3)-----Step6--
// ---------from  ---lcl: 3)-----Step6---
// _S_refill将申请的内存做了切分, 之后再进入的时候, 这个__result != 0
// 就不用再malloc了
			//_Obj* _M_free_list_link;// 类似于写链表的时候的next指针

			else 
			{
				*__my_free_list = __result -> _M_free_list_link;
				__ret = __result;
			}
		}  

};

//数据结构
	union _Obj 
	{
		union _Obj* _M_free_list_link;// 类似于写链表的时候的next指针
        char _M_client_data[1];    /* The client sees this. */
	};
	
	enum {_ALIGN = 8};
    enum {_MAX_BYTES = 128};
    enum {_NFREELISTS = 16}; // _MAX_BYTES/_ALIGN

//---------------------lcl: 3) Step1---------------------------	
//java的sun太阳公司, HP惠普
template <bool __threads, int __inst>
typename __default_alloc_template<__threads, __inst>::_Obj* __STL_VOLATILE
__default_alloc_template<__threads, __inst> ::_S_free_list[
# if defined(__SUNPRO_CC) || defined(__GNUC__) || defined(__HP_aCC)
    _NFREELISTS
# else
    __default_alloc_template<__threads, __inst>::_NFREELISTS
# endif
] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };//16个默认为0



//---------------------lcl: 3) Step2---------------------------
//找到要申请字节对应的自由链表的下标
// __bytes = 32 作为传入, 传出结果3
static  size_t _S_freelist_index(size_t __bytes)//32
{
	return (((__bytes) + (size_t)_ALIGN-1)/(size_t)_ALIGN - 1);	
}
// return (32 + 7)/8 - 1 = 4 - 1 = 3	
// lcl: 就是前面free_list的下标
//-----------------goto----lcl: 3)-----Step3----------------------	



//--------------------lcl: 3)-----Step5---------------------
//向上取整，得到8的倍数, 对比上面的
static size_t _S_round_up(size_t __bytes)//32 [25,32] == 32
{ 
	return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); 
	//   (32 + 7) & ~ (8 - 1) = (32 + 7) & ~ 7   = 39 & ~ 7
}	//	39 = 32 + 4 + 2 + 1 = 0010 0111
	//	      7 = 4 + 2 + 1 = 0000 0111
    (32 + 7) & ~ 7 = 32
   0010 0111
&  1111 1000
=
   0010 0000
   	(31 + 7) & ~ 7 = 32
   0010 0110
&  1111 1000
=
   0010 0000

   	(27 + 7) & ~ 7 = 32
   0010 0001
&  1111 1000
=
   0010 0000
   	(25 + 7) & ~ 7 = 32
   0010 0000
&  1111 1000
=
   0010 0000
   所以对照自由链表可知:  3对应的下标就是给申请25-32字节用的
   这就是_S_round_up的意义: 向上取整，得到8的倍数
//-----------------goto----lcl: 3)-----Step6-----看_S_refill-------


char* _S_start_free = 0;
char* _S_end_free = 0;
size_t _S_heap_size = 0;

template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_start_free = 0;

template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_end_free = 0;

template <bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_heap_size = 0;

//--------------------lcl: 3)-----Step7---------------------
char* _S_chunk_alloc(size_t __size, int& __nobjs)//传入 32  20
{
	char* __result;
    size_t __total_bytes = __size * __nobjs;// = 32 * 20 = 640;
    size_t __bytes_left = _S_end_free - _S_start_free;// = 0;
	
	if (__bytes_left >= __total_bytes) { // 0 >= 640 ? false
        __result = _S_start_free;
        _S_start_free += __total_bytes;
        return(__result);
    }else if(__bytes_left >= __size){// 0 >= 32 ? false 走不到
    	//...
    }
	else// 走这里
	{
		size_t __bytes_to_get = 2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
		                          // = 2 * 640 + 0 = 1280;
		//申请了1280						  
		_S_start_free = (char*)malloc(__bytes_to_get);// = (char *)malloc(1280);

		_S_heap_size += __bytes_to_get;// = 1280;
        _S_end_free = _S_start_free + __bytes_to_get;//头尾指针指定空间, 这就是内存池
        return(_S_chunk_alloc(__size, __nobjs));//执行递归调用, 还是传入32, 20没有变
		
				//递归调用
				
				char* __result;
		        size_t __total_bytes = __size * __nobjs = 32 * 20 = 640;
		        size_t __bytes_left = _S_end_free - _S_start_free = 1280;//已经变化了
				
				if (__bytes_left >= __total_bytes) // 1280 >= 640? true
				{
					__result = _S_start_free;//__result指向首地址
					_S_start_free += __total_bytes;//这时候, 1280一分为二
					return(__result);// 其中的640交给32, 剩下给到内存池子
					// --return to ---lcl: 3)-----Step6-
				}
	}
	
}

//--------------------lcl: 3)-----Step6---------------------

//将申请的内存做了切分
void* _S_refill(size_t __n)//32
{
    int __nobjs = 20;
    //--------------goto------lcl: 3)-----Step7---------------------
    char* __chunk = _S_chunk_alloc(__n, __nobjs);//32  20
    //_S_chunk_alloc申请了1280, 其中的640交给32, 交给了__chunk, 剩下给到内存池子
    _Obj* __STL_VOLATILE* __my_free_list;
    _Obj* __result;
    _Obj* __current_obj;
    _Obj* __next_obj;
    int __i;
	
	__my_free_list = _S_free_list + _S_freelist_index(__n);//_S_free_list[3]
	
	 __result = (_Obj*)__chunk;// 为什么强转给了
      *__my_free_list = __next_obj = (_Obj*)(__chunk + __n);//偏移了32个字节
      for (__i = 1; ; __i++) 
	  {
        __current_obj = __next_obj;
        __next_obj = (_Obj*)((char*)__next_obj + __n);//再偏移
        if (__nobjs - 1 == __i) {//目前20 - 1 = 19 --> 19 != __i
            __current_obj -> _M_free_list_link = 0;// 最后一次联合体的指针指向的是空, 类似链表的next指向空
            break;
        } else {
            __current_obj -> _M_free_list_link = __next_obj;//一直这样切19次, 就是20份, 挂到自由链表32对应的那里
        }
      }
    return(__result);//---return --lcl: 3)-----Step4-----

}  


//--------------------lcl: 3)-----Step4---------------------
//1、申请32个字节的时候
//---------------goto-----lcl: 3)-----Step5-------看分析----------
static void* allocate(size_t __n)//32
{
	else
	{
		_Obj** __my_free_list = _S_free_list + _S_freelist_index(__n);//_S_free_list[3]
    
      _Obj*  __result = *__my_free_list;
      if (__result == 0)
		  __ret = _S_refill(_S_round_up(__n));
	  
	  else 
	  {
		  *__my_free_list = __result -> _M_free_list_link;
		  __ret = __result;
	  }
	}
}

//2、申请64个字节的时候
// _S_chunk_alloc 征用了刚才申请32字节 预留的 640内存池, 此时内存池就已经为空了
// 注意此时, 传入的__nobjs因为是&引用, 已经变成了10
char* _S_chunk_alloc(size_t __size, int& __nobjs)//64 20 对比 32 20
{
    char* __result;
    size_t __total_bytes = __size * __nobjs;// = 1280;   对比 640
    size_t __bytes_left = _S_end_free - _S_start_free;// = 1280 - 640 = 640; 
	
	 else if (__bytes_left >= __size)
	 {
        __nobjs = (int)(__bytes_left/__size) = 640/64 = 10;
        __total_bytes = __size * __nobjs = 64 * 10 = 640;
        __result = _S_start_free; // 640首地址
        _S_start_free += __total_bytes;
        return(__result);
    }

}


//_S_free_list[7]
	__my_free_list = _S_free_list + _S_freelist_index(__n);
//640字节切成10份挂在64下面
void* _S_refill(size_t __n)//传入64
{
    int __nobjs = 20;
    char* __chunk = _S_chunk_alloc(__n, __nobjs);//64 20
    _Obj* __STL_VOLATILE* __my_free_list;
    _Obj* __result;
    _Obj* __current_obj;
    _Obj* __next_obj;
    int __i;
	
	__my_free_list = _S_free_list + _S_freelist_index(__n);//_S_free_list[7]
	
	 __result = (_Obj*)__chunk;
      *__my_free_list = __next_obj = (_Obj*)(__chunk + __n);
      for (__i = 1; ; __i++) {
        __current_obj = __next_obj;
        __next_obj = (_Obj*)((char*)__next_obj + __n);
        if (__nobjs - 1 == __i) {
            __current_obj -> _M_free_list_link = 0;
            break;
        } else {
            __current_obj -> _M_free_list_link = __next_obj;
        }
      }
    return(__result);

}
static void* allocate(size_t __n)//64
  {
    void* __ret = 0;

 
    else 
	{
      _Obj** __my_free_list = _S_free_list + _S_freelist_index(__n);//_S_free_list[7]

      _Obj*  __result = *__my_free_list;
      if (__result == 0)
        __ret = _S_refill(_S_round_up(__n));//_S_round_up(__n)还是64
      else {
        *__my_free_list = __result -> _M_free_list_link;
        __ret = __result;
      }
    }

    return __ret;
  };


//3、申请96个字节的时候(此时因为内存池子是空的).

char* _S_chunk_alloc(size_t __size, int& __nobjs)//96 20
{
    char* __result;
    size_t __total_bytes = __size * __nobjs;// = 96 * 20 = 1920;
    size_t __bytes_left = _S_end_free - _S_start_free = 0;// 内存池子是空的
	
	else 
	{								// >> 4 就是 除以 2的4次方, 除以16
        size_t __bytes_to_get = 2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
		                          //= 2 * 1920 + _S_round_up(1280 >> 4)
								  //= 3840 + 80 = 3920;
								  //因为3920 > 3840; 

		 _S_start_free = (char*)malloc(__bytes_to_get) = malloc(3920);
		 
		  _S_heap_size += __bytes_to_get;// = 1280 + 3920 = 5200;
         _S_end_free = _S_start_free + __bytes_to_get;
        return(_S_chunk_alloc(__size, __nobjs));
		
					//递归调用
					char* __result;
			        size_t __total_bytes = __size * __nobjs = 96 * 20 = 1920;
			        size_t __bytes_left = _S_end_free - _S_start_free = 3920;
					
					if (__bytes_left >= __total_bytes) {//3920  1920
			        __result = _S_start_free;
			        _S_start_free += __total_bytes;
			        return(__result);
    }
	}

}




void* _S_refill(size_t __n)//96
{
    int __nobjs = 20;
    char* __chunk = _S_chunk_alloc(__n, __nobjs);//96 20
    _Obj* __STL_VOLATILE* __my_free_list;
    _Obj* __result;
    _Obj* __current_obj;
    _Obj* __next_obj;
    int __i;
	
	 __my_free_list = _S_free_list + _S_freelist_index(__n);

    /* Build free list in chunk */
      __result = (_Obj*)__chunk;
      *__my_free_list = __next_obj = (_Obj*)(__chunk + __n);
      for (__i = 1; ; __i++) {
        __current_obj = __next_obj;
        __next_obj = (_Obj*)((char*)__next_obj + __n);
        if (__nobjs - 1 == __i) {
            __current_obj -> _M_free_list_link = 0;
            break;
        } else {
            __current_obj -> _M_free_list_link = __next_obj;
        }
      }
    return(__result);

}


static void* allocate(size_t __n)//96
  {
    void* __ret = 0;

 
    else 
	{
      _Obj** __my_free_list = _S_free_list + _S_freelist_index(__n);//_S_free_list[11]

      _Obj*  __result = *__my_free_list;
      if (__result == 0)
        __ret = _S_refill(_S_round_up(__n));
      else {
        *__my_free_list = __result -> _M_free_list_link;
        __ret = __result;
      }
    }

    return __ret;
  };


//4、申请72个字节的时候,此时堆空间耗尽，内存池也耗尽（可以申请成功的，从大于72的下标借）
//从96哪里借的96字节，并且将96切分为72 与24（留到内存池）

char* _S_chunk_alloc(size_t __size, int& __nobjs)//72 20
{
    char* __result;
    size_t __total_bytes = __size * __nobjs = 72 * 20 = 1440;
    size_t __bytes_left = _S_end_free - _S_start_free = 0;
	
	else 
	{
        size_t __bytes_to_get = 2 * __total_bytes + _S_round_up(_S_heap_size >> 4)
		                          = 2 * 1440 > 2880 ;
		_S_start_free = (char*)malloc(__bytes_to_get) ;
		
		if (0 == _S_start_free)//这是之前没有走过的代码, 因为malloc申请不到了

	    {
			size_t __i;
            _Obj* __STL_VOLATILE* __my_free_list;
	        _Obj* __p;
			
			//__i = __size = 72 
			//_MAX_BYTES = 128
			//_ALIGN = 8
			//72  80  88  96直到96不为空
			for (__i = __size; __i <= (size_t) _MAX_BYTES; __i += (size_t) _ALIGN) 
			{
				 __my_free_list = _S_free_list + _S_freelist_index(__i);//_S_free_list[11]
				 __p = *__my_free_list;
				 
				 if (0 != __p) 
				 {
                    *__my_free_list = __p -> _M_free_list_link;
                    _S_start_free = (char*)__p;
                    _S_end_free = _S_start_free + __i;
                    return(_S_chunk_alloc(__size, __nobjs));
					
					//递归调用
					 char* __result;
					 size_t __total_bytes = __size * __nobjs = 72 * 20 = 1440;
					 size_t __bytes_left = _S_end_free - _S_start_free = 96;
					 
					 else if (__bytes_left >= __size) 
					 {
                      __nobjs = (int)(__bytes_left/__size) = 96/72 = 1;
                      __total_bytes = __size * __nobjs = 72 * 1 = 72;
                      __result = _S_start_free;
                      _S_start_free += __total_bytes;
                      return(__result);
                     }
                   
                }
			}
		}
	}

}
void* _S_refill(size_t __n)//72
{
    int __nobjs = 20;
    char* __chunk = _S_chunk_alloc(__n, __nobjs);//72 20  传出来的时候__nobjs = 1
    _Obj* __STL_VOLATILE* __my_free_list;
    _Obj* __result;
    _Obj* __current_obj;
    _Obj* __next_obj;
    int __i;
	
	if (1 == __nobjs) return(__chunk);// 这里有个小技巧1写在前面

}


static void* allocate(size_t __n)//72
  {
    void* __ret = 0;

 
    else 
	{
      _Obj** __my_free_list = _S_free_list + _S_freelist_index(__n);//_S_free_list[8]

      _Obj*  __result = *__my_free_list;
      if (__result == 0)
        __ret = _S_refill(_S_round_up(__n));
      else {
        *__my_free_list = __result -> _M_free_list_link;
        __ret = __result;
      }
    }

    return __ret;
  }




//内存的释放
  static void deallocate(void* __p, size_t __n)//32
  {
    if (__n > (size_t) _MAX_BYTES)//128
      malloc_alloc::deallocate(__p, __n);//free
    else {
      _Obj**  __my_free_list = _S_free_list + _S_freelist_index(__n);//_S_free_list[3]
      _Obj* __q = (_Obj*)__p;


      __q -> _M_free_list_link = *__my_free_list;
      *__my_free_list = __q;

    }
  }









