#ifndef __TRIANGLE
#define __TRIANGLE

#include "GenVector.h"
#include "AbstractSurface.h"

class Triangle : public AbstractSurface{

    public:
        Triangle(){}

        ~Triangle(){}

        Triangle(Vector3 p1, Vector3 p2, Vector3 p3){
            this->p1 = p1;
            this->p2 = p2;
            this->p3 = p3;

            Vector3 v1 = p2 - p1;
            Vector3 v2 = p3 - p2;

            this->normal = v1.cross(v2);
        }

        bool checkIntersection(Ray ray){
            if(checkRayHittingPlane(ray)){
                return checkHitpointInTriangle(ray);
            }
        
        }

    private:
        Vector3 p1;
        Vector3 p2;
        Vector3 p3;

        Vector3 normal;

        float checkRayHittingPlane(Ray ray){
            Vector3 a = this->p1;

            float top = this->normal.dot((a - ray.getOrigin()));

            return false;
        }

        bool checkHitpointInTriangle(Ray ray){
            

            return false;
        }

};

#endif
