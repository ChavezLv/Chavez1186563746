#include <iostream>
#include <fstream>
#include "de_api.h"
#include "de_cv_api.h"
#include "de_ndarray.h"
#include "de_base_api.h"
#include "de_packed_func.h"

int image_process(std::string src_pic_path, std::string pout, 
					dePixelFormat src_fmt, int src_width, int src_height,  
                    dePixelFormat dst_fmt = DE_PIX_FMT_NONE, int dst_width = 0, int dst_height = 0, 
					deRect crop_rect = {0, 0, 0, 0}) {

	de::Device::GetFunc(0, "de.example.image_process")(src_pic_path, pout, 
														(int)src_fmt, src_width, src_height, 
														(int)dst_fmt, dst_width, dst_height, 
														crop_rect.x, crop_rect.y, crop_rect.w, crop_rect.h);					

	return 0;
}

void* load_library() {
	void* handle = de::Device::LoadLib(0,"host:/DEngine/desdk/cpp/example/image_proc/dev/lib/libimage_proc.so");
	return handle;
}

int unload_library(void* handle) {
	de::Device::UnLoadLibHandle(0,handle);
}

int main(int argc, char** argv) {
	std::string url="192.168.33.86";
  int port= 9200;
	if (argc >= 3) {
    url = argv[1];
    port = atoi(argv[2]);
		de::Device::Init(0, url, port);
	}
	
	void* handle = load_library();

	//1080p.yuv位于/DEngine/data/1080p，请将此文件复制至DP1000侧/root/data/image_test/目录
	//convert from NV12 to RGB888
	image_process("/root/data/image_test/1080p.yuv", "/root/data/image_test/test_1080p_888.rgb", DE_PIX_FMT_NV12, 1920, 1080, DE_PIX_FMT_RGB888);
	//resize from 1080P to 720P
	image_process("/root/data/image_test/test_1080p_888.rgb", "/root/data/image_test/test_720p_888.rgb", DE_PIX_FMT_RGB888, 1920, 1080, DE_PIX_FMT_RGB888, 1280, 720);
	//resize from 1080P to CIF
	image_process("/root/data/image_test/test_1080p_888.rgb", "/root/data/image_test/test_cif_888.rgb", DE_PIX_FMT_RGB888, 1920, 1080, DE_PIX_FMT_RGB888, 352, 288);
	//crop NV12
	image_process("/root/data/image_test/1080p.yuv", "/root/data/image_test/test_720p_crop.nv12.yuv", DE_PIX_FMT_NV12, 1920, 1080, DE_PIX_FMT_NV12, 1920, 1080, {640, 360, 1280, 720});
	//resize from 1080P to 720P and crop RGB888
	image_process("/root/data/image_test/test_1080p_888.rgb", "/root/data/image_test/test_640x360_crop.rgb", DE_PIX_FMT_RGB888, 1920, 1080, DE_PIX_FMT_RGB888, 1280, 720, {640, 360, 640, 360});

	unload_library(handle);

	return 0;
}