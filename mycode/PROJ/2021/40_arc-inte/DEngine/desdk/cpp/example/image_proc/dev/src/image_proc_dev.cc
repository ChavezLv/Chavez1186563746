#include <iostream>
#include <fstream>
#include "de_c_type.h"
#include "de_cv_api.h"
#include "de_ndarray.h"
#include "de_base_api.h"
#include "de_registry.h"
#include "de_packed_func.h"

de::NDArray get_empty_image_ndarray(dePixelFormat fmt, int width, int height) {

	de::NDArray image = de::NDArray::Create(width, height, fmt);
	ExtInfo* info = const_cast<ExtInfo*>(image.GetExtInfo());
	info->roi_rect.x = 0;
	info->roi_rect.y = 0;
	info->roi_rect.w = width;
	info->roi_rect.h = height;
	
	return image;
}

int copy_file(std::string src_pic_path, std::string pout) {
	int file_size = 0;
	std::fstream src_file;
	std::fstream dst_file;

	src_file.open(src_pic_path, std::ios::binary|std::ios::in|std::ios::ate);
	dst_file.open(pout, std::ios::binary|std::ios::out);

	if(!src_file) {
		LOG(INFO) << "src_file " << src_pic_path << " open fail";
		return -1;
	}

	if(!dst_file) {
		LOG(INFO) << "dst_file " << pout << " open fail";
		return -1;
	}

	file_size = src_file.tellg();
	char* imgData = new char[file_size];

	if(imgData == nullptr) {
		LOG(INFO) << "imgData == nullptr";
		return -1;
	}

	src_file.seekg(0);
	src_file.read(imgData, file_size);
	dst_file.write(imgData, file_size);

	src_file.close();
	dst_file.close();

	delete(imgData);
	imgData = nullptr;

	return 0;
}

int image_resize(de::NDArray& src_image, de::NDArray& dst_image, dePixelFormat fmt, int dst_width, int dst_height) {
	
	deSize resize_resolution;
	LOG(INFO) << "image_resize " << dst_width << " x " << dst_height;

	if(fmt != DE_PIX_FMT_RGB888 && fmt != DE_PIX_FMT_BGR888 && fmt != DE_PIX_FMT_RGB888_PLANE && fmt != DE_PIX_FMT_BGR888_PLANE) {
		LOG(INFO) << "No support format " << fmt;
		return -1;
	}

	resize_resolution.width = dst_width;
	resize_resolution.height = dst_height;
	return de::cv::Resize(src_image, dst_image, resize_resolution, de::cv::kNormScale);
}

