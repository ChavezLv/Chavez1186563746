#pragma once
#include <hiredis/hiredis.h>
#include "Dictionary.h"
/* #include "TcpConnection.hpp" */
#include <iostream>
using std::cout;
using std::endl;
class KetRecommander
{
public:
    //构造函数用来初始化字典文件和索引文件的位置和要被查询的关键字
    KetRecommander(const string &query,const string &d1,const string &d2)
        :_queryWord(query)
         ,_d1(d1)
         ,_d2(d2)
    {}
    ~KetRecommander()
    {};
    //执行查询
    void execute()
    {
        //连接redis
      redisContext *conn=redisConnect("192.168.10.132",6379);
      if(conn->err)
          perror("redisConnect");
      //先查询redis里面是否存有关键字的查询结构
      redisReply *reply =(redisReply*)redisCommand(conn,"get %s",_queryWord.c_str());               
      //如果没有
      if(reply->type==REDIS_REPLY_NIL)
      {
        //temp为查询到的最终结构
        string temp=doQueryK();
        //查询到后存入redis,设置过期时间为60秒
        redisCommand(conn,"setex %s 60 %s",_queryWord.c_str(),temp.c_str());              
        response(temp);
      }
      //有
      else
      {
        //被再次查询了,更新过期时间为60秒
        redisCommand(conn,"expire %s 60",reply->str);
        response(reply->str);
      }
      //释放redis连接和查询结构指针
      freeReplyObject(reply);
      redisFree(conn);
    }
    //执行查询,这里的查询和Dictionary.h里的查询区别是doQueryK是计算最小编辑距离后的最后结果,这里会引用Dictionary查询结果
    string doQueryK()
    {
        string res;
        _dic.init(_d1,_d2);
        vector<string>tempS=_dic.doQuery(_queryWord);
        //这里要采用mulimap,first是存储查询到的某个关键词的最小编辑距离,second是关键词
        //用muliti_map一是因为和map一样可以根据key来排序,不能用map是因为,map无重复key
        //假如有两个关键词最小编辑距离为2,map会淘汰一个,被关键距离大于2的所挤占
        //这里没有用到词频,有点复杂感觉没必要,因为绝大多数情况下只决定最后一个结果的最后一个关键词
        multimap<int,int>tempM;
        int i=0;
        for(auto str:tempS)
        {
            tempM.insert(make_pair(editDistance(tempS[i]),i));
            i++;
        }
        i=0;
        if(tempM.size())
        {
            //将所有最小十个的编辑距离的词汇append进一个string用" "隔开
            for(auto n:tempM)
            {    
               if(i>9)break;
               res.append(tempS[n.second]);
               res.append(" ");
               i++;
            }
        }

        return res;
    }
    void response(string str)
    {
        cout<<str<<endl;
    }
    //以下的所有函数是用来计算最小编辑距离
    size_t nBytesCode(const char ch)
    {
        if(ch & (1 << 7))
        {
            int nBytes = 1;
            for(int idx = 0; idx != 6; ++idx)
            {
                if(ch & (1 << (6 - idx)))
                {
                    ++nBytes;
                }
                else
                    break;
            }
            return nBytes;
        }
        return 1;
    }

    std::size_t length(const std::string &str)
    {
        std::size_t ilen = 0;
        for(std::size_t idx = 0; idx != str.size(); ++idx)
        {
            int nBytes = nBytesCode(str[idx]);
            idx += (nBytes - 1);
            ++ilen;
        }
        return ilen;
    }

    int triple_min(const int &a, const int &b, const int &c)
    {
        return a < b ? (a < c ? a : c) : (b < c ? b : c);
    }

    int editDistance(const std::string & lhs)
    {//计算最小编辑距离-包括处理中英文
        string rhs=_queryWord;
        size_t lhs_len = length(lhs);
        size_t rhs_len = length(rhs);
        int editDist[lhs_len + 1][rhs_len + 1];
        for(size_t idx = 0; idx <= lhs_len; ++idx)
        {
            editDist[idx][0] = idx;
        }

        for(size_t idx = 0; idx <= rhs_len; ++idx)
        {
            editDist[0][idx] = idx;
        }

        std::string sublhs, subrhs;
        for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
        {
            size_t nBytes = nBytesCode(lhs[lhs_idx]);
            sublhs = lhs.substr(lhs_idx, nBytes);
            lhs_idx += (nBytes - 1);

            for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
            {
                nBytes = nBytesCode(rhs[rhs_idx]);
                subrhs = rhs.substr(rhs_idx, nBytes);
                rhs_idx += (nBytes - 1);
                if(sublhs == subrhs)
                {
                    editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
                }
                else
                {
                    editDist[dist_i][dist_j] = triple_min(
                                                          editDist[dist_i][dist_j - 1] + 1,
                                                          editDist[dist_i - 1][dist_j] + 1,
                                                          editDist[dist_i - 1][dist_j - 1] + 1);
                }
            }
        }
        return editDist[lhs_len][rhs_len];
    }
private:
    string _queryWord;
    string _d1;
    string _d2;
    Dictionary _dic;        
};

