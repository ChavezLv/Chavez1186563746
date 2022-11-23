// #include "media_display.hpp"
#include <string>
#include <iostream>
#include <map>
#include "ws_c_api.h"
#include "media_session.h"
using namespace intellif;
#if TEST_SHOW
// MediaDisplay g_disp_obj;
#endif
std::mutex mtx_session_map;
std::map<int, intellif::MediaSession*> intellif::media_session_map;

void onopen(int id, void* usrdata) {printf("server session id %d.\n", id); mtx_session_map.lock(); media_session_map[id] = new MediaSession(id, E_SERVER); mtx_session_map.unlock();}
void onclose(int id, void* usrdata) {printf("close server session id %d.\n", id); mtx_session_map.lock(); delete media_session_map[id]; media_session_map.erase(id); mtx_session_map.unlock();}
void onmsg(int id, const void* msg, int len, void* usrdata) {
  //mtx_session_map.lock();
  if (media_session_map[id]){
    media_session_map[id]->on_msg(msg, len);
  }
  //mtx_session_map.unlock();
}
void session_loop(int port) {if_ws_server_start(port, onopen, onmsg, onclose);}

// void test_loop(MediaDisplay* disp_obj) {	
void test_loop() {	
	int index = 0;
	char buf[32]={0};
	uint64_t ts = 0;
	while(1){
		if(index >= 200) index = 0;
		sprintf(buf,"track2/%05d.jpg",index++);
		cv::Mat mat = cv::imread(buf);
		cv::namedWindow("1111");
		cv::imshow("1111", mat);
		cv::waitKey(40);
		
		// if_image_t image;
		// image.time_stamp = ts;
		// image.format = IF_IMG_BGR888;
		// image.width = mat.cols;
		// image.height = mat.rows;
		// image.size = mat.cols * mat.rows * 3;
		// image.data = new uint8_t[image.size];
		// memcpy(image.data, mat.data, image.size);
		// image.extra_data[0] = nullptr;
		// image.extra_data[1] = nullptr;
		// SPImage image_ptr(new Image(image));
		// disp_obj->feed_frame(image_ptr);
		// std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

if_log_level_t g_sdk_log_level = IF_LOG_LEVEL_DEBUG;
if_sdk_log_cb_t g_sdk_log_cb;

int main(int argc, char *argv[]) {
	// test_loop();
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int disp_cnt = atoi(argv[3]);
	int port = 9002;
	if (argc > 4) port = atoi(argv[4]);

	MediaDisplay::config("test", width, height, disp_cnt);

	#if TEST_SHOW
	test_loop();
	#else
	std::thread session_thread(std::bind(&session_loop, port));
	session_thread.join();
	#endif
		
	sleep(-1);
}
