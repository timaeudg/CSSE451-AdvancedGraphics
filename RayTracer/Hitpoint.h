#ifndef __HITPOINT
#define __HITPOINT

#include "Ray.h"
#include "GenVector.h"

class Hitpoint{

    public:
        Hitpoint(){}
        ~Hitpoint(){}

        Hitpoint(Ray ray, float parameterVal){
            this->parameterVal = parameterVal;
            this->ray = ray;
        }
        
        float getParameter(){
            return this->parameterVal;
        }
        
        Ray* getRay(){
            return &this->ray;
        }
        
        Vector3 getHitpoint(){
            return this->ray.getOrigin() + this->parameterVal*this->ray.getDirection();
        }
        
    private:
        float parameterVal;
        Ray ray;
};

#endif
