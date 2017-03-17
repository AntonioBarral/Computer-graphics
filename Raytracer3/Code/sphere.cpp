

#define _USE_MATH_DEFINES

#include "sphere.h"
#include <iostream>
#include <math.h>
#include <cmath>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    // place holder for actual intersection calculation
	
    double a=1;
    double b=2*(ray.O-position).dot(ray.D);
    double c=(ray.O-position).dot(ray.O-position)-r*r;

    double squareRoot=sqrt(b*b-4*a*c);
    double t=0;

    if(squareRoot>=0.0){
	t=(-b-squareRoot)/(2*a);
	
	if(t<0.0) {
	    t=(-b+squareRoot)/(2*a);
        }
    }else{
	return Hit::NO_HIT();
    }

    if(t<0.0) return Hit::NO_HIT();

    Point intersec=ray.O+t*ray.D;

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    * 
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

    Vector N=(intersec-position).normalized();

    return Hit(t,N);
}



/*
* Load the u,v of a texture at the intersection point of a sphere
*/
Color Sphere::loadTexture(Image *texture, Point intersection) 
{
    double zeta=0;
    double phi=0;
    double u=0;
    double v=0;

    zeta=acos((intersection.z-position.z)/r);
    phi=atan2 (intersection.y-position.y,intersection.x-position.x);

    if(phi<0) phi+=2*M_PI;

    u=phi/(2*M_PI);
    v=(M_PI-zeta)/M_PI;

    return texture->colorAt(u,v);
}
