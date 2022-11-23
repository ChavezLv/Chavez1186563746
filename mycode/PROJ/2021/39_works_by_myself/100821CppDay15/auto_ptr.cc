#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

using std::auto_ptr;
using std::unique_ptr;
using std::shared_ptr;

/* void test(){ */
/*     int *pInt = new int(10); */

/*     /1* auto_ptr<int> ap(new int(10)); *1/ */
/*     auto_ptr<int> ap(pInt); */
/*     cout << *ap << endl; */
/*     cout << ap.get() << endl; */
/* } */

void test1(){
    unique_ptr<int> sp(new int(10));
    cout << *sp << endl;

    /* unique_ptr<int> sp1(sp);//error */                  

    unique_ptr<int> sp2(new int(11));
    /* sp2 = sp; //error */
      
}

void test2(){
    shared_ptr<int> sp(new int(10));
    cout << *sp << endl;
    cout << sp.get() << endl;
    cout << sp.use_count() << endl;
}

int main()
{
    test1();
    test2();
    return 0;
}

