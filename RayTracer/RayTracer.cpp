#include "GenVector.h"
#include "simplePPM.h"
#include "Buffer.h"
#include "objLoader.h"
#include "Camera.h"
#include "RayGenerator.h"
#include <math.h>
#include <vector>
#include <memory>
#include "AbstractSurface.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Hitpoint.h"
#include "Scene.h"

template<typename shape>
Hitpoint* genericGetHitpoint(std::vector<shape>* shapes, Ray* newRay);
template<typename shape>
Hitpoint* genericGetHitpoint(std::vector<shape*>* shapes, Ray* newRay);
Hitpoint* getPriorityHitpoint(Scene* scene, Ray* ray);
Scene loadScene(objLoader* objData);

int main(int argc, char ** argv)
{
	//buffer
    int width =  800;
    int height = 800;
	Buffer buf = Buffer(width, height);

	//need at least one argument (obj file)
	if(argc < 2)
	{
		printf("Usage %s filename.obj\n", argv[0]);
		exit(0);
	}

	//load camera data
	objLoader objData = objLoader();
	objData.load(argv[1]);
    
    Scene scene = loadScene(&objData);
    printf("Scene loaded correctly\n");
    
    printf("Before Ray generation\n");
    RayGenerator rayGen = RayGenerator(scene.getCamera(), width, height, 90.0);
    for(int i = 0; i<width; i++){
        for(int k = height-1; k>=0; k--){
            Ray newRay = rayGen.getRay(k, i);
            Vector3 rayDirec = newRay.getDirection();
            
            float r = abs(rayDirec[0]*255);
            float g = abs(rayDirec[1]*255);
            float b = abs(rayDirec[2]*255);
            int rCast = (int)r;
            int gCast = (int)g;
            int bCast = (int)b;
            
            Color rayDirectionColor = Color(rCast, gCast, bCast);
            Hitpoint* hit = getPriorityHitpoint(&scene, &newRay);
            if(hit != NULL){
                buf.at(k, height-i) = Color(0, 0, 0);
            } else {
                buf.at(k, height-i) = rayDirectionColor;
            }
        }
    }
    printf("Writing file\n");
	simplePPM_write_ppm("test.ppm", buf.getWidth(), buf.getHeight(), (unsigned char*)&buf.at(0,0));
	return 0;
}

template<typename shape>
Hitpoint* genericGetHitpoint(std::vector<shape>* shapes, Ray* newRay){
    float smallestIntersected = -1.0;
    int index = -1;
    for(int j = 0; j<(*shapes).size(); j++){
        shape current = (*shapes)[j];
        float intersected = current.checkIntersection(*newRay);
        if(intersected>=0){
            if(intersected < smallestIntersected || smallestIntersected == -1.0){
                smallestIntersected = intersected;
                index = j;
            }
        }
    }
    Hitpoint hit;
    if(index>=0){
        hit = Hitpoint(*newRay, smallestIntersected);
        return &hit;
    } 
    return NULL;

}

template<typename shape>
Hitpoint* genericGetHitpoint(std::vector<shape*>* shapes, Ray* newRay){
    float smallestIntersected = -1.0;
    int index = -1;
    for(int j = 0; j<(*shapes).size(); j++){
        shape* current = (*shapes)[j];
        float intersected = current->checkIntersection(*newRay);
        if(intersected>=0){
            if(intersected < smallestIntersected || smallestIntersected == -1.0){
                smallestIntersected = intersected;
                index = j;
            }
        }
    }
    Hitpoint hit;
    if(index>=0){
        hit = Hitpoint(*newRay, smallestIntersected);
        return &hit;
    } 
    return NULL;

}

Hitpoint* getPriorityHitpoint(Scene* scene, Ray* ray){
    Hitpoint* hitpoint = genericGetHitpoint((*scene).getSurfaces(), ray);
    
    if(hitpoint != NULL){
        return hitpoint;
    } else {
        return NULL;
    }
}

