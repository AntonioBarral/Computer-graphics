//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"
#include "ray.h"
#include <algorithm>
#include <vector>

Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    if(renderMode=="normal"){
	return normalBuffer(min_hit);

    //If renderMode==zbuffer, it is needed to store all distances, the maxDistance and the minDIstance before do the zbuffer method (call it from the render function in the second double loop)
    }else if(renderMode=="zbuffer"){
	if(min_hit.t>maxDistance){
   	    maxDistance=min_hit.t;
        }

        if(minDistance>min_hit.t || minDistance==0){
      	    minDistance=min_hit.t;
        }

        addDistance(min_hit.t);
	return Color(1.0, 1.0, 1.0);  //Default value to know when we have to implement zBuffer

    }else if(renderMode=="gooch"){
	return goochModel(obj, min_hit, ray);
    }else{
        return phongModel(obj, min_hit, ray);
    }
}


/*
* Gooch illumination method
*/
Color Scene::goochModel(Object *obj, Hit min_hit, const Ray &ray)
{
    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector L;					   //the light vector
    Vector R;					   //the R vector
    Vector V = -ray.D;                             //the view vector
    Color id(0.0,0.0,0.0);			   //diffuse intensity
    Color is(0.0,0.0,0.0);			   //specular intensity
    Color kCool(0.0,0.0,0.0);			   //kCool variable
    Color kWarm(0.0,0.0,0.0);			   //kWarm variable
    Color color(0.0,0.0,0.0);			   //total color
    Color goochKd(0.0,0.0,0.0);				   //gooch kd

 //Calculation of id and is for each light
    for(unsigned int i=0; i<lights.size(); i++) {

	L=(lights[i]->position-hit).normalized();
	R=(2*(N.dot(L))*N-L).normalized();

	goochKd=lights[i]->color*material->color*material->kd;

	kCool=gooch->kBlue+gooch->alpha*goochKd;
	kWarm=gooch->kYellow+gooch->beta*goochKd;

 	id+=kCool*(1-N.dot(L))/2 + kWarm*(1+N.dot(L))/2;
	is+=lights[i]->color*pow(max(0.0, R.dot(V)),material->n)*material->ks; 
    }

    color=id+is;
    return color;
}

/*
* Phong illumination method
*/
Color Scene::phongModel(Object *obj, Hit min_hit, const Ray &ray)
{
    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector
    Vector L;					   //the light vector
    Vector R;					   //the R vector
    Color ia(0.0,0.0,0.0);			   //ambient intensity   
    Color id(0.0,0.0,0.0);			   //diffuse intensity
    Color is(0.0,0.0,0.0);			   //specular intensity
    Color color(0.0,0.0,0.0);			   //total color
    Color reflect(0.0,0.0,0.0);		           //reflection
    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/


    //Loading the textures if needed
    if(material->texture!=0) {
	ia=obj->loadTexture(material->texture, hit);
    }else{
	ia=material->color*material->ka;
    }
    
    color=ia;
    //Calculation of id and is for each light
    for(unsigned int i=0; i<lights.size(); i++) {

	L=(lights[i]->position-hit).normalized();
     
	//if there the shadows are not considered or there are no shadows 
	if(!considerShadows || !areShadows(hit, N, L)) {
	     R=(2*(N.dot(L))*N-L).normalized();
 	     id+=lights[i]->color*max(0.0, N.dot(L));
	     is+=lights[i]->color*pow(max(0.0, R.dot(V)),material->n); 
        }
    }
    
    //if the recursivity is activated and ks bigger than 0
    //recursively continuing rays in the direction of the reflection  vector

    if(recDepth>0 && material->ks>0) {
	Color RV=(2*(N.dot(V))*N-V).normalized();
	Ray recRay(hit+N, RV);
	setRecDepth(recDepth-1); 
	reflect=trace(recRay);      
    }

    //get diffuse and specular illumination to obtain the final color
    id=id*material->color*material->kd;
    is=(is+reflect)*material->ks;
    color+=id+is;
   
    
    return color;
}

/*
* Normal buffer method
*/
Color Scene::normalBuffer(Hit min_hit)
{
    Color normalColor;
    Vector N= min_hit.N;

    /*d is the value needed to transform the normal interval to the color range(after the sub between the maximum of the color range '1' and the multiplication between the maximum of the normal scale '1', multiplied by the difference between the color range and the normal range '1/2')*/

    double d= 1/2.0;  

    normalColor=N/2.0+d;
    return normalColor;
}


