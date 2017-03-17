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
#include "camera.h"
#include "gooch.h"

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
    bool considerShadows;
    bool extCamera;
    int recDepth;
    double ssFactor;
    double pixelSize;
    Camera *camera;
    Gooch *gooch;


public:
    Color trace(const Ray &ray);

    Color phongModel(Object *obj, Hit min_hit, const Ray &ray);
    Color normalBuffer(Hit min_hit);
    Color zBuffer(double t);    
    Color supersampling(int x, int y, int h, int auxRec);
    Color goochModel(Object *obj, Hit min_hit, const Ray &ray);


    bool areShadows(Point hit, Vector N, Vector L);
    Point extendedCamera(Camera &camera, Point pixel, int width, int height);

    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void addDistance(double t);
    void setEye(Triple e);
    void setRenderMode(std::string rm);
    void setShadows(bool b);
    void setRecDepth(int r);
    void setSSFactor(double ss);
    void setCamera(Camera *c);
    void setPixelSize(double lenghtUp);
    void setGooch(Gooch *g);

    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
