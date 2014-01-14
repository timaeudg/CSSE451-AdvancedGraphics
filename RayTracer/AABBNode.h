#ifndef __AABBNODE
#define __AABBNODE

#include <vector>

class AABBNode{

public:
    AABBNode(){
        
    }
    
    AABBNode(AbstractSurface* surface, AABBNode* left, AABBNode* right){
        this->surface = surface;
        this->left = left;
        this->right = right;
    }
    
    ~AABBNode(){}

private:
    AbstractSurface* surface;
    AABBNode* left;
    AABBNode* right;
};
#endif
