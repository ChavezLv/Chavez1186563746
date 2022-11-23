/**
 * @file de_cv_api.h
 * @brief cv功能用户接口
 * @details  提供cv图像处理类功能接口
 * @author DesdkTeam
 * @version 1.0.0
 * @date 2020/03/18
 * @copyright Intellif
 */
#ifndef DE_CV_API_H
#define DE_CV_API_H

#include <core/de_c_type.h>
#include <vector>

namespace de{

class NDArray;

namespace cv{

class CvEngine;

   /**@brief color space convert code
	*/
	typedef enum{
		kCvtYuvNv12Rgb = 0,		///< YUV420NV12=>RGB
		kCvtYuvUyvy2Rgb=1,		///< YUV422UYVY=>RGB
		kCvtYuvNv21Rgb=2,		    ///< YUV420NV21=>RGB
		kCvtYuvYuyv2Rgb=3,		///< YUV422YUYV=>RGB
		kCvtYuvNv16Rgb=4,			///< YUV422NV16=>RGB		
		kCvtYuvNv16YuvUYVY=5,		///< YUV422NV16=>YUV422UYVY
		kCvtYuvNv16YuvYUYV=6,		///< YUV422NV16=>YUV422YUYV
		kCvtYuvNv12YuvUYVY=7,		///< YUV420NV12=>YUV422UYVY
		kCvtYuvNv12YuvYUYV=8,		///< YUV420NV12=>YUV422YUYV
		kCvtYuvNv21YuvYUYV=9,     ///< YUV420NV21=>YUV422YUYV
		kCvtYuvNv21YuvUYVY=10,     ///< YUV420NV21=>YUV422UYVY
		kCvtYuvUYVYYuvNv12=11,		///< YUV422UYVY=>YUV420NV12
		kCvtYuvUYVYYuvNv21=12,     ///< YUV422UYVY=>YUV420NV21
		kCvtYuvYUYVYuvNv12=13,     ///< YUV422YUYV=>YUV420NV12
		kCvtYuvYUYVYuvNv21=14,     ///< YUV422YUYV=>YUV420NV21
		kCvtRgbYuvNv12=15,         ///< RGB=>YUV420NV12
		kCvtRgbYuvUYVY=16,         ///< RGB=>YUV422UYVY	
		kCvtYuvNv12Rgb888=17,     	///< YUV420NV12=>RGB888
		kCvtYuvUyvy2Rgb888=18,		///< YUV422UYVY=>RGB888, width must be even
		kCvtYuvNv21Rgb888=19,		///< YUV420NV21=>RGB888
		kCvtYuvYuyv2Rgb888=20,		///< YUV422YUYV=>RGB888, width must be even
		kCvtRgbYuvNv21=21,         ///< RGB=>YUV420NV21
		kCvtRgbYuvYUYV=22,         ///< RGB=>YUV422YUYV
		kCvtYuvNv12Bgr=23,
		kCvtYuvUyvy2Bgr=24,
		kCvtYuvNv21Bgr=25,
		kCvtYuvYuyv2Bgr=26,
		kCvtYuvNv16YuvNv12=27,		///< YUV422NV16=>YUV420Nv12
		kCvtYuvNv16YuvNv21=28,		///< YUV422NV16=>YUV420Nv21
		kCvtYuvNv12Bgr888=29,     	///< YUV420NV12=>RGB888
		kCvtYuvUyvy2Bgr888=30,		///< YUV422UYVY=>RGB888, width must be even
		kCvtYuvNv21Bgr888=31,		///< YUV420NV21=>RGB888
		kCvtYuvYuyv2Bgr888=32,		///< YUV422YUYV=>RGB888, width must be even
	}CvtColorCode;


