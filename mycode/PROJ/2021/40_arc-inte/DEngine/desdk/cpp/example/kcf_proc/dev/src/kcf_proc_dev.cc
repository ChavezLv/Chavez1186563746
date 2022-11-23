#include <iostream>
#include <fstream>
#include "de_c_type.h"
#include "de_cv_api.h"
#include "de_ndarray.h"
#include "de_base_api.h"
#include "de_registry.h"
#include "de_packed_func.h"

int get_image_ndarray(dePixelFormat fmt, int width, int height, std::string pic_path, de::NDArray& image) {

	std::fstream image_file;
	image = de::NDArray::Create(width, height, fmt);
	ExtInfo* info = const_cast<ExtInfo*>(image.GetExtInfo());
	info->roi_rect.x = 0;
	info->roi_rect.y = 0;
	info->roi_rect.w = width;
	info->roi_rect.h = height;

	image_file.open(pic_path, std::ios::binary|std::ios::in);

	if(!image_file) {
		LOG(INFO) << "file " << pic_path << " open fail";
		return -1;
	}

	LOG(INFO) << "image.GetTensorDataSize() " << image.GetTensorDataSize();
	image_file.read(reinterpret_cast<char *>(image.GetTensorData()), image.GetTensorDataSize());
	image_file.close();
	
	return 0;
}

int save_crop_image(de::NDArray image, deRect crop_rect, int crop_index, std::string pic_path_prefix) {

	std::string pic_path;
	std::fstream image_file;

	crop_rect.w = crop_rect.w / 2 * 2;
	crop_rect.h = crop_rect.h / 2 * 2;	

	de::NDArray crop_image = de::cv::Crop(image, crop_rect);

	pic_path = pic_path_prefix + std::to_string(crop_index) + "_" + std::to_string(crop_rect.w) + "x" + std::to_string(crop_rect.h) + ".nv12.yuv";
	image_file.open(pic_path, std::ios::binary|std::ios::out);

	if(!image_file) {
		LOG(INFO) << "file " << pic_path << " open fail";
		return -1;
	}

	image_file.write(reinterpret_cast<char *>(crop_image.GetTensorData()), crop_image.GetTensorDataSize());
	LOG(INFO) << "crop_image.GetTensorDataSize() " << crop_image.GetTensorDataSize();
	image_file.close();

	return 0;
}

int kcf_test() {

	deRect roi_rect = {791, 51, 837, 915};
	de::NDArray image[3];
	std::vector<std::pair<bool, deRect>> boundingBoxes;
	//yuv示例文件位于/DEngine/data/1080p，请将track*文件复制至DP1000侧/root/data/track_test/目录
	std::string yuv_seq_path[3] = {"/root/data/track_test/track0.nv12.yuv", "/root/data/track_test/track1.nv12.yuv", "/root/data/track_test/track2.nv12.yuv"};
	std::string yuv_crop_path_prefix[3] = {"/root/data/track_test/track0_crop", "/root/data/track_test/track1_crop", "/root/data/track_test/track2_crop"};
	
	for(int i = 0; i < 3; i++) {
		get_image_ndarray(DE_PIX_FMT_NV12, 1920, 1080, yuv_seq_path[i], image[i]);
	}

	de::cv::KCFMultiTracker tracker({0, 0, 1920, 1080});
	tracker.Init(image[0], roi_rect);
	save_crop_image(image[0], roi_rect, 0, yuv_crop_path_prefix[0]);

	for(int i = 1; i < 3; i++) {
		boundingBoxes.clear();
		tracker.Update(image[1], boundingBoxes);
		std::cout << "boundingBoxes.size() " << boundingBoxes.size() << std::endl;
		for(int j = 0; j < (int)boundingBoxes.size(); j++) {
			std::cout  << "Tracker: " << j << " " << boundingBoxes[j].first << "  " << boundingBoxes[j].second.x << "  " << boundingBoxes[j].second.y << "  " << boundingBoxes[j].second.w << "  " << boundingBoxes[j].second.h << std::endl;

			save_crop_image(image[i], roi_rect, j, yuv_crop_path_prefix[i]);
		}	
	}

	return 0;
}

DE_REGISTER_GLOBAL("de.example.kcf_process")
.set_body([](de::DEArgs args, de::DERetValue* rv) {
    *rv = kcf_test();
});

void __attribute__((destructor)) x_fini(void)
{
    std::cout<< __PRETTY_FUNCTION__ << "!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    de::Registry::Remove("de.example.kcf_process");
}

