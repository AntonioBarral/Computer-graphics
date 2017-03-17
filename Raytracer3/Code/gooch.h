#ifndef GOOCH_H_
#define GOOCH_H_

#include "triple.h"

class Gooch 
{

public:

    double b;
    double y;
    double alpha;
    double beta;

    Color kYellow;
    Color kBlue; 
 

    Gooch(double db, double dy, double a, double b) 
    {
	b=db;
	y=dy,
	alpha=a;
        beta=b;

        kYellow= Color(y,y,0.0);
	kBlue= Color(0.0,0.0,b);
    }

};

#endif