   /**@brief cv旋转类型
	*/
	enum RotationType {
		kRotation_90 = 0,		///< 顺时针旋转90度
		kRotation_180,		///< 顺时针旋转180度
		kRotation_270,		///< 顺时针旋转270度
		kRotation_angle     ///< 
	};


/**@brief cv翻转类型
*/
enum FlipType {
  kImageFlipVert = 0, ///< 垂直翻转
  kImageFlipHorz,   ///< 水平翻转
  kImageFlipVertHorz, ///< 垂直+水平翻转
};
  
/**@brief 人脸姿态定义
*/
struct FacePose{
  float pitch;      ///< pitch
  float yaw;        ///< yaw
  float roll;       ///< roll
};

/**@brief cv类浮点矩形框
*/
struct deRectf {
  float x;        ///< 左上顶点x坐标
  float y;        ///< 左上顶点y坐标
  float w;        ///< 矩形框宽
  float h;        ///< 矩形框高
};    

/**@brief cv缩放类型
*/
enum ResizeType {
  kNormal,      ///< 普通类型
  kNormScale,         ///< 等比例缩放
};

struct Peak {	
    int id;	
    int x;	
    int y;	
    float score;
};

enum EdgeDetectionType {
	kHorizontal = 0,
	kVertical
};

enum PaddingType {
	kBorderConstant,
	kBorderReplicate,
	kBorderReflect101,
	kBorderReflect
};

struct PerspectiveMatrix {
	float a11;
	float a12;
	float a13;
	float a21;
	float a22;
	float a23;
	float a31;
	float a32;
	float a33;
};

/**@brief cv 仿射变换矩阵
		*/
struct AffineMatrix {
	float a11;
	float a12;
	float a21;
	float a22;
	float xt;
	float yt;
};

#ifndef __XTENSA__
	
	/*@brief Convert an image from one color space to another 
	* @param[in]				src				input image
	* @param[out]				dst				output image 
	* @param[in]				cvtCode			convert code,refer to CvtColorCode		
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int CvtColor	(de::NDArray& src,de::NDArray& dst,int cvtCode);

	
	/**@brief Resize an image
	* @param[in]				src				input image
	* @param[out]				dst				output image
	* @parma[in]				sz				image size after operation	
	* @parma[in]				type		    image size type, details in ResizeType	
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Resize(de::NDArray& src,de::NDArray& dst, deSize sz,int type);
	
	/**@brief Convert an image from one color space to another AND resize to another size
	* @param[in]				src				input image
	* @param[out]				dst				output image
	* @param[in]				code			convert code,refer to CvtColorCode		
	* @param[in]				sz				image size after operation
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int CvtResize	(de::NDArray& src,de::NDArray& dst,int cvtCode, deSize sz);


	/**@brief Rotate an image of color sapce nv16 in 15 degree AND resize to another size
	* @param[in]				src				input image
	* @param[out]				dst				output image
	* @param[in]				cvtCode			convert code,refer to CvtColorCode
	* @param[in]				type			rotate type details in RotationType
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int CvtRotation	(de::NDArray& src, de::NDArray& dst, int cvtCode, int type);


	/**@brief Flip an image around vertical,horizonal,or both axes
	* @param[in]				src				input image
	* @param[out]				dst				output image
	* @param[in]				flipCode		flip code,refer to FlipType
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Flip(de::NDArray& src,de::NDArray& dst,int flipCode);

	/**@brief Rotate an image in multiples of 90 degree
	* @param[in]				src				input image
	* @param[out]				dst				output image
	* @param[in]				rotateCode 		rotate code,refer to RotationType
	* @param[in]				angle			only for rotateCode kRotation_angle!!
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Rotate(de::NDArray& src,de::NDArray& dst,int rotateCode);
	DE_DLL int RotateAngle(de::NDArray& src,de::NDArray& dst,float angle);

	/**@brief copy memory, from src to dst, using dsp idma
	* @param[in]				src				input image
	* @param[out]				dst				output image
	* @return					
	* @note:   only support 2D cpy!!!!!
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Copy2D(de::NDArray& src, de::NDArray& dst, 			
			int cpy_rows, int cpy_bytes, int dst_width, int dst_height,
			int src_x_off, int src_y_off, int dst_x_off, int dst_y_off);

	/**@brief copy memory, from src to dst, using boxdma
	* @param[in]				src				input image
	* @param[out]				dst				output image
	* @return					
	* @note:   only support 2D cpy!!!!!
	*							- 0:  success
	*							- -1: fail
	* @note acc width & height must be less than 2^15!!!!!!!
	*/	
	DE_DLL int Copy2DAcc(de::NDArray& src, de::NDArray& dst,
			int cpy_rows, int cpy_bytes, int dst_width, int dst_height,
			int src_x_off, int src_y_off, int dst_x_off, int dst_y_off  );

