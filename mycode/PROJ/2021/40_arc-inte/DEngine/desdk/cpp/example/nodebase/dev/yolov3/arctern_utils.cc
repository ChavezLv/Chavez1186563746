#include "de_arctern_utils.h"
#include "de_arctern_node.h"
// #include "de_arctern_manager.h"

namespace de
{

namespace arctern
{

float Fp16ToFp32(uint16_t f)
{
    uint16_t *i = (uint16_t*)&f;
    uint32_t sign = (*i >> 15) & 0x1;
    uint32_t exponent = ((*i >> 10) & 0x1f) - 0xf;
    uint32_t mantissa = (*i) & 0x3ff;
    if (exponent == 0x10)
        return 0;

    exponent += 0x7f;
    uint32_t ret = ((sign & 0x1) << 31);
    ret |= (exponent & 0xff) << 23;
    ret |= (mantissa << 13) & 0x7fffff;
    float result = *((float*)&ret);
    return result;
}

uint16_t Fp32ToFp16(float f) 
{ 
    // IEEE-754 16-bit floating-point format (without infinity): 1-5-10, exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
    // round-to-nearest-even: add last bit after truncated mantissa
    uint32_t b = *(uint32_t*)&f + 0x00001000; 

    // exponent
    uint32_t e = (b & 0x7F800000) >> 23; 

    // mantissa; in line below: 0x007FF000 = 0x00800000-0x00001000 = decimal indicator flag - initial rounding   
    uint32_t m = b & 0x007FFFFF; 

    // sign : normalized : denormalized : saturate
    uint16_t result = (b & 0x80000000) >> 16 | (e > 112) * ((((e - 112) << 10) & 0x7C00) | m >> 13) 
            | ((e < 113) & (e > 101)) * ((((0x007FF000 + m) >> (125 - e)) + 1) >> 1) | (e > 143) * 0x7FFF;
    
    return result;
}

void get_fp16_info(uint16_t f, int32_t& sign, int32_t& exponent, int32_t& mantissa)
{
    uint16_t *i = (uint16_t*)&f;
    sign = (*i >> 15) & 0x1;
    //exponent = ((*i >> 10) & 0x1f) - 0xf;
    exponent = (*i >> 10) & 0x1f; //不减0xf，后续比较也不减0xf
    mantissa = (*i) & 0x3ff;
}

deRect check_shrink_box(const deRect& box, int img_w, int img_h)
{
    int x1 = box.x;
    int y1 = box.y;
    int x2 = box.x + box.w - 1;
    int y2 = box.y + box.h - 1;
    x1 = std::max(0, std::min(x1, img_w - 1));
    y1 = std::max(0, std::min(y1, img_h - 1));
    x2 = std::max(0, std::min(x2, img_w - 1));
    y2 = std::max(0, std::min(y2, img_h - 1));
    deRect ret = {x1, y1, x2 - x1 + 1, y2 - y1 + 1};
    return ret;
}

deRect extend_box(const deRect &box, int32_t img_w, int32_t img_h, float ext_top_scale, float ext_bottom_scale)
{
    int w = box.w + 1;
    int h = box.h + 1;
    float x1 = box.x;
    float y1 = box.y;
    float x2 = box.x + box.w - 1;
    float y2 = box.y + box.h - 1;
    y1 = y1 - h * ext_top_scale;
    y2 = y2 + h * ext_bottom_scale;
    int by1 = std::min(std::max(0, (int)(y1 + 0.5)), img_h - 1);
    int by2 = std::min(std::max(0, (int)(y2 + 0.5)), img_h - 1);
    int hh = by2 - by1 + 1;
    int ww = hh;
    if(ww > w)
    {
        x1 = x1 - (ww - w) / 2;
        x2 = x2 + (ww - w) / 2;
    }
    int bx1 = std::min(std::max(0, (int)(x1 + 0.5)), img_w - 1);
    int bx2 = std::min(std::max(0, (int)(x2 + 0.5)), img_w - 1);
    deRect ret = {bx1, by1, bx2-bx1+1, by2-by1+1};
    return ret;
}

} // namespace arctern
} // namespace de
