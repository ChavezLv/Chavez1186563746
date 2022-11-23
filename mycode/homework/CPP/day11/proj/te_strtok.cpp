#include <iostream> 
#include <cstring> 
using namespace std; 
int main() 
{
 char sentence[]="This is a sentence with 7 tokens"; 
 
/* cout<<"The string to be tokenized is:\n"
  <<sentence<<"\n\nThe tokens are:\n\n"; */
 
 char *tokenPtr=strtok(sentence," "); 
 
 while(tokenPtr!=NULL) {
    string str=tokenPtr;
  tokenPtr=strtok(NULL," ");  
  cout<<str<<endl;
} 
 
// cout<<"After strtok, sentence = "<<sentence<<endl; 
 return 0; 
}
