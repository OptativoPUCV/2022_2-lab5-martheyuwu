 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *tree=(TreeMap*)malloc(sizeof(TreeMap));
    tree->root=NULL;
    tree->current=NULL;
    tree->lower_than=lower_than;
    //new->lower_than = lower_than;
    return tree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (searchTreeMap(tree,key)==NULL){
      TreeNode *aux=(TreeNode *)malloc(sizeof(TreeNode));
      TreeNode *nodo=(TreeNode *)malloc(sizeof(TreeNode));
      nodo=createTreeNode(key,value);
      aux=tree->root;
      while (1){
        int result=tree->lower_than(key,aux->pair->key);
        if (result==1){
          if (aux->left==NULL){
            aux->left=nodo;
            aux->left->parent=aux;
            tree->current=nodo;
            break;
          }
          aux=aux->left;
        }
        else{
          if (aux->right==NULL){
            aux->right=nodo;
            aux->right->parent=aux;
            tree->current=nodo;
            break;
          }
          aux=aux->right;
        }
      }
    }
}

TreeNode * minimum(TreeNode * x){
    while (1){
      if (x->left==NULL)
        return x;
      x=x->left;
    }
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left==NULL && node->right==NULL){
      if (node==node->parent->left)
         node->parent->left=NULL;
      if (node==node->parent->right)
        node->parent->right=NULL;
    }
    else{
      if (node->left!=NULL && node->right==NULL){
        if (node==node->parent->left){
          node->parent->left=node->left;
          node->left->parent=node->parent;
        }
        if (node==node->parent->right){
          node->parent->right=node->left;
          node->left->parent=node->parent;
        }
      }
      if (node->left==NULL && node->right!=NULL){
        if (node==node->parent->left){
          node->parent->left=node->right;
          node->right->parent=node->parent;
        }
        if (node==node->parent->right){
          node->parent->right=node->right;
          node->right->parent=node->parent;
        }
      }
      if (node->left!=NULL && node->right!=NULL){
        TreeNode *minimo=(TreeNode *)malloc(sizeof(TreeNode));
        minimo=minimum(node->right);
        node->pair=minimo->pair;
        if (minimo!=NULL)
          removeNode(tree,minimo);
      }        
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux=(TreeNode *)malloc(sizeof(TreeNode));
    aux=tree->root;
    while (1){
      if (aux==NULL)
        return NULL;
      if (is_equal(tree,key,aux->pair->key)==1){
        tree->current=aux;
        return aux->pair;
      }
      int result=tree->lower_than(key,aux->pair->key);
      if (result==1)
        aux=aux->left;
      else
        aux=aux->right;
    }
}


Pair * upperBound(TreeMap * tree, void* key) {
    int result,auxint=0,menorkey;
    Pair *search=(Pair *)malloc(sizeof(Pair));
    Pair *menorpair=(Pair *)malloc(sizeof(Pair));
    search=searchTreeMap(tree,key);
    if (search!=NULL)
      return search;
    TreeNode *aux=(TreeNode *)malloc(sizeof(TreeNode));
    aux=tree->root;
    void *menor=aux->pair->key;
    while (1){
      if (aux==NULL)
        break;
      result=tree->lower_than(key,aux->pair->key);
      menorkey=tree->lower_than(aux->pair->key,menor);
      if (result==1 && auxint==0){
        menor=aux->pair->key;
        menorpair=aux->pair;
        auxint=1;
      }
      if (result==1 && menorkey==1){ 
        menor=aux->pair->key;
        menorpair=aux->pair;
      }
      if (result==1)
        aux=aux->left;
      else
        aux=aux->right;
    }
    if (auxint==0)
      return NULL;
    return menorpair;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *aux=(TreeNode *)malloc(sizeof(TreeNode));
    aux=tree->root;
    while (1){
      if (aux->left==NULL)
        return aux->pair;
      aux=aux->left;
    }
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux=(TreeNode *)malloc(sizeof(TreeNode));
    int resultado;
    aux=tree->current;
    if (tree->current->right!=NULL){
      if (aux->right->left!=NULL){
        tree->current=aux->right->left;
        return tree->current->pair;
      }
      tree->current=aux->right;
      return tree->current->pair;
    }
    else{
      while (1){
        if (aux->parent==NULL)
          return NULL;
        resultado=tree->lower_than(aux->pair->key,aux->parent->pair->key);
        if (resultado==1){
          tree->current=aux->parent;
          return aux->parent->pair;
        }
        aux=aux->parent;
      }
    }
}
