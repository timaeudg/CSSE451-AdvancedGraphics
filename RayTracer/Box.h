#ifndef __BOX
#define __BOX

#include "AbstractSurface.h"
#include "GenVector.h"

public Box : public AbstractSurface{

    public:
        Box(){
            this->min = Vector3(0,0,0);
            this->max = Vector3(0,0,0);
        }

        Box(Vector3 min, Vector3 max){
            this->min = min;
            this->max = max;
        }

        ~Box(){}
        
        Vector3 getBBMin(){
            return this->min;
        }
        
        Vector3 getBBMax(){
            return this->max;
        }
        
        Vector3 getCenterPoint(){
            return Vector3((this->min[0]+this->max[0])/2.0f, (this->min[1]+this->max[1])/2.0f, (this->min[2]+this->max[2])/2.0f
        }
        
        void expandBox(AbstractSurface& shape){
            Vector3 shapeMax = shape.getBBMax();
            Vector3 shapeMin = shape.getBBMin();
            
            if(shapeMax[0] > max[0]){
                max[0] = shapeMax[0];
            }
            if(shapeMax[1] > max[1]){
                max[1] = shapeMax[1];
            }
            if(shapeMax[2] > max[2]){
                max[2] = shapeMax[2];
            }
            if(shapeMin[0] > min[0]){
                min[0] = shapeMin[0];
            }
            if(shapeMin[1] > min[1]){
                min[1] = shapeMin[1];
            }
            if(shapeMin[2] > min[2]){
                min[2] = shapeMin[2];
            }
        }
        
        Vector3 getAxisLengths(){
            Vector3 axisVector = Vector(0,0,0);
            axisVector[0] = max[0]-min[0];
            axisVector[1] = max[1]-min[1];
            axisVector[2] = max[2]-min[2];
            return axisVector;
        }

    private:
        Vector3 max;
        Vector3 min;

};

#endif
