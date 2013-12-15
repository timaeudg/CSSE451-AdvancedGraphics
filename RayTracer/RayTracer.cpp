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
#include "Material.h"
#include <cfloat>

template<typename shape>
bool genericGetHitpoint(std::vector<shape*>* shapes, Ray* newRay, float* param, int* index);
//Hitpoint* getPriorityHitpoint(Scene* scene, Ray* ray);
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
            Ray newRay = rayGen.getRay(i, k);
            Vector3 rayDirec = newRay.getDirection();
            
            float r = abs(rayDirec[0]*255);
            float g = abs(rayDirec[1]*255);
            float b = abs(rayDirec[2]*255);
            int rCast = (int)r;
            int gCast = (int)g;
            int bCast = (int)b;
            
            float paramVal = -1.0;
            int index = -1;
            Color rayDirectionColor = Color(rCast, gCast, bCast);
            bool hit = genericGetHitpoint(scene.getSurfaces(), &newRay, &paramVal, &index);
            if(hit){
                Vector3 hitLoc = newRay.pointAtParameterValue(paramVal);
                Hitpoint hit = Hitpoint(newRay, paramVal, (*(scene.getSurfaces()))[index]);
                Vector3 norm = hit.getSurface()->getNormal(hit.getHitpoint());
                //Vector3 norm = hit->getNormal();
                norm = norm*255.0f;
                //printf("multied color: %i,%i,%i\n", abs((int)norm[0]), abs((int)norm[1]), abs((int)norm[2]));
                Color n = Color(abs((int)norm[0]), abs((int)norm[1]), abs((int)norm[2]));
                //printf("about to colorize: %f,%f,%f\n", n[0], n[1], n[2]);
                buf.at(i, height-k) = n;
            } else{
                //buf.at(i, height-k) = Color((int)abs(rayDirec[0]*255.0f), (int)abs(rayDirec[1]*255.0f), (int)abs(rayDirec[2]*255.0f));
                buf.at(i, height-k) = Color(0, 0, 0);
            }
        }
    }
    printf("Writing file\n");
	simplePPM_write_ppm("test.ppm", buf.getWidth(), buf.getHeight(), (unsigned char*)&buf.at(0,0));
	return 0;
}

template<typename shape>
bool genericGetHitpoint(std::vector<shape*>* shapes, Ray* newRay, float* intersected, int* shapeIndex){
    float smallestIntersected = FLT_MAX;
    int index = -1;
    for(int j = 0; j<(*shapes).size(); j++){
        shape* current = (*shapes)[j];
        float intersected = current->checkIntersection(*newRay);
        if(intersected>=0){
            printf("intersected after return: %f\n", intersected);
            if(intersected < smallestIntersected){
                printf("replaced\n");
                smallestIntersected = intersected;
                index = j;
            }
        }
    }
    if(index>=0){
        *intersected = smallestIntersected;
        *shapeIndex = index;
        return true;
    } 
    return false;

}
/*
Hitpoint* getPriorityHitpoint(Scene* scene, Ray* ray){
    Hitpoint* hitpoint = genericGetHitpoint((*scene).getSurfaces(), ray);
//    printf("hitpoint norm check: %f,%f,%f\n", hitpoint->getNormal()[0], hitpoint->getNormal()[1], hitpoint->getNormal()[2]); 
    if(hitpoint != NULL){
        return hitpoint;
    } else {
        return NULL;
    }
}
*/
Scene loadScene(objLoader* objData){
    Camera camera;
    std::vector<Material> materials = std::vector<Material>();
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
    
    if((*objData).materialCount > 0 && (*objData).materialList != NULL){
        for(int j = 0; j < (*objData).materialCount; j++){
            float ambR = (*objData).materialList[ j ]->amb[0];
            float ambG = (*objData).materialList[ j ]->amb[1];
            float ambB = (*objData).materialList[ j ]->amb[2];
            
            float diffR = (*objData).materialList[ j ]->diff[0];
            float diffG = (*objData).materialList[ j ]->diff[1];
            float diffB = (*objData).materialList[ j ]->diff[2];
            
            float specR = (*objData).materialList[ j ]->spec[0];
            float specG = (*objData).materialList[ j ]->spec[1];
            float specB = (*objData).materialList[ j ]->spec[2];
            
            Color ambColor = Color((int)abs(ambR*255), (int)abs(ambG*255), (int)abs(ambB*255));
            Color diffColor = Color((int)abs(diffR*255), (int)abs(diffG*255), (int)abs(diffB*255));
            Color specColor = Color((int)abs(specR*255), (int)abs(specG*255), (int)abs(specB*255));
            
            Material mat = Material(ambColor, diffColor, specColor);
            materials.push_back(mat);
        }
    }
    
    return Scene(camera, surfaces, materials);
}