Scene loadScene(objLoader* objData){
    Camera camera;
    
    std::vector<AbstractSurface*> surfaces = std::vector<AbstractSurface*>();
    
	if((*objData).camera != NULL)
	{
        float x = (*objData).vertexList[ (*objData).camera->camera_pos_index ]->e[0];
        float y = (*objData).vertexList[ (*objData).camera->camera_pos_index ]->e[1];
        float z = (*objData).vertexList[ (*objData).camera->camera_pos_index ]->e[2];
        
        Vector3 camPos = Vector3(x, y, z);

        float xLook = (*objData).vertexList[ (*objData).camera->camera_look_point_index ]->e[0];
		float yLook = (*objData).vertexList[ (*objData).camera->camera_look_point_index ]->e[1];
		float zLook = (*objData).vertexList[ (*objData).camera->camera_look_point_index ]->e[2];

        Vector3 lookAt = Vector3(xLook, yLook, zLook);

		float xUp =	(*objData).normalList[ (*objData).camera->camera_up_norm_index ]->e[0];
		float yUp =	(*objData).normalList[ (*objData).camera->camera_up_norm_index ]->e[1];
		float zUp = (*objData).normalList[ (*objData).camera->camera_up_norm_index ]->e[2];
	
        Vector3 up = Vector3(xUp, yUp, zUp);
        up.normalize();

        camera = Camera(&camPos, &lookAt, &up);

		printf("Found a camera\n");
		printf(" position: ");
		printf("%f %f %f\n", x, y, z);
		printf(" looking at: ");
		printf("%f %f %f\n", lookAt[0], lookAt[1], lookAt[2]);
		printf(" up normal: ");
		printf("%f %f %f\n", xUp, yUp, zUp);
	}

    std::vector<Sphere> spheres = std::vector<Sphere>();
    if((*objData).sphereCount > 0 && (*objData).sphereList != NULL){
        for(int i = 0; i < (*objData).sphereCount; i++){
            float sphereX =  (*objData).vertexList[ (*objData).sphereList[i]->pos_index ]->e[0];
            float sphereY =  (*objData).vertexList[ (*objData).sphereList[i]->pos_index ]->e[1];
            float sphereZ =  (*objData).vertexList[ (*objData).sphereList[i]->pos_index ]->e[2];

            Vector3 spherePos = Vector3(sphereX, sphereY, sphereZ);

            float xUp = (*objData).normalList [ (*objData).sphereList[i]->up_normal_index ]->e[0];
            float yUp = (*objData).normalList [ (*objData).sphereList[i]->up_normal_index ]->e[1];
            float zUp = (*objData).normalList [ (*objData).sphereList[i]->up_normal_index ]->e[2];

            Vector3 sphereUp = Vector3(xUp, yUp, zUp);

            float radius = sphereUp.length();

            Sphere* sphere = new Sphere(spherePos, radius);
            surfaces.push_back(sphere);
        }
    }
    std::vector<Triangle> triangles = std::vector<Triangle>();
    if((*objData).faceCount > 0 && (*objData).faceList != NULL){
        for(int i = 0; i < (*objData).faceCount; i++){
            float p1X = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[0] ]->e[0];
            float p1Y = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[0] ]->e[1];
            float p1Z = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[0] ]->e[2];

            float p2X = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[1] ]->e[0];
            float p2Y = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[1] ]->e[1];
            float p2Z = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[1] ]->e[2];

            float p3X = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[2] ]->e[0];
            float p3Y = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[2] ]->e[1];
            float p3Z = (*objData).vertexList[ (*objData).faceList[i]->vertex_index[2] ]->e[2];

            Vector3 p1 = Vector3(p1X, p1Y, p1Z);
            Vector3 p2 = Vector3(p2X, p2Y, p2Z);
            Vector3 p3 = Vector3(p3X, p3Y, p3Z);

            Triangle* tri = new Triangle(p1, p2, p3);
            surfaces.push_back(tri);
        }
    }
    return Scene(camera, surfaces);
}

