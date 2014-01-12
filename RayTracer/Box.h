#ifndef __BOX
#define __BOX

#include "AbstractSurface.h"
#include "GenVector.h"

public Box : public AbstractSurface{

    public:
        Box(){}

        Box(Vector3 min, Vector3 max){
            this->min = min;
            this->max = max;
        }

        ~Box(){}

    private:
        Vector3 max;
        Vector3 min;

};

#endif
