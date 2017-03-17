
#ifndef PLANE_H_115209AE
#define PLANE_H_115209AE

#include "object.h"

class Plane : public Object
{
public:
    Plane(Point point ,Vector n) : point(point), n(n) { }

    virtual Hit intersect(const Ray &ray);
    virtual Color loadTexture(Image *texture, Point intersection);

    const Point point;
    Vector n;
};

#endif /* end of include guard: PLANE_H_115209AE */