
#include "infer_engine.h"
#include "de_ndarray.h"
#include "de_c_type.h"
#include <opencv2/opencv.hpp>

de::NDArray CvMat2Ndarray(const ::cv::Mat &img, dePixelFormat format)
{
    CHECK(!img.empty());
    CHECK(img.channels() == 3);
    std::vector<int64_t> shape;
    DLDataType dtype = {kDLInt, 8, 1};
    ExtInfo info = {format, deShapeCode::HW, {0, 0, img.cols, img.rows}};
    de::NDArray nd;
    int len;

    switch (format)
    {
    case dePixelFormat::DE_PIX_FMT_BGR888:
    {
        shape.push_back(img.rows * 3);
        shape.push_back(img.cols);
        info.shape_indicator = deShapeCode::HW ;
        nd = de::NDArray::Empty(shape, dtype, { kDLCPU, 0 }, info);
        len = nd.GetTensorDataSize();
        memcpy(nd.GetTensorData(), img.data, len);
    }
    break;
    case dePixelFormat::DE_PIX_FMT_RGB888_PLANE:
    {
        shape.push_back(img.rows * 3);
        shape.push_back(img.cols);
        info.shape_indicator = deShapeCode::HW ;
        nd = de::NDArray::Empty(shape, dtype, { kDLCPU, 0 }, info);
        len = nd.GetTensorDataSize();
        CHECK(len == 3 * img.rows * img.cols);

        uint8_t *p = (uint8_t*)nd.GetTensorData() ;
        for(uint32_t ch = 0; ch < 3; ch++)    // BGR ==> RGB_PLANE
        {
            uint32_t ch_bit = ch;
            if(ch == 0) ch_bit = 2;
            if(ch == 2) ch_bit = 0;
            for(int32_t r = 0; r < img.rows; r++)
            {
                for(int32_t c = 0; c < img.cols; c++)
                {
                    p[ch * img.rows * img.cols + r * img.cols + c] = img.data[r * img.cols * 3 + c * 3 + ch_bit];
                }
            }
        }
    }
    break;
    case dePixelFormat::DE_PIX_FMT_NV12:
    {
        shape.push_back(img.rows * 3 / 2);
        shape.push_back(img.cols);
        info.shape_indicator = deShapeCode::HW ;
        dtype.lanes = 1;
        nd = de::NDArray::Empty(shape, dtype, { kDLCPU, 0 }, info);
        len = nd.GetTensorDataSize();

        ::cv::Mat yuv;
        ::cv::cvtColor(img, yuv, CV_BGR2YUV_I420);
        uchar *buf = new uchar[img.rows * img.cols / 2];
        memcpy(buf, yuv.data + img.rows * img.cols, img.rows * img.cols / 2);
        for(int32_t i = 0; i < img.rows * img.cols / 4; i++)
        {
            yuv.data[img.rows * img.cols + 2 * i] = buf[i];
            yuv.data[img.rows * img.cols + 2 * i + 1] = buf[img.rows * img.cols / 4 + i];
        }
        delete buf;

        memcpy(nd.GetTensorData(), yuv.data, len);
    }
    break;
    default:
        CHECK(0) << "unsupport format, format=" << (int32_t)format;
    }
    //nd.SaveTensorInfo();
    return nd;
}

int main() {
  void *hdl = de::LoadLibrary(0, "host:/DEngine/desdk/platform/dev_linux-dp1000/lib/libloongson_education.so");

  {
    std::string net_file = "/DEngine/model/dp1000/caffe_squeezenet_v1.1/net.bin";
    std::string model_file = "/DEngine/model/dp1000/caffe_squeezenet_v1.1/model.bin";
    de::InferEngine engine(net_file, model_file);
    cv::Mat img = cv::imread("/DEngine/data/pic/1.jpg");
    // 不支持BGR888格式，需要转换为RGB888_PLANE格式
    auto nd = CvMat2Ndarray(img, dePixelFormat::DE_PIX_FMT_RGB888_PLANE);
    std::vector<de::NDArray> nd_in, nd_out;
    nd_in.emplace_back(nd);
    engine.Predict(nd_in, nd_out);
    printf("<====Predict result tensor num=%d\n", nd_out.size());
    for (int i = 0; i < nd_out.size(); i++)
      printf("tenser[%d] size=%d\n", i, nd_out[i].GetTensorDataSize());
  }

  de::UnloadLibrary(0, hdl);
}