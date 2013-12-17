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
#include "Light.h"
#include "PointLight.h"
#include <cfloat>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Vector3 getColor(Ray &ray, Hitpoint &hit, Scene &scene, float paramVal, int reflectDepth);
bool traceShadowRay(Scene &scene, Ray &ray);
Scene loadScene(objLoader* objData);

int main(int argc, char ** argv)
{
    //buffer
    int width =  1920;
    int height = 1080;

    //need at least one argument (obj file)
    if(argc < 2)
    {
        printf("Usage %s filename.obj\n", argv[0]);
        exit(0);
    }

    if(argc >= 3){
        width = atoi(argv[2]);
        if(argc == 4){
            height = atoi(argv[3]);
        }
    }

    Buffer buf = Buffer(width, height);
    
    //load camera data
    objLoader objData = objLoader();
    objData.load(argv[1]);
    
    Scene scene = loadScene(&objData);
    printf("Scene loaded correctly\n");
    
    printf("Before Ray generation\n");
    RayGenerator rayGen = RayGenerator(scene.getCamera(), width, height, 90.0);
    for(int i = 0; i<width; i++){
        for(int k = 0; k<height; k++){
            Ray newRay = rayGen.getRay(i, height-1-k);

            float paramVal = -1.0;
            int index = -1;

            bool hit = scene.getHitpoint(&newRay, &paramVal, &index);
            if(hit){
                Hitpoint hit = Hitpoint(newRay, paramVal, (*(scene.getSurfaces()))[index]);
                
                Color pixelColor;
                Vector3 colorVector = getColor(newRay, hit, scene, paramVal, 5);
                pixelColor = Color(abs((int)colorVector[0]), abs((int)colorVector[1]), abs((int)colorVector[2]));
    
                buf.at(i, k) = pixelColor;
            } else{
                buf.at(i, k) = Color(0,0,0);
            }
        }
    }
    printf("Writing file\n");
    simplePPM_write_ppm("test.ppm", buf.getWidth(), buf.getHeight(), (unsigned char*)&buf.at(0,0));
    return 0;
}

Vector3 getColor(Ray &ray, Hitpoint &hit, Scene &scene, float paramVal, int reflectDepth){
    //grab hitpoint info
    Vector3 hitLoc = ray.pointAtParameterValue(paramVal);
    Vector3 norm = hit.getNormal();
    
    //grab material and material values
    Material mat = scene.getMaterial(hit.getSurface()->getMaterialIndex());

    Vector3 diff = mat.getDiffColor();
    Vector3 spec = mat.getSpecColor();
    Vector3 amb = mat.getAmbColor();

    Vector3 summedColor = Vector3(0,0,0);
    
    for(int k = 0; k < scene.getLights().size(); k++){
        Light* light = scene.getLights()[k];
        
        //Light Vectors and resources
        Vector3 lightDir = (light->getPos() - hitLoc).normalize();
        Material lightMat = scene.getMaterial(light->getMaterialIndex());
        Vector3 lr = (2*(lightDir.dot(norm))*norm - lightDir).normalize();
        Vector3 viewToCamera = (scene.getCamera()->getPos() - hitLoc).normalize();
 
        //Light shading values
        Vector3 lightAmbient = lightMat.getAmbColor();
        Vector3 lightDiffuse = lightMat.getDiffColor();
        Vector3 lightSpec = lightMat.getSpecColor();
         
        Vector3 ambientColor = (lightAmbient * amb);
        Vector3 diffColor = norm.dot(lightDir) * diff * lightDiffuse;
        /*
            The coefficent here comes from the material, need to load that
        */
        Vector3 specColor = (lightSpec * spec * pow((viewToCamera.dot(lr)), 10.0));
        

        //Shadow code
        Ray shadowRay = Ray(hit.getHitpoint(0.99f), lightDir);
        
        bool inShadow = traceShadowRay(scene, shadowRay);

        Vector3 combinedColor = ambientColor;
        if(!inShadow){
            combinedColor = combinedColor+diffColor+specColor;
        }

        //Reflection code
        Vector3 reflectColor = Vector3(0,0,0);
        int hitpointIndex = -1;
        float hitpointParam = -1.0;

        /*
         *How should I stop the recursion here? 
         *I could just do straight depth, but is there another way based off of light intensity???
         */
        Ray reflectionRay = Ray(hit.getHitpoint(0.99f), lr);
        bool hitSomething = scene.getHitpoint(&reflectionRay, &hitpointParam, &hitpointIndex);
        if(hitSomething && reflectDepth > 0){
            //printf("recursing\n");
            Hitpoint reflectHit = Hitpoint(reflectionRay, hitpointParam, (*(scene.getSurfaces()))[hitpointIndex]);
            reflectColor = getColor(reflectionRay, reflectHit, scene, hitpointParam, --reflectDepth);

        }

        combinedColor = combinedColor*10.0f;
        summedColor = summedColor + combinedColor;
    }
    return summedColor;
}

