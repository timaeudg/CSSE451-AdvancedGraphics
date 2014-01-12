#ifndef __AABBNODE
#define __AABBNODE

#include <vector>

class AABBNode{

public:
    AABBNode(){
        
    }
    
    AABBNode(std::vector<AbstractSurface*> surfaces){
        
    }
    
    ~AABBNode(){}

private:
    std::vector<int> indeces = std::vector<int>();
};
#endif