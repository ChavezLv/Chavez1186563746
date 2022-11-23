/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：12testRedis.cc
*   @author     ：anonymous
*   @date       ：2021年09月03日 20:35:12
*   @description：
*
================================================================*/


#ifndef _REDIS_H_
#define _REDIS_H_

#include <hiredis/hiredis.h>

#include <unistd.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>


class Redis
{
public:

    Redis(){}

    ~Redis()
	{
		/* freeReplyObject(this->_reply); */
        this->_connect = NULL;
		this->_reply = NULL;
	}

	bool connect(std::string host, int port)
	{
	    this->_connect = redisConnect(host.c_str(), port);
		if(this->_connect != NULL && this->_connect->err)
		{
		    printf("connect error: %s\n", this->_connect->errstr);
			return 0;
		}
		return 1;
	}

    std::string get(std::string key)
	{
        if(_reply){
		    _reply = (redisReply*)redisCommand(_connect, "GET %s", key.c_str());

        }

		std::string str = _reply->str;
    std::cout << str << std::endl;
		return str;
	}

	void set(std::string key, std::string value)
	{
        if(_reply){
            redisCommand(_connect, "SET %s %s", key.c_str(), value.c_str());
        }
	}

    /* int redis_insert(redisContext *c, char *key, char *value, int ex) */
    int redis_insert(const char *key, const char *value, int ex)
    {
        redisReply *reply = NULL;

        if(ex < 0)
        {
            printf("Error: Invalid input argument ex[%d]", ex);
            return -1;
        }

        /*test if the key has been existed*/
        reply = (redisReply *)redisCommand(_connect, "GET %s", key);
        if( reply == NULL)
        {
            printf("Error: GET error!\n");
            return -1;
        }
        if(reply->type != REDIS_REPLY_STRING && reply->type != REDIS_REPLY_NIL)
        {
            printf("Error: GET error!\n");
            freeReplyObject(reply);
            return -1;
        }

        if(reply->type == REDIS_REPLY_STRING)
        {
            printf("Error: The key has existed.\n");
            freeReplyObject(reply);
            return -1;
        }
        freeReplyObject(reply);


        /*insert*/
        reply = NULL;
        if(ex != 0 )
        {
            reply = (redisReply *)redisCommand(_connect, "SET %s %s EX %d", key, value, ex);
        }
        else
        {
            reply = (redisReply *)redisCommand(_connect, "SET %s %s", key, value); /*without ex*/
        }

        if( reply == NULL)
        {
            printf("Error: SET error!\n");
            return -1;
        }
        if( !(reply->type == REDIS_REPLY_STATUS && memcmp(reply->str, "OK", 2) == 0) )
        {
            printf("Error: SET error!\n");
            freeReplyObject(reply);
            return -1;
        }


        freeReplyObject(reply);
        return 0;
    }

private:

    redisContext* _connect;
	redisReply* _reply;

};

#endif  //_REDIS_H_

#if 1

/* #include "redis.h" */
 
int main()
{
	Redis *r = new Redis();
	if(!r->connect("192.168.10.132", 6379))
	{
		printf("connect error!\n");
		return 0;
    }else{
        printf("connect success!\n");
    }
	/* r->set("name", "Mayuyu"); */
    
    char buf[] = "name";
    r->redis_insert(buf, std::string("alyu").c_str(), 1);
    sleep(3);
    std::cout << std::endl;
	printf("Get the name is %s\n", r->get("name").c_str());

	delete r;
	return 0;
}

#endif
