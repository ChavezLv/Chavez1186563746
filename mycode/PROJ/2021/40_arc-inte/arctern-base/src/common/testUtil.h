//
// Created by chengaohua on 2021/6/16.
//

#ifndef ARCTERN_BASE_SRC_COMMON_TESTUTIL_H_
#define ARCTERN_BASE_SRC_COMMON_TESTUTIL_H_

#include <iomanip>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
using namespace std;
#define OUTPUT

template<typename T>
inline void printfImg(const cv::Mat & img,int rows=15,int cols=15,string outToFile=""){
  // Mat img;
#ifdef OUTPUT
if(outToFile.size() > 0){
  std::ofstream fout(outToFile,ios::out | ios::binary);
  fout <<"H:"<<img.rows<<endl;
  fout <<"W:"<<img.cols<<endl;
  fout <<"C:"<<img.channels()<<endl;
  fout <<"E:"<<img.elemSize()<<endl;
  fout.write(reinterpret_cast<char*>(img.data),img.rows*img.cols*img.elemSize());
  fout.close();
}
else {
  rows = rows > img.rows ? img.rows : rows;
  cols = cols > img.rows ? img.cols : cols;

  cout.setf(ios::fixed);
  cout.fill(' ');
  cout.precision(3);
  int offset = 0;
  for (int r = offset; r < offset + rows; ++r) {
    const auto *data = img.ptr<T>(r);
    for (int c = offset; c < offset + cols; c += 1) {
      for (int k = 0; k < img.channels(); k++) {

        if (is_same<uchar, T>::value)
          cout << dec << setw(5) << (int) (data[c * img.channels() + k]) << " ";
        else
          cout << dec << setw(5) << (T) data[c * img.channels() + k] << " ";

      }
    }
    cout << endl;
  }
  cout << endl;
}
#endif //OUTPUT
}





#endif //ARCTERN_BASE_SRC_COMMON_TESTUTIL_H_