int image_convert(de::NDArray& src_image, dePixelFormat src_fmt, de::NDArray& dst_image, dePixelFormat dst_fmt) {

	int cvtCode = 0;

	LOG(INFO) << "image_convert " << src_fmt << " -> " << dst_fmt;
	if(src_fmt == DE_PIX_FMT_NV12 && dst_fmt == DE_PIX_FMT_RGB555) {
		cvtCode = de::cv::kCvtYuvNv12Rgb; 				//YUV420NV12=>RGB
	} else if(src_fmt == DE_PIX_FMT_YUV422_UYVY && dst_fmt == DE_PIX_FMT_RGB555) {
		cvtCode = de::cv::kCvtYuvUyvy2Rgb; 				//YUV422UYVY=>RGB
	} else if(src_fmt == DE_PIX_FMT_NV21 && dst_fmt == DE_PIX_FMT_RGB555) {
		cvtCode = de::cv::kCvtYuvNv21Rgb; 				//YUV420NV21=>RGB
	}  else if(src_fmt == DE_PIX_FMT_YUV422_YUYV && dst_fmt == DE_PIX_FMT_RGB555) {
		cvtCode = de::cv::kCvtYuvYuyv2Rgb; 				//YUV422YUYV=>RGB
	}  else if(src_fmt == DE_PIX_FMT_NV16 && dst_fmt == DE_PIX_FMT_RGB555) {
		cvtCode = de::cv::kCvtYuvNv16Rgb; 				//YUV422NV16=>RGB
	}  else if(src_fmt == DE_PIX_FMT_NV16 && dst_fmt == DE_PIX_FMT_YUV422_UYVY) {
		cvtCode = de::cv::kCvtYuvNv16YuvUYVY; 			//YUV422NV16=>YUV422UYVY
	} else if(src_fmt == DE_PIX_FMT_NV16 && dst_fmt == DE_PIX_FMT_YUV422_YUYV) {
		cvtCode = de::cv::kCvtYuvNv16YuvYUYV;			//YUV422NV16=>YUV422YUYV
	} else if(src_fmt == DE_PIX_FMT_NV12 && dst_fmt == DE_PIX_FMT_YUV422_UYVY) {
		cvtCode = de::cv::kCvtYuvNv12YuvUYVY;			//YUV420NV12=>YUV422UYVY
	} else if(src_fmt == DE_PIX_FMT_NV12 && dst_fmt == DE_PIX_FMT_YUV422_YUYV) {
		cvtCode = de::cv::kCvtYuvNv12YuvYUYV;			//YUV420NV12=>YUV422YUYV
	} else if(src_fmt == DE_PIX_FMT_NV21 && dst_fmt == DE_PIX_FMT_YUV422_YUYV) {
		cvtCode = de::cv::kCvtYuvNv21YuvYUYV;			//YUV420NV21=>YUV422YUYV
	} else if(src_fmt == DE_PIX_FMT_NV21 && dst_fmt == DE_PIX_FMT_YUV422_UYVY) {
		cvtCode = de::cv::kCvtYuvNv21YuvUYVY;			//YUV420NV21=>YUV422UYVY
	} else if(src_fmt == DE_PIX_FMT_YUV422_UYVY && dst_fmt == DE_PIX_FMT_NV12) {
		cvtCode = de::cv::kCvtYuvUYVYYuvNv12;			//YUV422UYVY=>YUV420NV12
	} else if(src_fmt == DE_PIX_FMT_YUV422_UYVY && dst_fmt == DE_PIX_FMT_NV21) {
		cvtCode = de::cv::kCvtYuvUYVYYuvNv21;			//YUV422UYVY=>YUV420NV21
	} else if(src_fmt == DE_PIX_FMT_YUV422_YUYV && dst_fmt == DE_PIX_FMT_NV12) {
		cvtCode = de::cv::kCvtYuvYUYVYuvNv12;			//YUV422YUYV=>YUV420NV12
	} else if(src_fmt == DE_PIX_FMT_YUV422_YUYV && dst_fmt == DE_PIX_FMT_NV21) {
		cvtCode = de::cv::kCvtYuvYUYVYuvNv21;			//YUV422YUYV=>YUV420NV21
	} else if(src_fmt == DE_PIX_FMT_RGB555 && dst_fmt == DE_PIX_FMT_NV12) {
		cvtCode = de::cv::kCvtRgbYuvNv12;				//RGB=>YUV420NV12
	} else if(src_fmt == DE_PIX_FMT_RGB555 && dst_fmt == DE_PIX_FMT_YUV422_UYVY) {
		cvtCode = de::cv::kCvtRgbYuvUYVY;				//RGB=>YUV422UYVY
	} else if(src_fmt == DE_PIX_FMT_NV12 && dst_fmt == DE_PIX_FMT_RGB888) {
		cvtCode = de::cv::kCvtYuvNv12Rgb888;			//YUV420NV12=>RGB888
	} else if(src_fmt == DE_PIX_FMT_YUV422_UYVY && dst_fmt == DE_PIX_FMT_RGB888) {
		cvtCode = de::cv::kCvtYuvUyvy2Rgb888;			//YUV422UYVY=>RGB888
	} else if(src_fmt == DE_PIX_FMT_NV21 && dst_fmt == DE_PIX_FMT_RGB888) {
		cvtCode = de::cv::kCvtYuvNv21Rgb888;			//YUV420NV21=>RGB888
	} else if(src_fmt == DE_PIX_FMT_YUV422_YUYV && dst_fmt == DE_PIX_FMT_RGB888) {
		cvtCode = de::cv::kCvtYuvYuyv2Rgb888;			//YUV422YUYV=>RGB888
	} else {
		LOG(INFO) << "convert format not support " << src_fmt << " to " << dst_fmt;
		return -1;
	}

	LOG(INFO) << "cvtCode " << cvtCode;
	return de::cv::CvtColor(src_image, dst_image, cvtCode);
}

