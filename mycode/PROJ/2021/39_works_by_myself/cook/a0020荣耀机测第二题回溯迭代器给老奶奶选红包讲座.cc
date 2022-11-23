#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;
bool sortByFirst(pair<int, int> &lhs, pair<int, int> rhs){
    return lhs.first < rhs.first;
}
bool sortBySecond(pair<int, int> &lhs, pair<int, int> rhs){
    return lhs.second > rhs.second;
}
class Solution{
public:
    void push_back(const int &startTime, const int &endTime){
        _lectures.push_back(make_pair(startTime, endTime));
    }
    void getCommands(){
        vector<pair<int, int> > vecs;
        vector<int> starts;
        sort(_lectures.begin(), _lectures.end(), sortByFirst);
        
        for(size_t index = 0; index < _lectures.size(); ++index){
            if(0 == vecs.size()){
                vecs.push_back(_lectures[index]);
            }else{
                /* cout << _lectures[index].first << " vs " << vecs.back().second << endl; */ 
                if(_lectures[index].first >= vecs.back().second){
                    /* if(index < _lectures.size() - 1 && _lectures[index + 1].first >= vecs.back().second && \ */
                    if(index < _lectures.size() - 1 && \
                       _lectures[index].second == _lectures[index + 1].second){
                        continue;//如果(4,6) (5,6) 选择晚开始的
                    }
                    if(index < _lectures.size() - 1 &&\
                       _lectures[index].first == _lectures[index + 1].first &&\
                       _lectures[index].second > _lectures[index + 1].second){
                        continue;//如果(3,4) (3,5)选择(3,4)
                    }
                    vecs.push_back(_lectures[index]);
                }else{
                    starts.push_back(index);
                }
            }
        }
        _commands.push_back(vecs);
        /* for(auto &elem: vecs){ */
        /*     cout << "(" << elem.first << "," << elem.second << ") "; */
        /* } */
        /* cout << endl; */

        vecs.clear();
        if(0 != starts.size()){
            for(size_t index = 0; index < starts.size(); ++index){
                for(size_t start = starts[index]; start < _lectures.size(); ++start){
                    if(0 == vecs.size()){
                        vecs.push_back(_lectures[start]);
                    }else{
                        if(_lectures[start].first >= vecs.back().second){
                            vecs.push_back(_lectures[start]);
                        }
                    }
                }
                _commands.push_back(vecs);
            }
        }
            
        size_t maxLec = 0;
        for(auto &command : _commands){
            if(command.size() > maxLec){
                maxLec =  command.size();  
            }        
        }
        for(auto it = _commands.begin(); it != _commands.end(); ){
            if((*it).size() != maxLec){
                it = _commands.erase(it);
            }else{
                ++it;
            }
        }
    }
    
