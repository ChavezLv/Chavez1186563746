/**
 * @file 
 * @brief 
 * @author 
 * @version 1.0.0
 * @date 2019/02/25
 * @copyright Intellifusion Corp.
 */

#ifndef HAL_MM_JPEG_H_
#define HAL_MM_JPEG_H_

#include <core/de_c_type.h>

#ifdef  __cplusplus
extern "C" {
#endif


/**
* @brief jpeg decoder error code
*/
enum de_JpegErrorCode{
	JPEG_ERROR_NONE = 0,       ///< that is ok
	JPEG_ERROR_INVAL = -1,     ///< invalid parameter
	JPEG_ERROR_FORMAT = -2,    ///< unsupported format
	JPEG_ERROR_MODE = -3,      ///< unsupported encode mode
	JPEG_ERROR_OVERFLOW = -4,  ///< input/output buffer overflow
	JPEG_ERROR_HW = -5,        ///< codec hardware error
	JPEG_ERROR_STRM = -6,      ///< input source error
	JPEG_ERROR_INTERNAL = -7,  ///< software internal resource error
	                           ///< such as: system memory insufficient, 
	                           ///< codec not initialized, internal check failed, ...
	JPEG_ERROR_LAST = -8,      ///< error code boundary                     
};

#if 0   //use defination in de_type.h
/**
* @brief jpeg codec supported formats
*/
enum de_JpegFrameFmt {
	JPG_FMT_UNKNOWN = 0,
    JPG_FMT_YUV420_PLANAR,          ///< YYYY... UUUU... VVVV...
    JPG_FMT_NV12,                   ///< YYYY... UVUVUV...        
    JPG_FMT_NV21,                   ///< YYYY... VUVUVU...      
    JPG_FMT_YUV422_YUYV,            ///< YUYVYUYV...             
    JPG_FMT_YUV422_UYVY,            ///< UYVYUYVY... 
	
	/**
	* belows formats only supported by jpeg encoder
	*/
    JPG_FMT_RGB565,                 ///< 16-bit RGB 16bpp          
    JPG_FMT_BGR565,                 ///< 16-bit RGB 16bpp          
    JPG_FMT_RGB555,                 ///< 15-bit RGB 16bpp          
    JPG_FMT_BGR555,                 ///< 15-bit RGB 16bpp          
    JPG_FMT_RGB444,                 ///< 12-bit RGB 16bpp          
    JPG_FMT_BGR444,                 ///< 12-bit RGB 16bpp          
    JPG_FMT_RGB888,                 ///< 24-bit RGB 32bpp          
    JPG_FMT_BGR888,                 ///< 24-bit RGB 32bpp          
    JPG_FMT_RGB101010,              ///< 30-bit RGB 32bpp          
    JPG_FMT_BGR101010               ///< 30-bit RGB 32bpp 	
};
#endif


/*************************   jpeg decoder  ******************************/
typedef long de_JpegDecHandle;     ///< jpeg decoder handle

/**
* @brief jpeg picture encode information 
*/
typedef struct {
	unsigned int width;            ///< image output buffer width
	unsigned int height;           ///< image output buffer height
	unsigned int dis_width;        ///< image display width
	unsigned int dis_height;       ///< image display height
	int format;                    ///< image format, see enum PixelFormat
} de_JpegImgInfo;

/**
* @brief jpeg decoder input/output buffer information
*/
typedef struct{
	unsigned char *v_addr;    ///< virtual address
	unsigned int  p_addr;     ///< pyhsical address
	unsigned int  size;       ///< buffer size
}de_JpegDecBuf;

/**
* @brief de_JpegDecoderOpen
* Open a jpeg decoder and return a handle
* 
* @return 
*  @retval > 0  jpeg decoder handle
*  @retval < 0  jpeg decoder open failed, return error code, 
*               see enum de_JpegErrorCode
*/
DE_DLL de_JpegDecHandle de_JpegDecoderOpen(void);

/**
* @brief de_JpegDecoderGetInfo
* Get jpeg picture encode information
*
* @param handle      jpeg decoder handle created by de_JpegDecoderOpen()
* @param pic_in  jpeg picture input, see de_JpegDecBuf
* @param img_info    jpeg picture encode information, see de_JpegImgInfo
*
* @return  see enum de_JpegErrorCode
*/
DE_DLL int de_JpegDecoderGetInfo(de_JpegDecHandle handle, de_JpegDecBuf *pic_in, de_JpegImgInfo *img_info);

/**
* @brief de_JpegDecode
* decode a jpeg picture
* 
* @param handle   jpeg decoder handle created by de_JpegDecoderOpen()
* @param pic_in   jpeg picture input, see de_JpegDecBuf
* @param img_out  yuv image output, see de_JpegDecBuf
*
* @return  see enum de_JpegErrorCode
*/
DE_DLL int de_JpegDecode(de_JpegDecHandle handle, de_JpegDecBuf *pic_in, de_JpegDecBuf *img_out);

/**
* @brief de_JpegDecoderClose
* Close jpeg decoder and release resource 
*
* @param handle  jpeg decoder handle created by de_JpegDecoderOpen()
*/
DE_DLL void de_JpegDecoderClose(de_JpegDecHandle handle);


/*************************   jpeg encoder  ******************************/
typedef long de_JpegEncHandle;            ///< jpeg encoder handle

/**
* @brief jpeg encoder Quantization Level
* the value is bigger, the defination of jpeg is higher
*/
enum de_EncQuality {
    ENC_QUALITY_LOW = 3,
    ENC_QUALITY_NORMAL = 7,
    ENC_QUALITY_HIGH = 8,
    
    ENC_QUALITY_0 = 0,
    ENC_QUALITY_1 = 1,
    ENC_QUALITY_2 = 2,
    ENC_QUALITY_3 = 3,
    ENC_QUALITY_4 = 4,
    ENC_QUALITY_5 = 5,
    ENC_QUALITY_6 = 6,
    ENC_QUALITY_7 = 7,
    ENC_QUALITY_8 = 8,
    ENC_QUALITY_9 = 9,
    ENC_QUALITY_10 = 10,
};

/**
* @brief jpeg encoder configuration
* include encode parameters and input/ouput buffer
*/
struct de_JpegEncInfo {
    uint32_t fmt;                  ///< see enum de_DecFrameFmt
    int level;                     ///< see enum de_EncQuality
    uint32_t width;                ///< input image width
    uint32_t height;               ///< input image heigh
    uint32_t xoffset;              ///< x-axis initial pixel to encode
    uint32_t yoffset;              ///< y-axis initial pixel to encode
    uint32_t encode_width;         ///< encode width
    uint32_t encode_height;        ///< encode height
    unsigned long in_paddr;        ///< raw image physical address
    unsigned long out_paddr;       ///< jpeg picture output physical address
    uint8_t *out_vaddr;            ///< jpeg picture output virtual address
    unsigned int osize;            ///< size of encoded image in bytes, 
                                   ///< before encode, it is a estimate value
                                   ///< after encode, it will be override by jpeg actual size
};

/**
* @brief de_JpegEncoderOpen
* Open a jpeg encoder and return a handle
* 
* @return 
*  @retval > 0  jpeg encoder handle
*  @retval < 0  jpeg encoder open failed, return error code, 
*               see enum de_JpegErrorCode

*/
DE_DLL de_JpegEncHandle de_JpegEncoderOpen();


/**
* @brief de_JpegEncode
* Encode a yuv image to jpeg
*
* @param handle    jpeg encoder handle created by de_JpegEncoderOpen()
* @param cfg       jpeg encoder configration, see struct de_JpegEncInfo
*
* @return  see enum de_JpegErrorCode
*/
DE_DLL int de_JpegEncode(de_JpegEncHandle handle, struct de_JpegEncInfo *cfg);


/**
* @brief de_JpegEncoderClose
* Close jpeg encoder and release resource
* 
* @param handle  jpeg encoder handle created by de_JpegEncoderOpen()
*/
DE_DLL void de_JpegEncoderClose(de_JpegEncHandle handle);

#ifdef  __cplusplus
}
#endif

#endif // HAL_MM_JPEG_H_