	/**@brief crop an image, copy roi of src image to dst
	* @param[in]				src				src image
	* @param[out]				dst				dst image
	* @param[in]				type		    edge detection type
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int EdgeDetection(de::NDArray& src, de::NDArray& dst, int type);

	/**@brief crop an image, copy roi of src image to dst
	* @param[in]				src				src image
	* @param[out]				dst				dst image
	* @param[in]				ksize		    kernel size, support 3 and 5
	* @param[in]				pad_type	    padding type  see PaddingType
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int GaussianBlur(de::NDArray& src, de::NDArray& dst, int ksize = 5, PaddingType pad_type = kBorderConstant);


	/**@brief find up & low edge
	* @param[in]				src				src image
	* @param[out_data]			dst				out_data[0] up; out_data[1] low
	* @param[x_start]			src				width start
	* @param[x_end]			    src				width end
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int EdgeRow(de::NDArray& src, int* out_data, int x_start, int x_end);

	/**@brief slid window
	* @param[in]				src				src image
	* @param[out_data]			dst			    window val
	* @param[window_width]		src				window len
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int SlidWindow(de::NDArray& src, int* out_data, int window_width);

	/**@brief exp an image
	* @param[in]				src				input data
	* @param[out]				dst				output data
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int NpExp(de::NDArray& src,de::NDArray& dst);

	/**@brief np where only support 1D
	* @param[in]				src				input data
	* @param[out]				vals			output values
	* @param[out]				index			output index
	* @param[out]				count			output count
	* @param[in]				threshold	    threshold val
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int NpWhere(de::NDArray& src,de::NDArray& vals, de::NDArray& index, int* count, float threshold);;

	/**@brief np hstack
	* @param[indata0]			src0		input data0
	* @param[indata1]			src1		input data1
	* @param[outdata]			dst			output data
	* @param[dim]				dim			dim info, the lowest is the max
	* @param[dimnum]			dimnum	    dim num
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Hstack(de::NDArray& indata0, de::NDArray& indata1, de::NDArray& outdata, int* dim, int dimnum);

	/**@brief np transpose
	* @param[in_data]				src				input data
	* @param[out_data]				dst			    output data
	* @param[width]				    width			org width
	* @param[height]				height			org height
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Transpose(de::NDArray& in_data, de::NDArray& out_data, int width, int height);

	/**@brief np maxinum
	* @param[in_data0]			src0			input data0
	* @param[in_data1]			src1			input data1
	* @param[out_data]			dst			    output data
	* @param[size0]				src0 len		src0 len
	* @param[size1]				src1 len	    src1 len
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Maxinum(de::NDArray& in_data0, de::NDArray& in_data1, de::NDArray& out_data, int size0, int size1);

	/**@brief select top n data
	* @param[in]				src				input data
	* @param[out]				vals			output values
	* @param[out]				index			output index
	* @param[in]				n  			    output count
	* @param[in]				threshold	    threshold val
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int TopNSelect(de::NDArray& src,de::NDArray& out_vals, de::NDArray& index, int n, int batch_num, int threshold);

	/**@brief vector multiply 
	* @param[in]				src0		    input data0
	* @param[in]				src1			input data1
	* @param[out]				dst 			output values
	* @param[in]				n  			    vector's dim
	* @param[in]				num0	        vector number of data0
	* @param[in]				num1	        vector number of data1
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int VectorMultiply(de::NDArray& src0, de::NDArray& src1, de::NDArray& dst, int n, int num0, int num1);

	/**@brief vector multiply 
	* @param[in]				src 		    input fe
	* @param[in]				ref			    input fe db
	* @param[out]				dst 			output fe
	* @param[out]				index			output index
	* @param[in]				fe_num  	    vector's dim
	* @param[in]				n  			    top n fe
	* @param[in]				src_num	        vector number of input fe
	* @param[in]				ref_num	        vector number of ref fe
	* @param[in]				threshold	    threshold val
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int FeCompare(de::NDArray& src, de::NDArray& ref, de::NDArray& dst, de::NDArray& index,	
		int fe_num, int n, int src_num, int ref_num, int threshold);

	/**@brief vector multiply 
	* @param[in]				src 		    input image
	* @param[in]				rois			the rois to calc luma
	* @param[out]				lumas 			output luma
	* @return					
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Luma(std::vector<de::NDArray>& src, std::vector<std::vector<deRect>>& rois, std::vector<std::vector<uint8_t>>& lumas);
	DE_DLL int Luma(de::NDArray& src, std::vector<deRect>& rois, std::vector<uint8_t>& lumas);

	/**@brief crop an image, copy roi of src image to dst
	* @param[in]				src				src image
	* @param[out]				dst				dst image
	* @param[in]				roi				rect of intrest 最大32个
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL de::NDArray Crop(const de::NDArray& src, const deRect &roi);
	DE_DLL int Crop(const de::NDArray& src, std::vector<de::NDArray>& dsts, const std::vector<deRect>& rois);

	/**@brief crop an image, copy roi of src image to dst
	* @param[in]				nd				    src image
	* @param[in]				rois				rois
	* @param[out]				dsts				dst images
	* @param[in]				ksize			    Aperture size used to compute the second-derivative filters.
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Laplacian(const de::NDArray& nd, std::vector<deRect> rois, std::vector<de::NDArray>& dsts, int ksize = 1, bool normalize = false);

	/**@brief Range, return roi of a ndarray, [row_start:row_end),[col_start:col_end)
	* @param[in]				nd				    src image
	* @param[in]				row_start		    start row index
	* @param[in]				row_end		        end row index
	* @param[in]				col_start		    start col index
	* @param[in]				col_end		        end col index	
	* @return                   ndarray of the range
	*							
	*/
	DE_DLL de::NDArray Range(de::NDArray& nd, int row_start, int row_end, int col_start, int col_end);

