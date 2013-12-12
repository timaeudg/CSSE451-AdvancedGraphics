#ifndef __SPHERE
#define __SPHERE

#include "Ray.h"
#include "GenVector.h"
#include "AbstractSurface.h"

class Sphere : public AbstractSurface{

    public:
        Sphere(){}

        ~Sphere(){}

        Sphere(Vector3 pos, float radius){
            setRadius(radius);
            setCenterPoint(pos);
        }

        void setRadius(float radius){
            this->radius = radius;
        }

        void setCenterPoint(Vector3 pos){
            this->centerPoint = pos;
        }

        float checkIntersection(Ray ray){
            float A = ray.getDirection().dot(ray.getDirection());
            float B = 2*ray.getDirection().dot((ray.getOrigin() - centerPoint));
            float C = (ray.getOrigin() - centerPoint).dot((ray.getOrigin() - centerPoint)) - (radius*radius);

            float rootPart = B*B - 4*A*C;

            if(rootPart >= 0){
                //Do more stuff because we probably want the hitpoint, but for now, just return true
                float topPartPlus = -B+rootPart;
                float topPartMinus = -B-rootPart;
                
                float tPlus = topPartPlus / 2*A;
                float tMinus = topPartMinus / 2*A;
                
                float tFinal = 0.0;
                
                if(tPlus < tMinus){
                    tFinal = tPlus;
                } else{
                    tFinal = tMinus;
                }
                
                return tFinal;
            } else {
                return -1.0;
            }
        }
        
        Vector3 getNormal(Vector3 hitpoint){
            Vector3 normVector = (this->centerPoint - hitpoint).normalize();
            return normVector;
        }

    private:
        float radius;
        Vector3 centerPoint;
};
#endif
