#ifndef CAMERA_H_
#define CAMERA_H_

#include "triple.h"

class Camera 
{

public:

    Camera(Triple e, Point c, Vector u, int s [2] ) 
    {
	eye=e;
	center=c;
	up=u;
	width=s[0];
        height=s[1];
    }

private:
    Triple eye;
    Point center;
    Vector up;
    int width;
    int height;

};

#endif