int image_process(std::string src_pic_path, std::string pout, 
					dePixelFormat src_fmt, int src_width, int src_height,  
                    dePixelFormat dst_fmt = DE_PIX_FMT_NONE, int dst_width = 0, int dst_height = 0, 
					deRect crop_rect = {0, 0, 0, 0}) {
	int ret = 0;
	std::fstream src_file;
	std::fstream dst_file;

	de::NDArray src_image;
	de::NDArray dst_image;
	de::NDArray dst_image2;
	de::NDArray dst_image3;

    if(dst_fmt == DE_PIX_FMT_NONE) {
        dst_fmt = src_fmt;
    }
    if(dst_width == 0) {
        dst_width = src_width;
    }
    if(dst_height == 0) {
        dst_height = src_height;
    }

	if(src_width == dst_width && src_height == dst_height && src_fmt == dst_fmt && crop_rect.w == 0 && crop_rect.h == 0) {
		if(src_pic_path == pout) {
			return 0;
		}
		if(copy_file(src_pic_path, pout) != 0) {
			return -1;
		}
		return 0;
	}

	src_file.open(src_pic_path, std::ios::binary|std::ios::in);
	dst_file.open(pout, std::ios::binary|std::ios::out);

	if(!src_file) {
		LOG(INFO) << "src_file " << src_pic_path << " open fail";
		return -1;
	}

	if(!dst_file) {
		LOG(INFO) << "dst_file " << pout << " open fail";
		return -1;
	}

	src_image = get_empty_image_ndarray(src_fmt, src_width, src_height);
	src_file.read(reinterpret_cast<char *>(src_image.GetTensorData()), src_image.GetTensorDataSize());

	if(src_width != dst_width || src_height != dst_height) {
		ret = image_resize(src_image, dst_image, src_fmt, dst_width, dst_height);
		LOG(INFO) << " image_resize ret " << ret;
	} else {
		dst_image = src_image;
	}

	if(src_fmt != dst_fmt) {
		ret = image_convert(dst_image, src_fmt, dst_image2, dst_fmt);
		LOG(INFO) << "image_convert ret " << ret;
	} else {
		dst_image2 = dst_image;
	}

	if(crop_rect.w != 0 && crop_rect.h != 0) {
		crop_rect.w = crop_rect.w / 2 * 2;
		crop_rect.h = crop_rect.h / 2 * 2;
		LOG(INFO) << "crop_rect " << crop_rect.x << ", " << crop_rect.y << ", " << crop_rect.w << ", " << crop_rect.h;
		dst_image3 = de::cv::Crop(dst_image2, crop_rect);	

	} else {
		dst_image3 = dst_image2;
	}

	dst_file.write(reinterpret_cast<char *>(dst_image3.GetTensorData()), dst_image3.GetTensorDataSize());
	LOG(INFO) << "dst_image3.GetTensorDataSize() " << dst_image3.GetTensorDataSize();
	src_file.close();
	dst_file.close();

    return 0;
}

DE_REGISTER_GLOBAL("de.example.image_process")
.set_body([](de::DEArgs args, de::DERetValue* rv) {
    int temp = 0;
	std::string src_pic_path = args[0];
	std::string pout = args[1];
	temp = args[2];
	dePixelFormat src_fmt = (dePixelFormat)temp;
	int src_width = args[3];
	int src_height = args[4];
	temp = args[5];
	dePixelFormat dst_fmt = (dePixelFormat)temp;
	int dst_width = args[6];
	int dst_height = args[7];
	deRect crop_rect = {args[8], args[9], args[10], args[11]};

    *rv = image_process(src_pic_path, pout, src_fmt, src_width, src_height,  
                    dst_fmt, dst_width, dst_height, crop_rect);

});


void __attribute__((destructor)) x_fini(void)
{
    std::cout<< __PRETTY_FUNCTION__ << "!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    de::Registry::Remove("de.example.image_process");
}

