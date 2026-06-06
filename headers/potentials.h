#ifndef POTENTIALS_H
#define POTENTIALS_H


#include <vector>
#include "vec3d.h"
#include "error.h"


#define EPSILON 1e-10


enum Potential_type {
    newton
};


class Potential {
public:
    Potential() {}
    virtual ~Potential() {}
    
    virtual vec3d force(double m1, vec3d r1, double m2, vec3d r2) = 0;     // force acting ON body 1 FROM body 2
    virtual double energy(double m1, vec3d r1, double m2, vec3d r2) = 0;   // energy OF body 1 IN FIELD OF body 2
};


class Newton: public Potential {
    double G;
public:
    Newton(): G(6.674e-11) {}
    Newton(double G_): G(G_) {}
    inline void set_G(double G_) { G = G_; }
    virtual ~Newton() {}

    virtual vec3d force(double m1, vec3d r1, double m2, vec3d r2) override;
    virtual double energy(double m1, vec3d r1, double m2, vec3d r2) override;
};


#endif
