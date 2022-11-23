//
// Created by Admin on 2019/6/18.
//

#include "gtest/gtest.h"
#include "../ImgPreprocessorOnNppEx.h"
#include "../buffers.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include <iostream>
#include <iomanip>
using namespace std;
TEST(NppImgPreprocess,ExtractChannel001){
  ImgPreprocessEx::ImgPreprocessorOnNppEx NppProc;
  float *inputData = new float[27];
  int rows,cols;
  rows = cols = 3;
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      for(int c=0;c<3;c++){
        inputData[i*cols*3 + j*3 +c] = 1.0 + c;
      }
    }
  }
  cout.fill(' ');
  cout.setf(ios::fixed);
  cout.precision(3);

  for(int i=0;i<27;i++){
    cout <<(float) inputData[i]<<" ";
  }
  cout <<endl;
  ImgPreprocessEx::DeviceBuffer input(rows,cols,3,nvinfer1::DataType::kFLOAT,1);
  cudaMemcpy(input.data(),static_cast<void*>(inputData),input.nbBytes(),cudaMemcpyHostToDevice);
  ImgPreprocessEx::DeviceBuffer  output(rows,cols,3,nvinfer1::DataType::kFLOAT,1);
  NppProc.ExtractChannel(input,output);

  float * outputData = new float [27];
  cudaMemcpy(static_cast<void *>(outputData),output.data(),output.nbBytes(),cudaMemcpyDeviceToHost);
  for(int c=0;c<3;c++) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        int pos = rows*cols*c + i * cols + j;
        cout <<setw(5)<<(float ) outputData[pos] <<" ";
        EXPECT_EQ((float ) outputData[pos],c+1);
      }
      cout << endl;
    }
    cout <<endl;
  }
  delete[] inputData;
  delete[] outputData;
}

TEST(NppImgPreprocess,Mergel001){
  ImgPreprocessEx::ImgPreprocessorOnNppEx NppProc;
  vector<float> r(9);
  vector<float> g(9);
  vector<float >b(9);
  int rows,cols;

  rows = cols = 3;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      r[i*cols + j] = 1;
      g[i*cols + j] = 2;
      b[i*cols + j] = 3;
    }
  }
  void * gpuData;
  ImgPreprocessEx::DeviceBuffer input(rows,cols,1,nvinfer1::DataType::kFLOAT,1);
  ImgPreprocessEx::DeviceBuffer input1(rows,cols,1,nvinfer1::DataType::kFLOAT,1);
  ImgPreprocessEx::DeviceBuffer input2(rows,cols,1,nvinfer1::DataType::kFLOAT,1);

  cudaMemcpy(input.data(),static_cast<void*>(r.data()),input.nbBytes(),cudaMemcpyHostToDevice);
  cudaMemcpy(input1.data(),static_cast<void*>(g.data()),input1.nbBytes(),cudaMemcpyHostToDevice);
  cudaMemcpy(input2.data(),static_cast<void*>(b.data()),input2.nbBytes(),cudaMemcpyHostToDevice);

  vector<ImgPreprocessEx::DeviceBuffer> vSrc(3);
  vSrc[0] = input;
  vSrc[1] = input1;
  vSrc[2] = input2;

  ImgPreprocessEx::DeviceBuffer output(rows,cols,3,nvinfer1::DataType::kFLOAT,1);
  NppProc.Merge(vSrc,output);


  cout.fill(' ');
  cout.setf(ios::fixed);
  cout.precision(3);
  vector<float> dst(27);
  cudaMemcpy(static_cast<void *>(dst.data()),output.data(),output.nbBytes(),cudaMemcpyDeviceToHost);

  for(int c=0;c<3;c++) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        int pos = rows*cols*c + i * cols + j;
        cout <<setw(5)<<(float ) dst[pos] <<" ";
        //EXPECT_EQ((float ) dst[pos],1);
      }
      cout << endl;
    }
    cout <<endl;
  }
}

