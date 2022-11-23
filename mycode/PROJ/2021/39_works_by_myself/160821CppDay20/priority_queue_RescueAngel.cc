#include <string.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

#include <queue>
using std::priority_queue;

#define MAX 205
char prisonMap[MAX][MAX] = { 0 };
int rescueMap[MAX][MAX] = { 0 };


int n, m , ax, ay;//n, m 输入的地图大小, ax, ay天使的位置

struct Node{
    int _x;
    int _y;
    int _time;
    Node(int x = 0, int y = 0, int step = 0)
    : _x(x)
    , _y(y)
    , _time(step){

    }

    //重载比较运算符
};
bool operator<(const Node &lhs, const Node &rhs) {
    return lhs._time > rhs._time;
}

int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};        

vector<string> vec = {
    "#.#####.",
    "#.a#..r.",
    "#..#x...",
    "..#..#.#",
    "#...##..",
    ".#......",
    "........"
};
/* vector< vector<int> > ; */

int bfs(int ax, int ay){
    /* priority_queue<std::pair<int , int > > que; */
    /* unordered_map<std::pair<int, int>, int> st; */

    priority_queue<Node, vector<Node>, std::less<Node> > que;
    /* priority_queue<Node > que; */

    que.push(Node(ax, ay, 0));
    rescueMap[ax][ay] = 1;//走过的点

    int fx, fy;
    Node tmp;
    while(!que.empty()){
        tmp = que.top();
        que.pop();
        //int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};        
        for(int i = 0; i < 4; ++i){
            fx = dir[i][0] + tmp._x;
            fy = dir[i][1] + tmp._y;
            if(fx >= 0 && fx < n && fy >= 0 && fy < m && \
               prisonMap[fx][fy] != '#' && rescueMap[fx][fy] != 1){
                if(prisonMap[fx][fy] == 'r'){
                    return tmp._time + 1;//逆向找到了朋友
                }
                if(prisonMap[fx][fy] == '.'){
                    que.push(Node(fx, fy, tmp._time + 1));//移动
                }
                if(prisonMap[fx][fy] == 'x'){
                    que.push(Node(fx, fy, tmp._time + 2));//杀怪+移动
                }
                rescueMap[fx][fy] = 1;//可以改成'#'
            }
        }
    } 
    return 0;
    
}
int main(int argc, char* argv[])
{

    /* test01(); */
    int i, j, len;
    char ch;
    while(cin >> n >> m){
        getchar();
        memset(&prisonMap, 0, sizeof(prisonMap));
        memset(&rescueMap, 0, sizeof(rescueMap));
        for(i = 0; i < n; ++i){
            for(j = 0; j < m; ++j){
                ch  = getchar();
                prisonMap[i][j] = ch;

                if(ch == 'a'){
                    ax = i;//记录天使位置
                    ay = j;
                }
            }
            getchar();//"\n"
        }
        /* for(i = 0; i < n; ++i){ */
        /*     for(j = 0; j < m; ++j){ */
        /*         cout << prisonMap[i][j]; */
        /*     } */
        /*     /1* cout << endl; *1/ */
        /* } */
        /* cout << endl; */
        /* cout << "1" << endl; */

        len = 0;
        int num = bfs(ax, ay);
        cout << num << endl;
        if(num){
            cout << "After " << num << " times, the angel and the seven dwarfs lived happily together! "<<  endl;
        }else{
            cout << "Poor ANGEL has to stay in the prison all his life." << endl;
        }
    }
    return 0;
}

