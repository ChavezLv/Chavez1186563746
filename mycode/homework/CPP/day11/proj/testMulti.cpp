#include <bits/stdc++.h> 
using namespace std; 
  
int main() 
{ 
  
    // initialize container 
    multimap<int, int> mp; 
  
    // insert elements in random order 
    mp.insert({ 1, 40 }); 
    mp.insert({ 2, 60 }); 
    mp.insert({ 3, 20 }); 
    mp.insert({ 1, 50 }); 
    mp.insert({ 4, 50 }); 
    mp.insert({ 2, 30 }); 
  
    cout << "The elements from position 3 in multimap are : \n"; 
    cout << "KEY\tELEMENT\n"; 
  
    // find() function finds the position at which 3 is 
    /*for (auto itr = mp.find(2); itr != mp.end(); itr++) 
        cout << itr->first 
             << '\t' << itr->second << '\n'; 
             */
    auto itr = mp.find(2);
    cout << itr->first<<'\t'<<itr->second<<'\n' ;
    cout << (++itr)->first<<'\t'<<itr->second<<'\n' ;
  
    return 0; 
}
