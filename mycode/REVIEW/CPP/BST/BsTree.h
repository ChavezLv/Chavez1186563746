#ifndef __BsTree_H__
#define __BsTree_H__
#include"queue.h"
typedef struct TreeNode_s{
    int key;
    P right;
    P left;
}TreeNode,*TN;

typedef struct Tree_s{
    TN root;
}Tree,*T;

T TreeCreate(){
    return (T)calloc(1,sizeof(Tree));
}
void TreeDestoy(T t){
    free(t);
}
void RecurInsert(TN node,TN newnode){
    if(node){//非空节点
        if(node->key>newnode->key){//left
            node=node->left;
            RecurInsert(node->left,newnode);//应该要递归
           
        }else{//right
            node=node->right;
            RecurInsert(node->right,newnode);
        }
    }else{//空节点
        node=newnode;
        return;
    }

}
void TreeInsert(T t,int val){
    //边界
    TN newnode=(TN)calloc(1,sizeof(TreeNode));
    newnode->key=val;
    RecurInsert(t->root,newnode);
}


#endif
