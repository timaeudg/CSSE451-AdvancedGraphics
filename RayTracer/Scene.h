#ifndef __SCENE
#define __SCENE

#include "Sphere.h"
#include "Triangle.h"
#include <vector>

class Scene{

    public:
        Scene(){}
        
        Scene(Camera cam, std::vector<AbstractSurface*> surfaces){
            this->cam = cam;
            this->surfaces = surfaces;
        }
        
        Camera* getCamera(){
            return &this->cam;
        }
        
        std::vector<AbstractSurface*>* getSurfaces(){
            return &this->surfaces;
        }
        
        ~Scene(){}

    private:
        Camera cam;
        std::vector<AbstractSurface*> surfaces;
};

#endif
