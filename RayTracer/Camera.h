#ifndef __CAMERA
#define __CAMERA

#include "GenVector.h"

class Camera{

    public:
        Camera(){
            setBasis(Vector3(0), Vector3(0), Vector3(0));
        }

        Camera(Vector3 position, Vector3 look, Vector3 up){
            setBasis(position, look, up);
        }

        void setBasis(Vector3 pos, Vector3 lookAtPoint, Vector3 up){
            this->position = pos;
            Vector3 lookAtVector = lookAtPoint - this->position;
            this->w = -lookAtVector;
            this->u = cross(w, up);
            this->v = cross(w, u);
        }

    private:
        Vector3 position;
        Vector3 w;
        Vector3 u;
        Vector3 v;

}
