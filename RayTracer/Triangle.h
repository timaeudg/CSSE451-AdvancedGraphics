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

            Vector3 nor = v1.cross(v2);
            nor.normalize();
            printf("Triangle norm: %f,%f,%f\n", nor[0], nor[1], nor[2]);
            this->normal = nor;
        }

        float checkIntersection(Ray ray){
            if(checkRayHittingPlane(ray)){
                return checkHitpointInTriangle(ray);
            }
        
        }

        Vector3 getP1(){
            return this->p1;
        }
        
        Vector3 getNormal(Vector3 hitpoint){
            
            printf("triangle p1: %f, %f, %f\n", p1[0], p1[1], p1[2]);
            printf("triangle p2: %f, %f, %f\n", p2[0], p2[1], p2[2]);
            printf("triangle p3: %f, %f, %f\n", p3[0], p3[1], p3[2]);
            
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
