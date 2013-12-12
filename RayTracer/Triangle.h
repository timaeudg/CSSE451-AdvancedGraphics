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

        float checkIntersection(Ray ray){
            if(checkRayHittingPlane(ray)){
                return checkHitpointInTriangle(ray);
            }
        
        }
        
        Vector3 getNormal(Vector3 hitpoint){
            printf("triangle normal: %f, %f, %f\n", this->normal[0], this->normal[1], this->normal[2]);
            return this->normal;
        }

    private:
        Vector3 p1;
        Vector3 p2;
        Vector3 p3;

        Vector3 normal;

        bool checkRayHittingPlane(Ray ray){
            Vector3 a = this->p1;

            float top = this->normal.dot((a - ray.getOrigin()));
            float bottom = ray.getDirection().dot(this->normal);
            if(bottom == 0){
                //The ray is parallel to the plane, so no intersection
                return false;
            }

            return true;
        }

        float checkHitpointInTriangle(Ray ray){
            float top = this->normal.dot((this->p1 - ray.getOrigin()));
            float bottom = ray.getDirection().dot(this->normal);

            float intersectParam = top/bottom;
            
            if(intersectParam < 0){
                return -1.0;
            }

            Vector3 rayPoint = ray.getOrigin() + intersectParam*ray.getDirection();

            float testVal1 = ((p2-p1).cross(rayPoint-p1)).dot(this->normal);
            float testVal2 = ((p3-p2).cross(rayPoint-p2)).dot(this->normal);
            float testVal3 = ((p1-p3).cross(rayPoint-p3)).dot(this->normal);

            if(testVal1>0 && testVal2>0 && testVal3>0){
                return intersectParam;
            }

            return -1.0;
        }

};

#endif
