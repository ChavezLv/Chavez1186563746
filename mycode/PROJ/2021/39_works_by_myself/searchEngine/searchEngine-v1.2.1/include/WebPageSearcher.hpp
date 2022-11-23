/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：WebPageSearcher.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 14:56:23
*   @description：
*
================================================================*/


#ifndef __WD_WEBPAGESEARCHER_HPP__
#define __WD_WEBPAGESEARCHER_HPP__

/* #include "JsonMsg.hpp" */
#include "TcpConnection.hpp"
#include "WebPageQuery.hpp"

#include <iostream>
#include <hiredis/hiredis.h>

#include <string>
using std::string;
using std::cout;

namespace wd{

class WebPageSearcher{
public:
    WebPageSearcher(const string &sought, const TcpConnectionPtr &conn)
    :_sought(sought)
    , _conn(conn){

    }
    
    void doQuery(){
        /* std::cout << "WebPageSearcher::doQuery()" <<std::endl; */
/* std::cout << "after WebPageSearcher::doQuery()" <<std::endl; */
        
        redisContext *conn = redisConnect("127.0.0.1", 6379);
        if(conn->err){
            perror("redisConnect");
        }

        redisCommand(conn, "select 2");//选择数据库
        redisReply *reply = (redisReply*) redisCommand(conn, "get %s", \
                            _sought.c_str());
        string res;
        //如果没有
        if(reply->type == REDIS_REPLY_NIL){
            std::cout << "not find in cache" << std::endl;
            WebPageQuery webQuery;
            res = webQuery.doQuery(_sought);//直接就是返回json格式的字符串了

            //查询到后存入redis,设置过期时间为10秒
            redisCommand(conn, "setex %s 30 %s", _sought.c_str(), res.c_str()); 
            
        }else{
            std::cout << "find in cache" << std::endl;
            redisCommand(conn, "expire %s 60", reply->str); 
            redisCommand(conn, "setex %s 30 %s", _sought.c_str(), reply->str); 
            res = reply->str;
        }

        response(res);

        //释放redis连接和查询结构指针
        freeReplyObject(reply);
        redisFree(conn);
    }

    void response(const string &res){
        /* string response = JsonMsg(_sought.size(), 200, _sought).toJsonLayoutStream();//模拟回显服务 */
		_conn->sendInLoop(res);
        
    }

private:
    string _sought;// void doQuery(const string &str){} 形式下这个没有用
    TcpConnectionPtr _conn;



};

}//end of namespace wd

#endif //__WD_WEBPAGESEARCHER_HPP__
