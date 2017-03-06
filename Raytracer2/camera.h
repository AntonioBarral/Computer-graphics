#ifndef CAMERA_H_
#define CAMERA_H_

#include "triple.h"

class Camera 
{

public:

    Triple eye;
    Point center;
    Vector up;
    int width;
    int height;
    double pixelSize;

    Camera(Triple e, Point c, Vector u, int w, int h ) 
    {
	eye=e;
	center=c;
	up=u;
	width=w;
        height=h;
    }

    void setPixelSize() 
    { 
        pixelSize = up.length(); 
    }
};

#endif
