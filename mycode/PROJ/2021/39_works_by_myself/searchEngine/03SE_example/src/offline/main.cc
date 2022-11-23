#include"configuration.hpp"
#include"dictProducer.hpp"
#include"splitTool.hpp"
int main(){
	rxl::Configuration conf("../../conf/config");
	rxl::SplitTool tool(conf);
	rxl::DictProducer::getInstance(conf,tool);
}