	/**@brief BlurDetect,      blur detect: laplacian + meanstddev
	* @param[in]				nd				    src image
	* @param[in]				rois		        rois
	* @param[out]				means		        means
	* @param[out]				stddevs		        stddevs
	* @return                   - 0:  success
	*							- -1: fail
	*							
	*/
	DE_DLL int BlurDetect(const de::NDArray& nd, std::vector<deRect>& rois, std::vector<float>& means, std::vector<float>& stddevs);

	/**@brief crop an image, copy roi of src image to dst
	* @param[in]				srcs				src images
	* @param[out]				means				dst mean
	* @param[out]				stddevs				dst stddev
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int MeanStddev(std::vector<de::NDArray>& srcs, std::vector<float>& means, std::vector<float>& stddevs);

	/**@brief affine of an image
	* @param[in]				src 				src image
	* @param[in]				matrix				affine matrix
	* @param[out]				dst				    dst image
	* @param[in]				pad_type		    0: const pad 0  1: replicate pad
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int WarpAffine(de::NDArray& src, std::vector<float>& matrix, de::NDArray& dst, int width_out, int height_out, int pad_type);

	/**@brief affine of an image
	* @param[in]				heatMaps 			src pointers
	* @param[in]				heatMapsCols		cols
	* @param[in]				heatMapsRows	    rows
	* @param[in]				minPeaksDistance	distance threshold
	* @param[out]				allPeaks	        all out peaks
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int FindPeaks(const std::vector<de::NDArray>& heat_maps, const std::vector<int>& heat_map_cols, 
		const std::vector<int>& heat_map_rows, const float min_peak_distance, std::vector<std::vector<Peak>>& all_peaks);

	/**@brief padding  padding模式为Reflect101时，padding的大小需要限制在64以内，为Replicate时，padding大小需要限制在128以内
	* @param[in]				src 				src image
	* @param[out]				dst				    dst image
	* @param[in]				top 				padding top
	* @param[in]				bottom 				padding bottom
	* @param[in]				left 				padding left
	* @param[in]				right 				padding right
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int Padding(de::NDArray& src, de::NDArray& dst, int top, int bottom, int left, int right, PaddingType type, int pad_val = 0);
	
	/**@brief GetPerspectiveTransform  获取透视变换矩阵
	* @param[in]				src_pts 			src points
	* @param[in]				dst_pts				dst image
	* @param[out]				pptm 				perspective matrix
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int GetPerspectiveTransform(std::vector<std::vector<int>>& src_pts, std::vector<std::vector<int>>& dst_pts, PerspectiveMatrix& pptm);

	/**@brief perspective of an image
	* @param[in]				src 				src image
	* @param[in]				matrix				affine matrix
	* @param[out]				dst				    dst image
	* @param[in]				pad_type		    0: const pad 0  1: replicate pad
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int WarpPerspective(de::NDArray& src, std::vector<float>& matrix, de::NDArray& dst, int width_out, int height_out, int pad_type);

	/**@brief GetPerspectiveTransform  获取放射变换矩阵
	* @param[in]				src_pts 			src points
	* @param[in]				dst_pts				dst image
	* @param[out]				pptm 				affine matrix
	* @return
	*							- 0:  success
	*							- -1: fail
	*/
	DE_DLL int GetAffineTransform(std::vector<std::vector<int>>& src_pts, std::vector<std::vector<int>>& dst_pts, AffineMatrix& ffm);


#endif //__XTENSA__

/** @brief KCF 多目标跟踪
   @code：
   ImagedeRect_t roi(0,0,1000,1000);
   ImageInfo_t imgs[128];
   
   step1 : 构造Tracker对象
   KCFMultiTracker tracker(roi);

   step2:  初始化增加a,b跟踪物体
   ImagedeRect_t obj_a(10,20, 48, 48);
   ImagedeRect_t obj_b(60,100,48, 48);
   tracker.Init(imgs[0], {obj_a, obj_c});
   
   step3: 跟踪物体a和b, 获取跟踪结果
   std::vector< std::pair<bool, ImagedeRect_t >> boundingBoxes;
   tracker.Update(imgs[1]);
   tracker.Update(imgs[2], boundingBoxes);

   step4： 增加物体c跟踪（当前帧必须先update，然后才允许add新物体）
   ImagedeRect_t obj_c(10,20, 48, 48);
   tracker.Add(imgs[2], {obj_c, });

   step5: 跟踪物体a和b,c,  获取跟踪结果
   tracker.Update(imgs[3], boundingBoxes);

   step6: 获取a,b,c跟踪结果处理，重新跟踪a,c(假设丢失b)
   obj_a = adjust(obj_a);
   obj_c = adjust(obj_c);
   tracker.Init(imgs[3], {obj_a, obj_c});
  */
class DE_DLL KCFMultiTracker {
 public:
  /**
    * @brief 构建KCFTracker
    * @param roi 跟踪的ROI区域，如果跟踪目标超过ROI，那么认为丢失
    * @param max_track_num 最大跟踪目标数目
    *
    */
  //KCFMultiTracker(const ImagedeRect_t roi, const int max_track_num = 40);
  KCFMultiTracker(const deRect roi, const int max_track_num = 40, bool small_scale = false);
  KCFMultiTracker(const int max_track_num = 40, bool small_scale = false);
  ~KCFMultiTracker(); 
  /**
  * @brief 清除历史跟踪信息，初始化单个跟踪目标
  * @param image 图像
  * @param bounding_box 目标框
  *
  */
  //void Init(ImageInfo_t* image, const  ImagedeRect_t bounding_box);
  void Init(NDArray image, const  deRect& bounding_box);

