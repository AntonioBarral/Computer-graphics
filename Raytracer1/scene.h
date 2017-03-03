//
//  Framework for a raytracer
//  File: scene.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    std::vector<double> distances;
    std::string renderMode;
    Triple eye;
    double maxDistance;
    double minDistance;
public:
    Color trace(const Ray &ray);

    Color phongModel(Object *obj, Hit min_hit, const Ray &ray);
    Color normalBuffer(Hit min_hit);
    Color zBuffer(double t);

    Color diffuse(Material *material, Triple N, Point hit);
    Color specular(Material *material, Triple N, Triple V, Point hit);

    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void addDistance(double t);
    void setEye(Triple e);
    void setRenderMode(std::string rm);
   

    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
