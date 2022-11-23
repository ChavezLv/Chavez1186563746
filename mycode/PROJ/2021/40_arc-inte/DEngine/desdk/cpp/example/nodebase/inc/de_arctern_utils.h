/**
 * @file de_arctern_utils.h
 * @brief
 * @author Desdk Group
 * @version
 * @date 2020-03-31
 */
#ifndef DE_ARCTERN_UTILS_H_
#define DE_ARCTERN_UTILS_H_

#include <core/de_ndarray.h>
#include <core/de_thread.h>
#include <fstream>
#ifdef USE_X86_PLATFORM
#include <opencv2/opencv.hpp>
#include <arctern_manager.h>
#include "de_arctern_manager.h"
#endif

#include "de_arctern_node.h"
#include "toolbox.h"

namespace de
{
namespace arctern
{




float Fp16ToFp32(uint16_t f);
deRect check_shrink_box(const deRect& box, int img_w, int img_h);
deRect extend_box(const deRect &box, int32_t img_w, int32_t img_h, float ext_top_scale, float ext_bottom_scale);

uint16_t Fp32ToFp16(float f);
void get_fp16_info(uint16_t f, int32_t& sign, int32_t& exponent, int32_t& mantissa);

inline int fp16_compare_with_positive_f16(uint16_t fa, int32_t exponent_b, int32_t mantissa_b)
{
    return ((fa>>15)&0x1)==0 ? ((fa>>10)>exponent_b ? 1 : ((fa>>10)==exponent_b ? ((fa&0x3ff)>mantissa_b ? 1 : -1) : -1)) : -1;
}

/**
 * @brief convert from license-feat  to string
 *
 * @param vector<uint8_t>
 *
 * @return
 */

DE_DLL std::string LicenseVecToString(std::vector<uint8_t> &license) ;

#ifdef USE_X86_PLATFORM
int32_t MultiCatType2DeCatType(::arctern::MultiCatType cat_type);

//---------------------------------------------------------------
//
//                  AiE (X86)
//
//--------------------------------------------------------------
class DE_DLL AiEX86 : public de::Thread
{
public:
    using PreProcFunc = std::function< std::vector<de::NDArray >(void*, int32_t, void*)>;
    using PostProcFunc = std::function< void* (void* , int32_t, std::vector<de::NDArray>, void*)>;
    AiEX86(bool stream_mode = true);
    ~AiEX86() {}
    void SetPreProc(PreProcFunc pre_func);
    void SetPostProc(int pout_idx, PostProcFunc post_func);
    virtual void UpdatePara(void) override;

protected:
    bool stream_mode_ = true;
    std::string model_path;
    de::arctern::Manager manager_;
    PreProcFunc pre_func_ = nullptr;
    PostProcFunc post_funcs_[kMaxPoutNum] = {nullptr, };

};
#endif


//---------------------------------------------------
//
//         OSD  
//
//--------------------------------------------------

/**
 * @brief Write Qual result to image file
 *
 * @param data
 * @param dir_path
 */
DE_DLL void WriteImgToDir(de::arctern::ObjQualRsltImg* data, const std::string dir_path);


/**
 * @brief Write Attr result to image file
 *
 * @param req
 * @param rslt
 * @param dir_path
 */
DE_DLL void WriteImgToDir(de::arctern::ObjAttrReq* req, de::arctern::ObjAttrRslt* rslt, const std::string dir_path);


/**
 * @brief Write MultiAttr result to image file
 *
 * @param req
 * @param rslt
 * @param dir_path
 */
DE_DLL void WriteImgToDir(de::arctern::ObjAttrReq* req, de::arctern::ObjMultiAttrRslt* rslt, const std::string dir_path);


/**
 * @brief Write PersonMultiAttrRslt result to image file
 *
 * @param req
 * @param rslt
 * @param dir_path
 */
DE_DLL void WriteImgToDir(de::arctern::ObjAttrReq* req, de::arctern::PersonMultiAttrRslt* rslt, const std::string dir_path);


/**
 * @brief Write MultiDetRsltImg result to image file
 *
 * @param req
 * @param rslt
 * @param dir_path
 */
DE_DLL void WriteImgToDir(de::arctern::MultiDetRsltImg* data, const std::string dir_path);


/**
 * @brief Adjust rect within the image according to image width & height
 *
 * @param width
 * @param height
 * @param rect
 */
void ProtectRectBoundary(int32_t width, int32_t height, deRect& rect);


/**
 * @brief Adjust rects within the image according to image width & height
 *
 * @param width
 * @param height
 * @param rects
 */
void ProtectRectBoundary(int32_t width, int32_t height, std::vector<deRect>& rects);



} // namespace arctern
} // namespace de


#endif
