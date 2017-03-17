
#ifndef TRIANGLE_H_115209AE
#define TRIANGLE_H_115209AE

#include "object.h"

class Triangle : public Object
{
public:
    Triangle(Point v1, Point v2, Point v3) : v1(v1), v2(v2), v3(v3) { }

    virtual Hit intersect(const Ray &ray);
    virtual Color loadTexture(Image *texture, Point intersection);
    bool outOfTriangle(Point P, Vector N);

    const Point v1;
    const Point v2;
    const Point v3;
};

#endif /* end of include guard: TRIANGLE_H_115209AE */
