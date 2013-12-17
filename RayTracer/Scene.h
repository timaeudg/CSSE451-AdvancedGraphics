#ifndef __SCENE
#define __SCENE

#include "Sphere.h"
#include "Triangle.h"
#include "Material.h"
#include "Light.h"
#include <vector>
#include <cfloat>

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
        
        bool getHitpoint(Ray* newRay, float* intersected, int* shapeIndex){
            float smallestIntersected = FLT_MAX;
            int index = -1;
            for(int j = 0; j<surfaces.size(); j++){
                AbstractSurface* current = surfaces[j];
                float intersected = current->checkIntersection(*newRay);
                if(intersected>=0){
                    if(intersected < smallestIntersected){
                        smallestIntersected = intersected;
                        index = j;
                    }
                }
            }
            if(index>=0){
                *intersected = smallestIntersected;
                *shapeIndex = index;
                return true;
            } 
            return false;
        }
        
        ~Scene(){}

    private:
        Camera cam;
        std::vector<AbstractSurface*> surfaces;
        std::vector<Material> materials;
        std::vector<Light*> lights;
};

#endif
