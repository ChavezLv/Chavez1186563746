#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::vector;
using std::string;
using std::cout;
using std::endl;

/* class Solution { */
/* public: */
/*     bool exist(vector<vector<char>>& board, string word) { */
/*         //转换类型 */
/*         /1* vector<char> ppath = (vector<char>)word.c_path(); *1/ */
/*         vector<char> path; */
/*         path.assign(word.begin(), word.end()); */

/*         for(int i = 0; i < static_cast<int>(board.size()); ++i){ */
/*             for(int j = 0; j < (int)board[0].size(); ++i){ */
/*                 if(dfs(board, path, i, j, 0)){ */
/*         cout << 1 << endl; */
/*                     return true; */    
/*                 } */
/*             } */
/*         } */
        
/*         return false; */
            
/*     } */
/*     bool dfs(vector<vector<char>>& board, vector<char> words, int a, int b, int c){ */
/*         if(a < 0|| b < 0|| a >= (int)board[0].size() || b >= (int)board[0].size() ||\ */
/*            words[c] != board[a][b]){ */
/*             return false; */ 
/*         } */

/*         if(c == (int)words.size() - 1){ */
/*             return true; */
/*         } */

/*         char tmp = board[a][b]; */
/*         board[a][b] = '/'; */
/*         bool res = dfs(board, words, a + 1, b, c + 1)|| dfs(board, words, a - 1, b, c + 1)\ */
/*                    || dfs(board, words, a, b + 1, c + 1) || dfs(board, words, a, b - 1, c + 1); */
/*         board[a][b] = tmp; */
/*         return res; */

/*     } */

/* }; */
/* int main() */
/* { */
/*     vector<vector<char>> board = {{'A','B','C','E'},{'S','F','C','S'},{'A','D','E','E'}}; */
    
/*     Solution sol; */
/*     cout << sol.exist(board, "ABCCED") << endl; */

/*     return 0; */
/* } */
class Solution3 {
public:
    bool exist(vector<vector<char>>& board, string word) {
        //转换类型
        /* vector<char> ppath = (vector<char>)word.c_path(); */
        vector<char> path;
        path.assign(word.begin(), word.end());

        for (int i = 0; i < static_cast<int>(board.size()); ++i) {
            for (int j = 0; j < (int)board[0].size(); ++j) {
                if (dfs(board, path, i, j, 0)) {
                    //cout << 1 << endl;
                    return true;
                }
            }
        }

        return false;

    }
    bool dfs(vector<vector<char>>& board, vector<char> words, int a, int b, int c) {
        if (a < 0 || b < 0 || a >= (int)board.size() || b >= (int)board[0].size() || \
            words[c] != board[a][b]) {
            return false;
        }

        if (c == (int)words.size() - 1) {
            return true;
        }

        char tmp = board[a][b];
        board[a][b] = '/';
        bool res = dfs(board, words, a + 1, b, c + 1) || dfs(board, words, a - 1, b, c + 1)\
                   || dfs(board, words, a, b + 1, c + 1) || dfs(board, words, a, b - 1, c + 1);
        board[a][b] = tmp;
        return res;

    }

};
int main()
{
    Solution3 sol;
    vector<vector<char>> board = { {'A','B','C','E'},{'S','F','C','S'},{'A','D','E','E'}  };
    /* vector<vector<char>> board = { {'a','b'}, {'c','d'}  }; */
    /* vector<vector<char>> board = { {'a'}, {'b'}, { 'c','d'  }}; */
    cout << sol.exist(board, "ABCCED") << endl;
    /* cout << sol.exist(board, "abcd") << endl; */

    return 0;
}
