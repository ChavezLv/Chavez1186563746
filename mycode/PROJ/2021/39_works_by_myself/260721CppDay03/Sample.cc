#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Sample {
public:
    
    Sample();
    void display();

private:
    int i;
    static int k;

};

Sample::Sample(){
    i = 0;
    k++;
}

void Sample::display(){
    cout << "i = " << i 
         << "k = " << k
         << endl;
}

int Sample::k = 0;
int main()
{
    Sample a, b;
    a.display();
    b.display();
    return 0;
}

