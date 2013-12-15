#ifndef __HITPOINT
#define __HITPOINT

#include "Ray.h"
#include "GenVector.h"

class Hitpoint{

    public:
        Hitpoint(){}
        ~Hitpoint(){}

        Hitpoint(Ray ray, float parameterVal, AbstractSurface* surface){
            this->parameterVal = parameterVal;
            this->ray = ray;
            this->hitSurface = surface;
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

        AbstractSurface* getSurface(){
            return this->hitSurface;
        }
        
        Vector3 getNormal(){
            return getSurface()->getNormal(getHitpoint());
            /*
            printf("getting hitloc\n");
            Vector3 hitLocation = getHitpoint();

            printf("getting p1\n");
            this->hitSurface->getP1();

            printf("getting normal\n");
            if(this->hitSurface ==NULL){
                printf("Why is hitsurface null???\n");
            }
            Vector3 normal = (this->hitSurface)->getNormal(hitLocation);
            return normal;
            */
        }
        
    private:
        float parameterVal;
        Ray ray;
        AbstractSurface* hitSurface;
};

#endif
