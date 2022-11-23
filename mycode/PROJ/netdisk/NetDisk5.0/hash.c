#include "head.h"
#include "hash.h"
#include "math.h"
int  hash(int peerfd){
    int key=peerfd+(int)sqrt(peerfd)*123456;
    key=key%31;
    return key;
}

hashnode* CreateHashTable(){
   
}
int  HashTableADD(int peerfd){
}
