#ifndef __SCENE
#define __SCENE

#include "Sphere.h"
#include "Triangle.h"
#include "Material.h"
#include <vector>

class Scene{

    public:
        Scene(){}
        
        Scene(Camera cam, std::vector<AbstractSurface*> surfaces, std::vector<Material> mats){
            this->cam = cam;
            this->surfaces = surfaces;
            this->materials = mats;
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
        std::vector<Material> materials;
};

#endif
