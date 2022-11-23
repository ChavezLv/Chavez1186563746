#include <iostream>
#include <fstream>
#include "de_c_type.h"
#include "de_cv_api.h"
#include "de_ndarray.h"
#include "de_base_api.h"
#include "de_packed_func.h"

int kcf_test() {
	de::Device::GetFunc(0, "de.example.kcf_process")();					
	return 0;
}

void* load_library() {
	void* handle = de::Device::LoadLib(0,"host:/DEngine/desdk/cpp/example/kcf_proc/dev/lib/libkcf_proc.so");
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
	kcf_test();
	unload_library(handle);

	return 0;
}