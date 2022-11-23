#include<iostream>
#include<vector>
#include<string>
using namespace std;
class Track{
public:
    Track(int n)
        :_n(n)   
        ,_path(n,string(n,'O'))
        ,_col(n,false)
        ,_diag(n*2,false)
         ,_udiag(2*n,false){
             dfs();
         }
    void dfs(int x=0){
        if(x==_n){
            _chessBoard.push_back(_path);
            return;
        }
        for(int y=0;y<_n;++y){
            if(_col[y]||_diag[x+y]||_udiag[x-y+_n]){
                continue;
            }
            _col[y]=true;
            _diag[x+y]=true;
            _udiag[x-y+_n]=true;
            _path[x][y]='X';
            dfs(x+1);
            _path[x][y]='O';
            _col[y]=_diag[x+y]=_udiag[x-y+_n]=false;
                         
        }
    }
    void print(){
        int i=1;
        for(auto chess:_chessBoard){
            cout<<"第"<<i++<<"种:"<<endl;
            for(auto elem:chess){
                cout<<elem<<endl;
            }
        }
        cout<<_n<<"*"<<_n<<":"<<_chessBoard.size()<<endl;
        }
private:
    const int _n;
    vector<string>_path;
    vector<vector<string>>_chessBoard;
    vector<bool>_col,_diag,_udiag;
};
int main(){
    Track tk(8);
    tk.print();
    return 0;
}