/*
* Zbuffer method
*/
Color Scene::zBuffer(double t)
{
    Color hitColor;

    /*d is the value needed to transform the distance interval to the color range(after the sub between the maximum of the color range '1' and the multiplication between the maximum of the distance scale 'maxDistance', multiplied by the difference between the color range and the distance range '1/maxDistance-minDistance')*/

    

    double d=1-(maxDistance*(1/(maxDistance-minDistance)));
    double distanceNorm= (t/(maxDistance-minDistance))+d;
    double i=1.0-distanceNorm;
    hitColor+=i;
    return hitColor;
}

void Scene::render(Image &img)
{ 
    int w = img.width();
    int h = img.height();
    maxDistance=0;
    minDistance=0; 
    Color color;
    int auxRec=recDepth;
    

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            Point pixel(x+pixelSize/2, h-1-y+pixelSize/2, 0);
           
	    //get the pixel from the extended camera model
	    if(extCamera) {
		setEye(camera->eye);
		pixel=extendedCamera(*camera, pixel, w, h);
	    }

	    //if supersampling is activated
	    if(ssFactor!=1) {
		color=supersampling(x, y, h,auxRec);
	    }else{
		Ray ray(eye, (pixel-eye).normalized());
		color = trace(ray);
		recDepth=auxRec;
		
	    }
	    color.clamp();
	    img(x,y) = color;
        }
    }

    //This part only done if renderMode==zbuffer
    if(renderMode.compare("zbuffer")) return;


    //If the pixel equals (1.0,1.0,1.0) (default vale chosen for us to determine when there is a hit), apply zbuffer method to that pixel.
    int counter=0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
	    if(img(x,y).x==1.0 && img(x,y).y==1.0 && img(x,y).z==1.0) {
    	        Color col= zBuffer(distances.at(counter));
                col.clamp();
                img(x,y) = col;
	        counter++;
	    }	    
        }
    }
}



/*
*Supersampling method
*/

Color Scene::supersampling(int x, int y, int h, int auxRec){

	Color color;
	for (double i = h-y; i > h-y-pixelSize; i=i-(pixelSize/ssFactor)) {  
	    for (double j = x; j < x+pixelSize; j=j+(pixelSize/ssFactor)) {

		Point subpixel(j+(pixelSize/(2*ssFactor)),i-(pixelSize/(2*ssFactor)), 0);

		 

		Ray ray(eye, (subpixel-eye).normalized());
		color += trace(ray);
		recDepth=auxRec;


	    }

	}
	//get the average of all the colors
	color/=ssFactor*ssFactor;
	return color;
}

/*
*Calculate if the L of the object hits another object
*/

bool Scene::areShadows(Point hit, Vector N, Vector L) 
{
   Hit no_hit(std::numeric_limits<double>::infinity(),Vector());
  
   Ray nray(hit+N,L);       

    for(unsigned int j=0; j<objects.size(); j++) {
       Hit nHit=objects[j]->intersect(nray);
       if(nHit.t<no_hit.t) {
           return true;
       }
    }

   return false;
}
/*
* Extended camera model such that other image resolutions are possible
*/
Point Scene::extendedCamera(Camera &camera, Point pixel, int width, int height)
{
    Point finalPixel;

    double sx=pixel.x/(width-1);
    double sy=pixel.y/(height-1);

    Vector G=camera.center-eye;			//gaze direction and distance 
    Vector A=G.cross(camera.up).normalized();	//A = G x U(up vector)
    Vector B=A.cross(G).normalized();		//B = A x G

    Vector H=A*(width*pixelSize/2);		//H and V are used to sweep the screen
    Vector V=B*(height*pixelSize/2);

    finalPixel=camera.center+(2*sx-1)*H+(2*sy-1)*V;

    return finalPixel;
}


void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::addDistance(double t)
{
    distances.push_back(t);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setRenderMode(std::string rm)
{
    renderMode=rm;
}

void Scene::setShadows(bool b)
{
   considerShadows=b;
}

void Scene::setRecDepth(int r)
{
   recDepth=r;
}

void Scene::setSSFactor(double ss) 
{
   ssFactor=ss;
}

void Scene::setCamera(Camera *c) 
{
   camera=c;
   extCamera=true;    
}

void Scene::setPixelSize(double lenghtUp) 
{ 
   pixelSize = lenghtUp; 
}

void Scene::setGooch(Gooch *g)
{
   gooch=g;   
}
