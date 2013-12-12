#ifndef __MATERIAL
#define __MATERIAL

#include "GenVector.h"
#include "AbstractSurface.h"

class Material {

    public:
        Material(){}
        ~Material(){}
        
        Material(Color amb, Color diff, Color spec){
            this->ambColor = amb;
            this->diffColor = diff;
            this->specColor = spec;
        }
        
    private:
        Color ambColor;
        Color diffColor;
        Color specColor;
};

#endif
