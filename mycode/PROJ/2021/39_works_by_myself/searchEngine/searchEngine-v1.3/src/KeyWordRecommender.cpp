/*
*  File    KeyWordRecommender.cc
*  Author  WayneGreat
*  Date    2021-09-05  11:59:26
*  Describe 
*/

#include "JsonMsg.hpp"
#include "Configuration.hpp"
#include "TcpConnection.hpp"
#include "KeyRecommender.hpp"

#include <hiredis/hiredis.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>


using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::multimap;
using std::pair;


namespace wd
{

struct KeyWords
{
    string keyWords;
};


KeyRecommender::KeyRecommender(const string &queryWord, const TcpConnectionPtr &conn, ConfigurationPtr &config)
: _queryWord(queryWord)
, _config(config)
, _conn(conn)
{
    readInfoFromFile(); //导入词典及索引文件
}

void KeyRecommender::doProcess()
{
    //连接redis
    redisContext *conn = redisConnect("127.0.0.1", 6379);
    if (conn->err) {
        perror("redisConnect");
    }

    //先查询redis里面是否存有关键字的查询结构
    redisReply *reply = (redisReply *)redisCommand(conn, "get %s", _queryWord.c_str());

    vector<string> keyWords; //获取相关单词后保存的容器
    //如果没有
    if (reply->type == REDIS_REPLY_NIL) {
        //temp为查询到的最终结构
        // string temp = doQueryK();
        findWordInDict(keyWords); //根据关键字获取词典库中的单词
        string temp = calSimilarity(keyWords);
        //计算查询词与关键词的相似度，将所有最小十个的编辑距离的词汇append进一个string用" "隔开

        //查询到后存入redis,设置过期时间为60秒
        redisCommand(conn, "setex %s 60 %s", _queryWord.c_str(), temp.c_str());

        responseByJson(temp);
    }
    //有
    else {
        //被再次查询了,更新过期时间为60秒
        redisCommand(conn, "expire %s 60", reply->str);
        responseByJson(reply->str);
        atoi(reply->str);
    }
    //释放redis连接和查询结构指针
    freeReplyObject(reply);
    redisFree(conn);
}

void KeyRecommender::responseByJson(string str)
{
    cout << str << endl;
    json js = 
    {
        {"msgLength", 1},
        {"msgType", 100},
        {"msgContent", str}
    };

    string responseJsonLayoutString = JsonMsg().toJsonLayoutStreamForWebSearch(js);
    cout << responseJsonLayoutString << endl;
    _conn->sendInLoop(responseJsonLayoutString);
    
}


void KeyRecommender::readInfoFromFile()
{
    string zhDictPath = _config->getConfigMap()["zhdict"];
    string zhDictIndexPath = _config->getConfigMap()["zhdictIndex"];

    ifstream zhDictIfs(zhDictPath.c_str());
    ifstream zhDictIndexIfs(zhDictIndexPath.c_str());

    if((!zhDictIfs) || (!zhDictIndexIfs)) {
        cout << "zhDict or zhDictIndex lib open error" << endl;
    }

    string line, word;
    int wordFreq;

    while (getline(zhDictIfs, line)) {
        istringstream iss(line);
        iss >> word >> wordFreq;
        _zhDict.push_back({word, wordFreq}); 
    }

    string singleWord;
    int wordIndex;
    set<int> dictSetIndex;

    while (getline(zhDictIndexIfs, line)) {
        istringstream iss(line);
        iss >> singleWord;
        while (iss >> wordIndex) {
            dictSetIndex.insert(wordIndex);
        }
        _zhDictIndex[singleWord] = dictSetIndex;
        dictSetIndex.clear();
    }

    // for (auto &elem : _zhDictIndex) {
    //     cout << elem.first << " ";
    //     for (auto &sitem : elem.second) {
    //         cout << sitem << " ";
    //     }
    //     cout << endl;
    // }

    cout << "readInfoFile success!" << endl;
}

//====================================================
void KeyRecommender::findWordInDict(vector<string> & temp)
{
    temp = doQuery(_queryWord);
}

vector<string> KeyRecommender::doQuery(string &word)
{
    vector<string> temp;
    set<int> res;
    string s;
    int i = 1;
    int flag = 0;
    while (flag < (int)word.size())
    //循环提取查询关键字中每个字
    {
        s = sub(word, i, flag, i); //提取单个字
        if (_zhDictIndex.count(s)) //字在索引表出现
        {
            for (auto num : _zhDictIndex[s]) //遍历索引值，保存到set中
            {
                res.insert(num);
            }
        }
        i++;
    }
    for (auto n : res) //遍历set，获取vector下标
    {
        temp.push_back(_zhDict[n].first); //根据下标获取单词，保存到vector中
    }
    return temp; //返回单词vector集合
}

int KeyRecommender::is_zh_ch(char p)
{
    /*
    汉字的两个字节的最高为都为1,这里采用判断最高位的方法
    将p字节进行移位运算，右移8位，这样，如果移位后是0，
    则说明原来的字节最高位为0，不是1那么也就不是汉字的一个字节
    */
    if (~(p >> 8) == 0)
    {
        return 1; //代表不是汉字
    }

    return -1;
}

string KeyRecommender::sub(string str, int start, int &flag, int end)
{
    //flag为外部传入的标志位,来判断是否可以结束外部的循环
        //因为汉字和字母字符长度不一,不能简单按长度遍历
    if (typeid(str) == typeid(string) && str.length() > 0)
    {
        int len = str.length();

        string tmp = "";

        //先把str里的汉字和英文分开
        vector<string> dump;
        int i = 0;
        while (i < len)
        {
            if (is_zh_ch(str.at(i)) == 1)
            {
                dump.push_back(str.substr(i, 3));
                i = i + 3;
            }
            else
            {
                dump.push_back(str.substr(i, 1));
                i = i + 1;
            }
        }
        end = end > 0 ? end : dump.size(); //end默认为dump.size
        if (start < 0 || start > end)
            printf("start is wrong");
        //直接从dump里取即可
        for (i = start; i <= end; i++)
        {
            tmp += dump[i - 1];
        }
        //size()>1表示此次查询是汉字,flag+=3
        if (tmp.size() > 1)
            flag += 3;
        //否则为英语字符+=1
        else
            flag += 1;
        return tmp;
    }
    else
    {
        printf("str is not string\n");
        return "";
    }
}

//====================================================
string KeyRecommender::calSimilarity(vector<string> & tempS)
// vector<string> KeyRecommender::calSimilarity(vector<string> & tempS)
{
    //这里要采用mulimap,first是存储查询到的某个关键词的最小编辑距离,second是关键词
    //用muliti_map一是因为和map一样可以根据key来排序,不能用map是因为,map无重复key
    //假如有两个关键词最小编辑距离为2,map会淘汰一个,被关键距离大于2的所挤占
    //这里没有用到词频,有点复杂感觉没必要,因为绝大多数情况下只决定最后一个结果的最后一个关键词

    string res;
    // vector<string> ret;
    multimap<int,int>tempM;
    int i = 0;
    for (auto str : tempS)
    {
        tempM.insert({editDistance(tempS[i]), i}); //i为查询字对应单词vector的下标
        i++;
    }
    i = 0;
    if (tempM.size())
    {
        for (auto n : tempM)
        {
            if (i > 9)
                break;
            res.append(tempS[n.second]);
            res.append(" ");
            // ret.push_back(tempS[n.second]);
            i++;
        }
    }

    return res;//将所有最小十个的编辑距离的词汇append进一个string用" "隔开
    // return ret; //返回十个最小编辑距离的单词的vector集合
}

size_t KeyRecommender::nBytesCode(const char ch)
{
    if (ch & (1 << 7))
    {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx)
        {
            if (ch & (1 << (6 - idx)))
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

size_t KeyRecommender::length(const string &str)
{
    std::size_t ilen = 0;
    for (std::size_t idx = 0; idx != str.size(); ++idx)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

int KeyRecommender::triple_min(const int &a, const int &b, const int &c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int KeyRecommender::editDistance(const string &lhs)
{
    string rhs = _queryWord;
    size_t lhs_len = length(lhs);
    size_t rhs_len = length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];
    for (size_t idx = 0; idx <= lhs_len; ++idx)
    {
        editDist[idx][0] = idx;
    }

    for (size_t idx = 0; idx <= rhs_len; ++idx)
    {
        editDist[0][idx] = idx;
    }

    std::string sublhs, subrhs;
    for (std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
    {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);

        for (std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
        {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);
            if (sublhs == subrhs)
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

}// end of namespace wd

