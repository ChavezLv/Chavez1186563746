/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0015test.cc
*   @author     ：anonymous
*   @date       ：2021年09月20日 14:42:01
*   @description：
*
================================================================*/
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

struct Output{
    /* friend bool operator<(const Output &lhs, const Output &rhs); */
    string symbol;
    int maxTimeGap;
    int volume;
    int weightedAveragePrice;
    int maxPrice;

};
bool operator<(const Output &lhs, const Output &rhs){
    return lhs.symbol < rhs.symbol;
}

/* template<typename _Tp> */
class Solution {
public:
    Solution(string inputFile){
        readFromFile(inputFile);
    }
    
private:
    void readFromFile(string inputFile){
        cout << inputFile << endl;
        ifstream ifs(inputFile, std::ios::in);  
        if(!ifs.good()){
            cout << "input file open error" << endl;
        }

        string line;
        vector<int> timestamps;
        while(getline(ifs, line)){
            string symbol;
            int timestamp = 0;
            int quantity = 0;
            int price = 0;

            string temp;
            int cnt = 0;
            for(size_t index = 0; index < line.size(); ++index){
                if(line[index] != ','){
                    temp += line[index];
                }
                if(0 == cnt && line[index] == ','){
                    timestamp = atoi(temp.c_str()); 
                    temp.clear();
                    ++ cnt;
                }
                else if(1 == cnt && line[index] == ','){
                    cout << temp << endl;
                    symbol = temp;
                    temp.clear();
                    ++ cnt;
                }
                else if(2 == cnt && line[index] == ','){
                    quantity = atoi(temp.c_str());
                    temp.clear();
                    ++ cnt;
                }
                /* cout << temp << " " << cnt << endl; */
            } 
            price = atoi(temp.c_str());
            cout << "symbol:" << symbol << "timestamp: " << timestamp << "price:" << price << "quantity:" << quantity << endl; 
            _timestamp[symbol].push_back(timestamp);
            _price[symbol].push_back(make_pair(price, quantity));


            _inputs[symbol].push_back(make_pair(timestamp, make_pair(price, quantity)));
            /* timestamps.push_back(timestamp); */
            /* set<pair<int, int> > setID(make_pair(price, quantity)); //error */
            /* _timestamp.insert(make_pair(symbol, vector<int>(timestamp))); */
            /* set<pair<int, int> > setID; */
            /* setID.insert(make_pair(price, quantity)); */
            /* _price.insert(make_pair(symbol, setID)); */
            /* vector<int, int> vec1(price, quantity); */
            /* vector<pair<long, vector<int, int> > >vec; */
            /* vec.push_back(make_pair(timestamp, vec1)); */
            /* _input.insert(make_pair(symbol, vec)); */ 
        }
        ifs.close();
    }
    
public:
    int calMaxTimeGap(vector<int> & vec){
        int maxTimeGap = 0;
        int curTimeGap = 0;
        for(size_t index = 1; index < vec.size(); ++index){
            curTimeGap = vec[index] - vec[index - 1];
            cout << curTimeGap << "maxTimeGap" << maxTimeGap << endl;
            maxTimeGap = maxTimeGap > curTimeGap ? maxTimeGap : curTimeGap;
        }
        return maxTimeGap;
    }
    int calVolume(vector<pair<int, int> >  & setID){
        int volume = 0;
        for(auto &elem : setID){
            volume += elem.second;
        } 
        return volume;
    }
    int calWeightedAveragePrice(vector<pair<int, int> >  & setID){
        int volume = 0;
        int weightedAveragePrice = 0;
        for(auto &elem : setID){
            volume += elem.second;
            weightedAveragePrice += elem.first * elem.second;   
        }
        weightedAveragePrice /= volume;
        return weightedAveragePrice;
    }
    int calMaxPrice(vector<pair<int, int> >  & setID){
        int maxPrice = 0;
        for(auto &elem : setID){
            maxPrice = maxPrice > elem.first ? maxPrice : elem.first;
        }
        return maxPrice;
    }
    void calAll(Output & output, const string &symbol){
        output.maxTimeGap = calMaxTimeGap(_timestamp[symbol]);
        output.volume = calVolume(_price[symbol]);
        output.maxPrice = calMaxPrice(_price[symbol]);
        output.weightedAveragePrice = calWeightedAveragePrice(_price[symbol]); 
    }
    void store(string outputFile){
        ofstream ofs(outputFile);
        if(!ofs.good()){
            cout << "output file open error!" << endl;
        }

        for(auto &elem: _timestamp){
            Output output;
            output.symbol = elem.first;
            calAll(output, elem.first);
            _outputs.push_back(output);
        }

        sort(_outputs.begin(), _outputs.end());

        for(auto &output : _outputs){
            string layout = output.symbol + "," + to_string(output.maxTimeGap) + "," + \
                            to_string(output.volume) + "," + to_string(output.weightedAveragePrice) + \
                            "," + to_string(output.maxPrice) + "\n";
            ofs << layout;
        }

        ofs.close();

    }

private:
    unordered_map<string, vector<int> > _timestamp;
    unordered_map<string, vector<pair<int, int> > > _price;

    unordered_map<string, vector<pair<int, pair<int, int> > > > _inputs;
    vector<Output> _outputs;
   
};
void test00(){
    /* time_t timestamp = time(nullptr); */
    time_t timestamp = 52924702;
    /* printf("%lu\n", timestamp); */
    struct tm* timeinfo = localtime(&timestamp);
    int year = timeinfo->tm_year + 1900;
    int month = timeinfo->tm_mon + 1;
    int day = timeinfo->tm_mday;
    int date = year * 10000 + month * 100 + day;
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;
    int second = timeinfo->tm_sec;
    fprintf(stdout, "time: %ld, %d, %.2d:%.2d:%.2d\n",timestamp,date, hour, minute, second);
}
void test01(){

}
int main(int argc, char* argv[])
{

    /* test01(); */
    
    Solution sol(argv[1]);
    sol.store(argv[2]);

    return 0;
}


