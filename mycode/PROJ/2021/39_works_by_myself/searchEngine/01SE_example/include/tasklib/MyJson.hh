/* <MyJson> -*- C++ -*- */
#ifndef __MYJSON_H__
#define __MYJSON_H__

#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

#include <fstream>
#include <memory>
#include <string>

/*
 * 客户端发 { "query" : "string" }
 * 服务器发 { "result" : [ { "res" : "string1" }, { "res" : "string2" }, { "res" : "string3" } ] }
 */

namespace wd
{

class MyJson
{
public:
    MyJson(std::ifstream & ifs);
    MyJson(std::ofstream & ofs);

    void json_write(const std::string & key, const std::string & value);
    void json_write(const std::string & key, Json::Value & arrayObj);

    const std::string str();

    void json_read(const std::string & json);

    Json::Value & getRoot();

private:
    Json::Value _root;

    Json::StreamWriterBuilder _wBuilder;
    const std::unique_ptr<Json::StreamWriter> writer;

    Json::CharReaderBuilder _rBuilder;
    JSONCPP_STRING errs;
};

} // end of namespace wd
#endif

