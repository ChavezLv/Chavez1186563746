#include "toolbox.h"


namespace de
{

#ifndef USE_DP1000_PLATFORM
::cv::Mat Ndarray2CvMat(const de::NDArray &nd, deRect *rect_, dePixelFormat fmt)
{
    CHECK((fmt == dePixelFormat::DE_PIX_FMT_BGR888) || (fmt == dePixelFormat::DE_PIX_FMT_GRAY8));
    const DLTensor *tensor = nd.GetTensor();
    const ExtInfo* ext_info_ptr = nd.GetExtInfo();
    ::cv::Rect cv_rect;    
    if(rect_ != nullptr)cv_rect = DeRect2CvRect(*rect_);
    if (ext_info_ptr->format == dePixelFormat::DE_PIX_FMT_NV12)
    {
        ::cv::Mat img_nv12,img_bgr, img_y, img_dst;        
        if(fmt == dePixelFormat::DE_PIX_FMT_GRAY8)
        {
            CHECK(tensor->shape[0] % 3 == 0);
            img_y.create(tensor->shape[0] * 2 / 3, tensor->shape[1], CV_8UC1);
            memcpy(img_y.data, nd.GetTensorData(), img_y.rows*img_y.cols);            
            img_dst = img_y;
        }
        else
        {
            img_nv12.create(tensor->shape[0], tensor->shape[1], CV_8UC1);
            memcpy(img_nv12.data, nd.GetTensorData(), nd.GetTensorDataSize());
            ::cv::cvtColor(img_nv12, img_bgr, ::cv::COLOR_YUV2BGR_NV12);
            img_dst = img_bgr;
        }
        if(rect_ != nullptr)
            return img_dst(cv_rect);
        else
            return img_dst;
    }
    else if (ext_info_ptr->format == dePixelFormat::DE_PIX_FMT_BGR888)
    {
        ::cv::Mat img(tensor->shape[0] / 3, tensor->shape[1], CV_8UC3);
        memcpy(img.data, nd.GetTensorData(), nd.GetTensorDataSize());        
        ::cv::Mat img_yuv, img_dst;
        img_dst = img;
        if(fmt == dePixelFormat::DE_PIX_FMT_GRAY8)
        {
            ::cv::cvtColor(img, img_yuv, ::cv::COLOR_BGR2YUV_IYUV);
            ::cv::Mat img_y(img.rows, img.cols, CV_8UC1,img_yuv.data);
            img_dst = img_y;
        }

        if(rect_ != nullptr)
            return img_dst(cv_rect);
        else
            return img_dst;
    }
    else if (ext_info_ptr->format == dePixelFormat::DE_PIX_FMT_MJPEG)
    {
        std::vector<uint8_t> data((uint8_t*)nd.GetTensorData(), (uint8_t*)nd.GetTensorData()+nd.GetTensorDataSize());
        ::cv::Mat img;
        if(nd.GetTensorDataSize() > 0)img = ::cv::imdecode(data, CV_LOAD_IMAGE_COLOR);        
        if((rect_ != nullptr) && (!img.empty()))
            return img(cv_rect);
        else
            return img;
    }
    else
    {
        ::cv::Mat img;
        CHECK(0) << "unsupport format, format=" << (int32_t)ext_info_ptr->format;
        return img;
    }
}



de::NDArray CvMat2Ndarray(const ::cv::Mat &img, dePixelFormat format)
{
    CHECK(!img.empty());
    CHECK(img.channels() == 3);
    std::vector<int64_t> shape;
    DLDataType dtype = {kDLInt, 8, 1};
    ExtInfo info = {format, deShapeCode::HW, {0, 0, img.cols, img.rows}};
    de::NDArray nd ;
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


deRect CvRect2DeRect(const ::cv::Rect& rect)
{
    deRect r;
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.width;
    r.h = rect.height;
    return r;
}

::cv::Rect DeRect2CvRect(const deRect& rect)
{
    ::cv::Rect r;
    r.x = rect.x;
    r.y = rect.y;
    r.width = rect.w;
    r.height = rect.h;
    return r;
}

#endif 

DE_DLL de::NDArray ReadImg(const std::string& img_file, dePixelFormat fmt)
{
#ifndef USE_DP1000_PLATFORM
    CHECK((fmt == dePixelFormat::DE_PIX_FMT_BGR888)
          || (fmt == dePixelFormat::DE_PIX_FMT_RGB888_PLANE)
          || (fmt == dePixelFormat::DE_PIX_FMT_NV12)) << "x86 arctern only support bgr formate";
    auto img = ::cv::imread(img_file);
    auto img_nd = CvMat2Ndarray(img, fmt);
    
    //add bear data corrspond to videoDecodeNode
    struct DecBareData baredata;
	auto to_epoch = std::chrono::steady_clock::now().time_since_epoch();
	auto time_now = std::chrono::duration_cast<std::chrono::milliseconds>(to_epoch);
	baredata.dts = time_now.count();
    de::WriteDataToBearArea<DecBareData>(img_nd, baredata);
    return img_nd;
#endif
}
DE_DLL de::NDArray get_image(dePixelFormat fmt, int width, int height, const char* pic_path)
{

    LOG(DEBUG) << "get_image start! fmt:" << fmt << ",w:" << width << ",h:" << height;

    int real_width = width;
    int real_height = height;
    std::vector<int64_t> shape;
    ExtInfo addition;
    switch (fmt)
    {
    case dePixelFormat::DE_PIX_FMT_NV12:
    case dePixelFormat::DE_PIX_FMT_NV21:
        real_height = height * 3 / 2;
        shape.push_back(real_height);
        shape.push_back(real_width);
        addition.shape_indicator = deShapeCode::HW;
        break;
    case dePixelFormat::DE_PIX_FMT_RGB888:
    case dePixelFormat::DE_PIX_FMT_BGR888:
        shape.push_back(real_height * 3);
        shape.push_back(real_width);
        addition.shape_indicator = deShapeCode::HW;
        break;
    case dePixelFormat::DE_PIX_FMT_GRAY8:
        shape.push_back(real_height);
        shape.push_back(real_width);
        addition.shape_indicator = deShapeCode::HW;
        break;
    default:
        CHECK(0) << "get_image fmt error!";
        break;
    }

    addition.format = fmt;
    addition.bear_data_len = 0;
    addition.pts = 0;
    addition.roi_rect = {0, 0, width, height};
    de::NDArray image = de::NDArray::Empty(shape, {kDLUInt, 8, 1},
                                           DEContext{kDLCPU, 0}, addition);
    //image.SaveTensorInfo();
    LOG(INFO) << "img format:" << image.GetExtInfo()->format << ",roi:[" << image.GetExtInfo()->roi_rect.x
              << "," << image.GetExtInfo()->roi_rect.y << "," << image.GetExtInfo()->roi_rect.w << "," << image.GetExtInfo()->roi_rect.h;
    if(pic_path)
    {
        std::string file = pic_path;
        std::ifstream f(file, std::ios_base::binary | std::ios_base::out);
        if(f)
        {
            f.read(reinterpret_cast<char*>(image.GetTensorData()), image.GetTensorDataSize());
            f.close();
        }
        else
        {
            LOG(ERROR) << "file open fail! " << file;
        }
    }
    return image;
}

int32_t GetImgNdWidthHeight(const de::NDArray &nd, int32_t &width, int32_t &height)
{
    const ExtInfo* ext = nd.GetExtInfo();
    switch(ext->format)
    {
    case dePixelFormat::DE_PIX_FMT_NV12:
    case dePixelFormat::DE_PIX_FMT_NV21:
        CHECK(nd.GetTensor()->dtype.lanes == 1);
        CHECK(ext->shape_indicator == deShapeCode::HW);
        height = nd.GetTensor()->shape[0] * 2 / 3;
        width = nd.GetTensor()->shape[1];
        break;
    case dePixelFormat::DE_PIX_FMT_YUV422_YUYV:
    case dePixelFormat::DE_PIX_FMT_YUV422_UYVY:
        CHECK(nd.GetTensor()->dtype.lanes == 1);
        CHECK(ext->shape_indicator == deShapeCode::HW);
        height = nd.GetTensor()->shape[0] * 1 / 2;
        width = nd.GetTensor()->shape[1];
        break;
    case dePixelFormat::DE_PIX_FMT_RGB888:
    case dePixelFormat::DE_PIX_FMT_BGR888:
        CHECK(nd.GetTensor()->dtype.lanes == 1);
        CHECK(ext->shape_indicator == deShapeCode::HW);
        height = nd.GetTensor()->shape[0] / 3;
        width = nd.GetTensor()->shape[1];
        break;
    default:
        CHECK(0) << "FORMAT NOT SUPPORT!";
        break;
    }
    return 0;
}








}
