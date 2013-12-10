#ifndef __ABSTRACTSURFACE
#define __ABSTRACTSURFACE

#include "Ray.h"

class AbstractSurface{

    public:
        AbstractSurface(){}
        ~AbstractSurface(){}

        virtual bool checkIntersection(Ray ray){
            return false;
        }

    private:
};

#endif
