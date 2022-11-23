#include"configuration.hpp"
#include"myserver.hpp"
#include"dictProducer.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using std::string;
void test1(){
	rxl::Configuration conf("../../conf/config_online");
	rxl::MyServer server(conf); 
	server.start();
}
void test2(){
	rxl::Configuration conf("../../conf/config_online");
	rxl::DictProducer::getInstance(conf);
}
int main(void)
{
	test1();
	return 0;
}