  /**
  * @brief 清除历史跟踪信息，初始化多个跟踪目标
  * @param image 图像
  * @param bounding_boxs 多目标框
  *
  */
  //void Init(ImageInfo_t* image, const  std::vector< ImagedeRect_t > bounding_boxs);
  void Init(NDArray image, const  std::vector<deRect> bounding_boxs);

  /**
  * @brief 增加单个跟踪目标
  * @param image 图像
  * @param bounding_box 目标框
  * @note 当前帧先进行update操作，然后才允许add操作
  *
  */
  //void Add(ImageInfo_t* image, const  ImagedeRect_t bounding_box);
  void Add(NDArray image, const  deRect& bounding_box);

  /**
  * @brief 增加多个跟踪目标
  * @param image 图像
  * @param bounding_boxs 多个目标框
  * @note 当前帧先进行update操作，然后才允许add操作 
  */
  //void Add(ImageInfo_t* image, const  std::vector< ImagedeRect_t > bounding_boxs);  
  void Add(NDArray image, const std::vector<deRect> bounding_boxs);

  /**
  * @brief 更新跟踪目标
  * @param image 图像
  * @note The result will be saved in the internal storage.
  */
  // Update the current tracking status.The result will be saved in the internal storage.
  //bool Update(ImageInfo_t* image);  
  bool Update(NDArray image);

	/**
	* @brief 更新跟踪目标
	* @param image 图像
	* @param bounding_boxs 新的跟踪目标信息,  
		 first : true - 目标存在， false - 目标丢失
		 second ：跟踪目标的新位置（如果目标丢失，保持最近的跟踪结果）
	*/
	//bool Update(ImageInfo_t* image, 
	//	std::vector< std::pair<bool, ImageRect_t >>& bounding_boxs);	
	bool Update(NDArray image, 
		std::vector< std::pair<bool, deRect >>& bounding_boxs);

  /**
  * @brief 设置最大跟踪目标数
  * @param max_num 最大目标数
  * @note 
  */
  void SetTrackerMaxNum(int max_num);

  /**
  * @brief 设置跟踪ROI
  * @param rect 跟踪ROI，跟踪只在此ROI内进行
  * @note 
  */
  void SetTrackRoi(deRect& rect);

  private:
    void* kcf_impl_ = nullptr;
    #ifdef __CSKY__
    CvEngine* init_engine_ = nullptr;
    CvEngine* update_engine_ = nullptr;
    #endif
};
  
}
}

#endif