bool traceShadowRay(Scene &scene, Ray &ray){
    float intersected = -1.0;
    int index = -1;
    bool hitSomething = scene.getHitpoint(&ray, &intersected, &index);
    if(hitSomething){
        return true;
    }
    return false;
}

Scene loadScene(objLoader* objData){
    Camera camera;
    std::vector<Material> materials = std::vector<Material>();
    std::vector<AbstractSurface*> surfaces = std::vector<AbstractSurface*>();
    std::vector<Light*> lights = std::vector<Light*>();
    
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
            
            Vector3 ambColor  = Vector3( ambR,  ambG,  ambB);
            Vector3 diffColor = Vector3(diffR, diffG, diffB);
            Vector3 specColor = Vector3(specR, specG, specB);
            
            printf("Found a material, index will be %i\n", j);
            
            Material mat = Material(ambColor, diffColor, specColor);
            materials.push_back(mat);
        }
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

            /*
            float ambR = (*objData).materialList[ (*objData).sphereList[i]->materialIndex ]->amb[0];
            float ambG = (*objData).materialList[ (*objData).sphereList[i]->materialIndex ]->amb[1];
            float ambB = (*objData).materialList[ (*objData).sphereList[i]->materialIndex ]->amb[2];
            
            float diffR = (*objData).materialList[ (*objData).sphereList[i]->materialIndex ]->diff[0];
            float diffG = (*objData).materialList[ (*objData).sphereList[i]->materialIndex]->diff[1];
            float diffB = (*objData).materialList[ (*objData).sphereList[i]->materialIndex ]->diff[2];
            
            float specR = (*objData).materialList[ (*objData).sphereList[i]->materialIndex ]->spec[0];
            float specG = (*objData).materialList[ (*objData).sphereList[i]->materialIndex ]->spec[1];
            float specB = (*objData).materialList[ (*objData).sphereList[i]->materialIndex ]->spec[2];
            
            Color ambColor = Color((int)abs(ambR*255), (int)abs(ambG*255), (int)abs(ambB*255));
            Color diffColor = Color((int)abs(diffR*255), (int)abs(diffG*255), (int)abs(diffB*255));
            Color specColor = Color((int)abs(specR*255), (int)abs(specG*255), (int)abs(specB*255));
            */

            Vector3 sphereUp = Vector3(xUp, yUp, zUp);

            float radius = sphereUp.length();
            int materialIndex = (*objData).sphereList[i]->material_index;

            Sphere* sphere = new Sphere(spherePos, radius, materialIndex);
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

            int materialIndex = (*objData).faceList[i]->material_index;

            Triangle* tri = new Triangle(p1, p2, p3, materialIndex);
            surfaces.push_back(tri);
        }
    }

    if((*objData).lightPointCount > 0 && (*objData).lightPointList != NULL){
        for(int i = 0; i < (*objData).lightPointCount; i++){
           float posX = (*objData).vertexList[ (*objData).lightPointList[i]->pos_index ]->e[0]; 
           float posY = (*objData).vertexList[ (*objData).lightPointList[i]->pos_index ]->e[1]; 
           float posZ = (*objData).vertexList[ (*objData).lightPointList[i]->pos_index ]->e[2]; 

           int materialIndex = (*objData).lightPointList[i]->material_index;

           Vector3 pos = Vector3(posX, posY, posZ);
           

           PointLight* light = new PointLight(pos, materialIndex);
           printf("Light Found, x,y,z,mat: %f,%f,%f,%i\n", light->getPos()[0], light->getPos()[1], light->getPos()[2], light->getMaterialIndex());
           lights.push_back(light);
           
           Light* derp = lights[0];
        }
    }
    
    return Scene(camera, surfaces, materials, lights);
}

