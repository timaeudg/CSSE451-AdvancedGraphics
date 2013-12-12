#ifndef __SCENE
#define __SCENE

#include "Sphere.h"
#include "Triangle.h"
#include <vector>

class Scene{

    public:
        Scene(){}
        
        Scene(Camera cam, std::vector<Sphere> spheres, std::vector<Triangle> triangles){
            this->cam = cam;
            this->spheres = spheres;
            this->triangles = triangles;
        }
        
        Camera* getCamera(){
            return &this->cam;
        }
        
        std::vector<Sphere>* getSpheres(){
            return &this->spheres;
        }
        
        std::vector<Triangle>* getTriangles(){
            return &this->triangles;
        }
        
        ~Scene(){}

    private:
        Camera cam;
        std::vector<Sphere> spheres;
        std::vector<Triangle> triangles;
};

#endif