    void getOneCommand(){
        /* int minEndTime = 256; */
        if(_commands.size() <= 1){
            return;
        }
        vector<int> minEndTimes(_commands.size() - 1, 25);//24hours
        auto minEndTime = minEndTimes.end() - 1;
        for(auto commandIt = _commands.end() - 1; commandIt >= _commands.end();){
        /* for(auto &elem: (*commandIt)){ */
        /*     cout << "(" << elem.first << "," << elem.second << ") "; */
        /* } */
        /* cout << endl; */
            
            /* sort((*commandIt).begin(), (*commandIt).end(), sortBySecond); */
            if((*commandIt).front().second < (*minEndTime)){
                *minEndTime = (*commandIt).front().second;
                -- commandIt;
                -- minEndTime;
            }else{
                commandIt = _commands.erase(commandIt);
            }
        }
        if(_commands.size() <= 1){
            return;
        }
        int maxStartTime = 0;
        for(auto commandIt = _commands.begin(); commandIt != _commands.end();){
            sort((*commandIt).begin(), (*commandIt).end(), sortByFirst);
            if((*commandIt).back().second > maxStartTime){
                maxStartTime = (*commandIt).back().first;
                ++ commandIt;
            }else{
                commandIt = _commands.erase(commandIt);
            }
        }
    }
    void printCommand(){
        /* getCommands(); */
        /* for(auto &elem: _commands[0]){ */
        /*     cout << "(" << elem.first << "," << elem.second << ") "; */
        /* } */
        /* cout << endl; */
        getMaxLectures();
        getBestLecture();
        /* getOneCommand(); */
        cout << "size: " << _commands.size() << endl;
        for(auto &elem: _commands[0]){
            cout << "(" << elem.first << "," << elem.second << ") ";
        }
        cout << endl;
        /* for(auto &elem: _commands[1]){ */
        /*     cout << "(" << elem.first << "," << elem.second << ") "; */
        /* } */
    }
    void getBestLecture(){
        if(_commands.size() <= 1){
            return;
        }

        //思路: 怎么从多个同样大小的数组去除一些不太行的
        typedef vector<pair<int, int> >::iterator vecIter;
        /* vector<pair<vecIter, int> > commandIterVec; //因为所有的command都是同步进行的, 所以不需要int*/
        vector<vecIter > commandIterVec; //因为所有的command都是同步进行的, 所以不需要int
        int step = 0;
        /* for(auto &command: _commands){ */
        /*     commandIterVec.push_back(command.end() - 0); */
        /* } */
        bool loopFlag = true;
        while(loopFlag){
            commandIterVec.clear();
            for(auto &command: _commands){
                commandIterVec.push_back(command.end() - 1 - step);
            }

            int curMinEndTime = INT32_MAX;
            for(auto &vec : commandIterVec){
                if(vec->second < curMinEndTime){
                    curMinEndTime = vec->second;
                }
            }
            auto it = remove_if(_commands.begin(), _commands.end(),[&](vector<pair<int, int> > & command){
                      /* if(command.size() >= 1){ */
                            return command[command.size() - 1 - step].second > curMinEndTime;
                      /* } */
                      });    
            _commands.erase(it, _commands.end());


            if(_commands.size() <= 1 || step == (int)_commands[0].size() - 1){
                loopFlag  = false;
            }else{
                ++ step;
            }

            
        }
        if(_commands.size() <= 1){
            return ;
        }
         
        loopFlag = true;
        step = 0;
        cout << _commands.size() << endl;
        while(loopFlag){
            commandIterVec.clear();
            for(auto &command: _commands){
                commandIterVec.push_back(command.begin() + step);
            }

            int curMaxStartTime= INT32_MIN;
            for(auto &vec : commandIterVec){
                if(vec->first > curMaxStartTime){
                    curMaxStartTime = vec->first;
                }
            }
            auto it = remove_if(_commands.begin(), _commands.end(),[&](vector<pair<int, int> > & command){
                      /* if(command.size() >= 1){ */
                            return command[step].first < curMaxStartTime;
                      /* } */
                      });    
            _commands.erase(it, _commands.end());



            if(_commands.size() <= 1 || step == (int)_commands[0].size()){
                loopFlag  = false;
            }else{
                ++ step;
            }
        }
        //每一个相同组合, 比如(1,2) (3, 4), (5, 8) 和 (1,3 ), (3,4), (6,8)
        /* vector<int> vec(_commands[0].size(), 25); */
        /* for(auto command = _commands.begin(); command != _commands.end();\ */
        /*     ){ */
        /*     for(int index = (*command).size() - 1; index >= 0; --index){ */
                
        /*         if((*command)[index].second < vec[index]){ */
        /*             vec[index] = (*command)[index].second; */     

        /*         } */

        /*         if((*command)[index].second > vec[index]){ */
        /*             command = _commands.erase(command); */
        /*         }else{ */
        /*             ++ command; */
        /*         } */
        /*     } */
            
        /* } */
         
    }
    void getMaxLectures(){
        sort(_lectures.begin(), _lectures.end(), sortByFirst);
        backTracking(0);
    }
    void backTracking(size_t startIndex){
        /* cout << "size() : " << _lectures.size() << endl; */
        /* cout << _command.size() << endl; */
        /* cout << _commands.size() << endl; */
        /* cout << "?" << endl; */
        if(0 != _commands.size()){
            if(_command.size() > _commands[0].size()){//if size() is 0, _commands[0].size()会错的
                _commands.clear();
                _commands.push_back(_command);
                /* return; */
            }else if(_command.size() == _commands[0].size()){
                _commands.push_back(_command);
            }

        }else{
            if(0 != _command.size()){
                _commands.push_back(_command);
                /* return; */
            }
        }

        /* cout << "size() : " << _lectures.size() << endl; */
        /* cout << _command.size() << endl; */
        /* cout << _commands.size() << endl; */
        for(size_t index = startIndex; index < _lectures.size(); ++index){
            /* cout << _lectures[index].first << " vs " << _command.back().second << endl; //error*/ 
            if(index < _lectures.size() - 1 && _lectures[index] == _lectures[index + 1]){
                continue;//避免有重复的连续, 比如(1,3) (3,4) (3,4)
            }
            if(0 == _command.size()){
                _command.push_back(_lectures[index]);
            }else{
                if(_command.back().second <= _lectures[index].first){
                    _command.push_back(_lectures[index]);
                }else{
                    continue;
                }
            }
            backTracking(startIndex + 1);
            _command.pop_back();
        }
    }
private:
    vector<pair<int, int> > _lectures;
    vector<vector<pair<int, int> > > _commands;
    vector<pair<int, int> > _command;
};

int main(){
    string lecture;
    Solution sol;
    while(cin >> lecture){
        /* cout << "lecture: " << lecture << " " ; */
        string temp;
        int startTime = 0;
        int endTime = 0;
        for(size_t index = 0; index < lecture.size(); ++index){
            /* if(lecture[index] >= '0' && lecture[index] < '0'){ */
            if(lecture[index] >= '0' && lecture[index] <= '9'){
                temp += lecture[index];
            }
            if(lecture[index] == ','){
                startTime = stoi(temp);
                temp.clear();
            }
            else if(lecture[index] == ')'){
                endTime = stoi(temp);
            }
        }
        /* cout << startTime << "," << endTime << endl; */
        sol.push_back(startTime, endTime);
    }
    sol.printCommand();
    return 0;
}
