
/**
 * @file toolbox.h
 * @brief
 * @author Desdk Group
 * @version
 * @date 2020-03-31
 */
#ifndef UTILS_H_
#define UTILS_H_

#include <core/de_ndarray.h>
#include <fstream>
#ifndef USE_DP1000_PLATFORM
#include <opencv2/opencv.hpp>
#endif



namespace de
{

template<typename ... Args>
std::string FormatString(const std::string& format, Args ... args)
{
    size_t size = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
};


#ifndef USE_DP1000_PLATFORM

/**
 * @brief ndarray convert to cv mat
 *
 * @param nd
 *
 * @return
 */
DE_DLL ::cv::Mat Ndarray2CvMat(const de::NDArray &nd, deRect *rect_=nullptr, dePixelFormat format = DE_PIX_FMT_BGR888);
/**
 * @brief cv bgr mat  convert to ndarray
 *
 * @param img : bgrbgrbgr....
 * @pram format:  output format for returned NDArray
 *
 * @return
 */
//extern de::NDArray CvMat2Ndarray(const ::cv::Mat &img, dePixelFormat format = DE_PIX_FMT_BGR888);
 de::NDArray CvMat2Ndarray(const ::cv::Mat &img, dePixelFormat format = DE_PIX_FMT_BGR888);

/**
 * @brief cv rect convert to de rect
 *
 * @param rect
 *
 * @return
 */
//extern deRect CvRect2DeRect(const ::cv::Rect& rect);
 deRect CvRect2DeRect(const ::cv::Rect& rect);
/**
 * @brief de rect convert to cv rect
 *
 * @param rect
 *
 * @return
 */
DE_DLL ::cv::Rect DeRect2CvRect(const deRect& rect);


#endif

/**
 * @brief read from jpeg file
 *
 * @param img_file file path
 * @param fmt
 *
 * @return
 */
//extern de::NDArray ReadImg(const std::string& img_file, dePixelFormat fmt = dePixelFormat::DE_PIX_FMT_BGR888);
 de::NDArray ReadImg(const std::string& img_file, dePixelFormat fmt = dePixelFormat::DE_PIX_FMT_BGR888);


/**
 * @brief move to desdk
 *
 * @param nd
 * @param width
 * @param height
 *
 * @return
 */
 int GetImgNdWidthHeight(const de::NDArray &nd, int32_t &width, int32_t &height);
//extern  int GetImgNdWidthHeight(const de::NDArray &nd, int32_t &width, int32_t &height);
de::NDArray get_image(dePixelFormat fmt, int width, int height, const char* pic_path) ;
//extern de::NDArray get_image(dePixelFormat fmt, int width, int height, const char* pic_path) ;


//---------------------------------------------------
//
//          Pic Draw
//
//--------------------------------------------------
#ifndef USE_DP1000_PLATFORM

#define COLOR_RED  ::cv::Scalar(0, 0, 255)
#define COLOR_BLACK ::cv::Scalar       (0, 0, 0)
#define COLOR_GREEN ::cv::Scalar(0, 255, 0)
#define COLOR_BLUE     ::cv::Scalar(255, 0, 0)
#define COLOR_YELLOW ::cv::Scalar(0, 255, 255)
#define COLOR_CYAN ::cv::Scalar(255, 255, 0)


class PicDraw
{
public:

    PicDraw(de::NDArray& nd,  deRect *_rect = nullptr, int jpg_qual = 50)
    {
        pic = Ndarray2CvMat(nd, _rect);
        jpg_quality = jpg_qual;
    };

    PicDraw(::cv::Mat& _img_bgr) : pic(_img_bgr) {};
    ~PicDraw() {};


    PicDraw& AddRect(const ::cv::Rect& rect , ::cv::Scalar color = COLOR_RED)
    {
        int thickness = (pic.rows + pic.cols) / 500 / 3;
        ::cv::rectangle(pic, rect, color, thickness);
        return *this;
    }


    PicDraw& AddRect(const deRect& rect , ::cv::Scalar color = COLOR_RED)
    {
        ::cv::Rect cv_rect = DeRect2CvRect(rect);
        AddRect(cv_rect, color);
        return *this;
    }

    PicDraw& AddText(const std::string& label, ::cv::Point & pt, ::cv::Scalar color = COLOR_GREEN)
    {
        int fontface = ::cv::FONT_HERSHEY_TRIPLEX;
        double scale = 0.6 * pic.rows * 1e-3;
        int thickness = (pic.rows + pic.cols) / 500 / 5;
        ::cv::putText(pic, label, pt, fontface, scale, color, thickness, 8);
        return *this;
    }

    PicDraw& AddRectText(const ::cv::Rect& rect, std::vector<std::string> labels, bool rect_draw = true,
                         ::cv::Scalar rect_color = COLOR_RED, ::cv::Scalar text_color = COLOR_GREEN)
    {
        if(rect_draw)
        {
            AddRect(rect, rect_color);
        }
        //::cv::Point p = { rect.x + rect.width, rect.y + linePix };
		::cv::Point p = { rect.x + 10, rect.y + linePix };
        for (auto it = labels.begin(); it != labels.end(); ++it)
        {
            AddText(*it, p, text_color);
            p.y += linePix;
        }
        return *this;
    }

    PicDraw& AddRectText(const deRect& rect, std::vector<std::string> labels, bool rect_draw = true,
                         ::cv::Scalar rect_color = COLOR_RED, ::cv::Scalar text_color = COLOR_GREEN)
    {
        ::cv::Rect cv_rect = DeRect2CvRect(rect);
        return AddRectText(cv_rect, labels, rect_draw, rect_color, text_color);
    }


    void Save(const std::string& dst_file_name)
    {
        std::vector<int> compress_param;
        compress_param.push_back(CV_IMWRITE_JPEG_QUALITY);
        compress_param.push_back(jpg_quality);
        ::cv::imwrite(dst_file_name, pic, compress_param);
    }

private:
    ::cv::Mat pic;
    const int linePix = 20;
    int jpg_quality = 90;
}; // PicDraw
#endif // USE_X86_PLATFORM


} // namespace de


#endif
