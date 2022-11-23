#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class Solution {
public:

};
int main()
{
    int a[5] = {1, 2, 3, 4, 5};
    int *ptr = (int*) (&a + 1);
    printf("%d, %d", *(a+1), *(ptr-1));
    return 0;
}

