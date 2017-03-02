

#include "plane.h"
#include <iostream>
#include <math.h>

/************************** Plane **********************************/


//The formule needed is t=(point-ray.O).dot(n)/ray.D.dot(n). One point and the normal are given

Hit Plane::intersect(const Ray &ray)
{

    double denumerator=ray.D.dot(n);
    double numerator=(point-ray.O).dot(n);
    if(denumerator<=0){
	return Hit::NO_HIT();
    }

    double t=numerator/denumerator;

    if(t<0) return Hit::NO_HIT();

    


//The normal is provided in the data of the figure
    return Hit(t,n);
}
