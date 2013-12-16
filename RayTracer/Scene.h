#ifndef __SCENE
#define __SCENE

#include "Sphere.h"
#include "Triangle.h"
#include "Material.h"
#include "Light.h"
#include <vector>

class Scene{

    public:
        Scene(){}
        
        Scene(Camera cam, std::vector<AbstractSurface*> surfaces, std::vector<Material> mats, std::vector<Light*> lights){
            this->cam = cam;
            this->surfaces = surfaces;
            this->materials = mats;
            this->lights = lights;
        }
        
        Camera* getCamera(){
            return &this->cam;
        }
        
        std::vector<AbstractSurface*>* getSurfaces(){
            return &this->surfaces;
        }

        Material getMaterial(int index){
            return this->materials[index];
        }

        std::vector<Light*> getLights(){
            return this->lights;
        }
        
        ~Scene(){}

    private:
        Camera cam;
        std::vector<AbstractSurface*> surfaces;
        std::vector<Material> materials;
        std::vector<Light*> lights;
};

#endif
