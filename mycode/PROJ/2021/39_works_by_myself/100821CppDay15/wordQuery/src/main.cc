#include "../include/Query.hh"
#include "../include/TextQuery.hh"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

//基础测试, only search one word
void test(int argc, char* argv[]){
    TextQuery file = getFile(argc, argv);
    do{
        string word;
        cout << "please input the word you want to search:" << endl;
        if(!getWord(word)){
            break;
        }

        Query name(word);
        const QueryResult res = name.eval(file);
        cout << endl << "Executing Query for: "  << name << endl;
        cout << res << endl;
        
    }while(1);
}

//word1 & word2
void test1(int argc, char* argv[]){
    TextQuery file = getFile(argc, argv);
    do{
        string word1, word2;
        cout << "please input the word you want to search:" << endl;
        if(!getWords(word1, word2)){
            break;
        }

        Query andQuery = Query(word1) & Query(word2);
        cout << endl << "Executing Query for: "  << andQuery << endl;

        QueryResult res = andQuery.eval(file);
        cout << res << endl;

        /* res = Query(word1).eval(file); */  
        /* cout << endl << "Executing Query for: "  << Query(word1) << endl; */
        /* cout << res << endl; */
        
        /* res = Query(word2).eval(file); */
        /* cout << endl << "Executing Query for: "  << Query(word2) << endl; */
        /* cout << res << endl; */

    }while(1);
}

void test2(int argc, char* argv[]){
    TextQuery file = getFile(argc, argv);
    do{
        string word;
        cout << "please input the word1 & word2 you want to search:" << endl;
        if(!getWord(word)){
            break;
        }

        Query notQuery = ~Query(word);
        cout << endl << "Executing Query for: "  << notQuery << endl;
        QueryResult res = notQuery.eval(file);
        cout << res << endl;

        /* Query name(word); */
        /* res = name.eval(file); */
        /* cout << endl << "Executing Query for: "  << name << endl; */
        /* cout << res << endl; */
        
    }while(1);
}


void test3(int argc, char* argv[]){
    TextQuery file = getFile(argc, argv);
    do{
        string word1, word2;
        cout << "please input the word1 & word2 | word3 you want to search:" << endl;
        if(!getWords(word1, word2)){
            break;
        }
        string word3;
        cout << endl << "enter third word exist: "  << endl;
        cin >> word3;

        Query qAll = Query(word1) & Query (word2) | Query(word3);
        cout << endl << "Executing Query for: "  << qAll << endl;
        QueryResult res = qAll.eval(file);
        cout << res << endl;
        
    }while(1);
}
void test4(int argc, char* argv[]){
    TextQuery file = getFile(argc, argv);
    do{
        string word1, word2;
        cout << "please input the word1 & word2 & ~word3 you want to search:" << endl;
        if(!getWords(word1, word2)){
            break;
        }
        string word3;
        cout << endl << "enter third word doesn's exist: "  << endl;
        cin >> word3;

        Query qAll = Query(word1) & Query (word2) & ~Query(word3);
        cout << endl << "Executing Query for: "  << qAll << endl;
        QueryResult res = qAll.eval(file);
        cout << res << endl;
        
    }while(1);
}
int main(int argc, char* argv[])
{
    /* test(argc, argv); */    
    /* test2(argc, argv); */    
    /* test1(argc, argv); */    
    /* test3(argc, argv); */    
    test4(argc, argv);    
    return 0;
}

