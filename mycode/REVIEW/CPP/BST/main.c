#include"BsTree.h"
int main(){
    T mytree=TreeCreate();
    TreeInsert(mytree,5);

    printf("\nstart\n");
    //TreeInsert(mytree,4);
    //TreeInsert(mytree,75);
    //TreeInsert(mytree,2);
    //TreeInsert(mytree,15);
    TN target=mytree->root;
    printf("root=%d",target->key);
    /*for(int i=0;i<5;++i){
        printf("%d,",target->key);
        if(target->left)
            target=target->left;
        else if(target->right)
            target=target->left;
        else{
             break;
        }
    }*/
    printf("\nfinish\n");
}
