# 1 "a0012testTemplate.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "a0012testTemplate.c"
# 16 "a0012testTemplate.c"
# 1 "a0012testTemplate.h" 1
# 16 "a0012testTemplate.h"
template<typename T1>
class Temp{
public:
    Temp(){}
    ~Temp(){}

    T1 add(T1 x, T1 y);

};


# 1 "a0012testTemplate.c" 1
# 16 "a0012testTemplate.c"
# 1 "a0012testTemplate.h" 1
# 17 "a0012testTemplate.c" 2

template<typename T1>
T1 Temp<T1>:: add(T1 x, T1 y){
    return x + y;

}
# 28 "a0012testTemplate.h" 2
# 17 "a0012testTemplate.c" 2

template<typename T1>
T1 Temp<T1>:: add(T1 x, T1 y){
    return x + y;

}
