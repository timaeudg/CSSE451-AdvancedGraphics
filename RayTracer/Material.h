#ifndef __MATERIAL
#define __MATERIAL

#include "GenVector.h"
#include "AbstractSurface.h"

class Material {

    public:
        Material(){}
        ~Material(){}
        
        Material(Vector3 amb, Vector3 diff, Vector3 spec){
            this->ambColor = amb;
            this->diffColor = diff;
            this->specColor = spec;
        }

        Vector3 getAmbColor(){
            return this->ambColor;
        }

        Vector3 getDiffColor(){
            return this->diffColor;
        }

        Vector3 getSpecColor(){
            return this->specColor;
        }
        
    private:
        Vector3 ambColor;
        Vector3 diffColor;
        Vector3 specColor;
};

#endif
