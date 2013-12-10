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

        bool checkIntersection(Ray ray){
            float A = ray.getDirection().dot(ray.getDirection());
            float B = 2*ray.getDirection().dot((ray.getOrigin() - centerPoint));
            float C = (ray.getOrigin() - centerPoint).dot((ray.getOrigin() - centerPoint)) - (radius*radius);

            float rootPart = B*B - 4*A*C;

            if(rootPart >= 0){
                //Do more stuff because we probably want the hitpoint, but for now, just return true
                return true;
            } else {
                return false;
            }
        }

    private:
        float radius;
        Vector3 centerPoint;
};
#endif
