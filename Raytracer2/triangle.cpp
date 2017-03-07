
#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/


//Using the plane ecuation Ax+By+Cz+D=0 and the intersection point ecuation P=O+P.D*t we get t=-(N.dot(O)+D)/N.dot(R)

Hit Triangle::intersect(const Ray &ray)
{
    Vector N=(v2-v1).cross(v3-v1);

    Vector conflictN_rayD=N.cross(ray.D);

    if(conflictN_rayD.x==0.0 && conflictN_rayD.y==0.0 && conflictN_rayD.z==0.0){
	N=-N;
    }

    double D=N.dot(v1);

    double numerator=-(N.dot(ray.O))+D;
    double denominator=N.dot(ray.D);

    


    //The ray is parallel triangle 
    if(fabs(denominator)<=0) return Hit::NO_HIT();  
    
    

    double t=numerator/denominator;
    Point P=ray.O+t*ray.D;

    
    //Calculates if the point is in the triangle or not, or the  triangle is behind the ray
    if(outOfTriangle(P, N) || t<0) return Hit::NO_HIT();

    return Hit(t,N);
}


bool Triangle::outOfTriangle(Point P, Vector N)
{
    Vector V1=v2-v1;
    Vector VP1=P-v1;
    if(N.dot(V1.cross(VP1))<0) return true;

    Vector V2=v3-v2;
    Vector VP2=P-v2;
    if(N.dot(V2.cross(VP2))<0) return true;

    Vector V3=v1-v3;
    Vector VP3=P-v3;
    if(N.dot(V3.cross(VP3))<0) return true;

    return false;
}
