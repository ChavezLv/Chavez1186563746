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
#include <fstream>
#include <algorithm>
#include <set>
using namespace std;

struct Output{
    string symbol;
    int maxTimeGap;
    int volume;
    int weightedAveragePrice;
    int maxPrice;
    Output()
    : symbol("")
    , maxTimeGap(0)
    , volume(0)
    , weightedAveragePrice(0)
    , maxPrice(0){

    }
    bool operator()(const Output &lhs, const Output &rhs) const {
        return lhs.symbol < rhs.symbol;
    }
};
/* bool operator<(const Output &lhs, const Output &rhs){ */
/*     return lhs.symbol < rhs.symbol; */
/* } */
/* bool operator>(const Output &lhs, const Output &rhs){ */
/*     return lhs.symbol < rhs.symbol; */
/* } */

/* struct OutputCompare{ */
/*     bool operator()(const Output &lhs, const Output &rhs) const { */
/*         return lhs.symbol < rhs.symbol; */
/*     } */
/* }; */


/* template<typename _Tp> */
class Solution {
public:
    Solution(string inputFile, string outputFile = "output.csv"){
        readFromFile(inputFile);
        store(outputFile);
    }
private:
    void store(string outputFile){
        ofstream ofs(outputFile);
        if(!ofs.good()){
            cout << "output file open error!" << endl;
        }

        for(auto &elem: _inputs){
            Output output;
            getAllMembersOfOutput(output, elem.first);
            _outputs.insert(output);
            /* _outputs.push_back(output); */
        }

        /* sort(_outputs.begin(), _outputs.end(), ); //set的时候不可以用sort*/

        for_each(_outputs.begin(), _outputs.end(), [&ofs](const Output & output){
                    string layout = output.symbol + "," + to_string(output.maxTimeGap) + "," + \
                    to_string(output.volume) + "," + to_string(output.weightedAveragePrice) + \
                    "," + to_string(output.maxPrice) + "\n";
                    ofs << layout;
                 });

        ofs.close();

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
                    symbol = temp;
                    temp.clear();
                    ++ cnt;
                }
                else if(2 == cnt && line[index] == ','){
                    quantity = atoi(temp.c_str());
                    temp.clear();
                    ++ cnt;
                }
            } 
            price = atoi(temp.c_str());
            _inputs[symbol].push_back(make_pair(timestamp, make_pair(price, quantity)));
        }
        ifs.close();
    }
    
    void getAllMembersOfOutput(Output & output, const string &symbol){
        output.symbol = symbol;
        int curTimeGap = 0;
        int preTimeStamp = 0;
        for(auto &elem : _inputs[symbol]){
            if(preTimeStamp != 0){
                curTimeGap = elem.first - preTimeStamp;
                output.maxTimeGap = output.maxTimeGap > curTimeGap ? output.maxTimeGap : curTimeGap;
            } 
            preTimeStamp = elem.first;

            output.volume += elem.second.second;
            output.weightedAveragePrice += elem.second.first * elem.second.second;
            output.maxPrice = output.maxPrice > elem.second.first ? output.maxPrice : elem.second.first;
        }
        output.weightedAveragePrice /= output.volume;
    }

private:
    unordered_map<string, vector<pair<int, pair<int, int> > > > _inputs;
    set<Output, Output> _outputs;
    /* set<Output, OutputCompare> _outputs; */
   
};

int main(int argc, char* argv[])
{
    if(3 != argc){
        cout << "error args" << endl;
        return -1;
    }
    Solution sol(argv[1], argv[2]);

    /* sol.store(argv[2]); */
    return 0;
}
