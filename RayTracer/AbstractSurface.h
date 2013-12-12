#ifndef __ABSTRACTSURFACE
#define __ABSTRACTSURFACE

#include "Ray.h"

class AbstractSurface{

    public:
        AbstractSurface(){}
        ~AbstractSurface(){}

        virtual float checkIntersection(Ray ray){
        }

    private:
};

#endif
