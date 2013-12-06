#include "GenVector.h"
#include "simplePPM.h"
#include "Buffer.h"
#include "objLoader.h"
#include "Camera.h"
#include "RayGenerator.h"
#include <math.h>
#include <vector>
#include "Sphere.h"

int main(int argc, char ** argv)
{
	//buffer
    int width =  800;
    int height = 800;
	Buffer buf = Buffer(height, width);

	//need at least one argument (obj file)
	if(argc < 2)
	{
		printf("Usage %s filename.obj\n", argv[0]);
		exit(0);
	}

	//load camera data
	objLoader objData = objLoader();
	objData.load(argv[1]);

    Camera cam;
    bool camFound = false;

	if(objData.camera != NULL)
	{
        float x = objData.vertexList[ objData.camera->camera_pos_index ]->e[0];
        float y = objData.vertexList[ objData.camera->camera_pos_index ]->e[1];
        float z = objData.vertexList[ objData.camera->camera_pos_index ]->e[2];
        
        Vector3 camPos = Vector3(x, y, z);

        float xLook = objData.vertexList[ objData.camera->camera_look_point_index ]->e[0];
		float yLook = objData.vertexList[ objData.camera->camera_look_point_index ]->e[1];
		float zLook = objData.vertexList[ objData.camera->camera_look_point_index ]->e[2];

        Vector3 lookAt = Vector3(xLook, yLook, zLook);

		float xUp =	objData.normalList[ objData.camera->camera_up_norm_index ]->e[0];
		float yUp =	objData.normalList[ objData.camera->camera_up_norm_index ]->e[1];
		float zUp = objData.normalList[ objData.camera->camera_up_norm_index ]->e[2];
	
        Vector3 up = Vector3(xUp, yUp, zUp);
        up.normalize();

        cam = Camera(&camPos, &lookAt, &up);
        camFound = true;

		printf("Found a camera\n");
		printf(" position: ");
		printf("%f %f %f\n", x, y, z);
		printf(" looking at: ");
		printf("%f %f %f\n", lookAt[0], lookAt[1], lookAt[2]);
		printf(" up normal: ");
		printf("%f %f %f\n", xUp, yUp, zUp);
	}

    std::vector<Sphere> spheres = std::vector<Sphere>();

    if(objData.sphereCount > 0 && objData.sphereList != NULL){
        for(int i = 0; i < objData.sphereCount; i++){
            float sphereX =  objData.vertexList[ objData.sphereList[i]->pos_index ]->e[0];
            float sphereY =  objData.vertexList[ objData.sphereList[i]->pos_index ]->e[1];
            float sphereZ =  objData.vertexList[ objData.sphereList[i]->pos_index ]->e[2];

            Vector3 spherePos = Vector3(sphereX, sphereY, sphereZ);

            float xUp = objData.normalList [ objData.sphereList[i]->up_normal_index ]->e[0];
            float yUp = objData.normalList [ objData.sphereList[i]->up_normal_index ]->e[1];
            float zUp = objData.normalList [ objData.sphereList[i]->up_normal_index ]->e[2];

            Vector3 sphereUp = Vector3(xUp, yUp, zUp);

            float radius = sphereUp.length();

            Sphere sphere = Sphere(spherePos, radius);
            spheres.push_back(sphere);
        }
    }
    
    printf("Before Ray generation\n");
    if(camFound){
        RayGenerator rayGen = RayGenerator(&cam, width, height, 90.0);
        for(int i = height-1; i>=0; i--){
            for(int k = 0; k<width; k++){
                Ray newRay = rayGen.getRay(k, i);
                Vector3 rayDirec = newRay.getDirection();
                float r = abs(rayDirec[0]*255);
                float g = abs(rayDirec[1]*255);
                float b = abs(rayDirec[2]*255);
                int rCast = (int)r;
                int gCast = (int)g;
                int bCast = (int)b;
                Color rayDirectionColor = Color(rCast, gCast, bCast);

                for(int j = 0; j<spheres.size(); j++){
                    Sphere currentSphere = spheres[j];
                    bool intersected = currentSphere.checkIntersection(newRay);
                    if(intersected){
                        rayDirectionColor = Color(0, 0, 0);
                    }
                }


                buf.at(k, height - i) = rayDirectionColor;
            }
        }
    }

	simplePPM_write_ppm("test.ppm", buf.getWidth(), buf.getHeight(), (unsigned char*)&buf.at(0,0));
	return 0;
}
