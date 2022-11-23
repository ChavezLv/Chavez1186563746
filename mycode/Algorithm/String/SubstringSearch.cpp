#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
using std::cout;
using std::cin;
using std::string;
using std::endl;
using LL=long long;

const LL MOD=1000000007;
const LL P=10000019;
const LL MAXN=1010;

LL powP[MAXN],H1[MAXN]={0},H2[MAXN]={0};
std::vector<std::pair<int,int>>pr1,pr2;
namespace strg{
void init(int len){
    powP[0]=1;
    for(int i=1;i<len;++i){
        powP[i]=(powP[i-1]*P)%MOD;
    }
}

void calH(LL H[],std::string &str){
    H[0]=str[0];
    for(int i=1;i<(int)str.length();i++){
        H[i]=(H[i-1]*P+str[i])%MOD;
    }
}
//calSingleSubH计算H[i·j]
int calSingleSubH(LL H[], int i, int j){
    if(i ==0)return H[j];//H[0·…j]单独处理
    return ((H[j]-H[i-1] * powP[j-i+ 1]) %MOD + MOD)% MOD;
}
//calSubH计算所有子串的hash值,并将<子串hash值,子串长度>存入pr
void calSubH(LL H[], int len, std::vector<std::pair<int,int>>&pr){
    for (int i=0;i< len; ++i){
        for(int j=i;j<len; j++){
            int hashValue=calSingleSubH(H,i,j);
            pr.push_back (std::make_pair(hashValue,j-i+1));
        }
    }
}

}

//计算pr1和pr2中相同的hash值,维护最大长度
int getMax() {
    int ans = 0;
    for(int i =0; i< (int )pr1.size();i++){
        for (int j=0;j< (int)pr2.size();j++){
            if(pr1[i].first==pr2[j].first){
                ans=std::max(ans, pr1[i] .second);
            }
        }
    }
    return ans;
}
using namespace strg;
int main() {
    string str1,str2;
    getline (cin,str1);getline(cin, str2);
    init(std::max(str1.length(), str2.length()));//初始化powP数组calH(H1,str1);//分别计算str1和str2的hash值
    calH(H2, str2);
    calSubH(H1, str1. length(), pr1);
    //分别计算所有H1[i…j]和H2[i…j]
    calSubH(H2,str2. length(), pr2);
    printf ("ans = %d\n", getMax());//输出最大公共子串长度return 0;
    return  0;
}
