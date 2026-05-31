#ifndef VEC3D_H
#define VEC3D_H


#include <math.h>
#include <iostream>


struct vec3d {
public:
    double x, y, z;
    vec3d(): x(0), y(0), z(0) {}
    vec3d(double x_, double y_, double z_): x(x_), y(y_), z(z_) {}
    vec3d(const vec3d &v): x(v.x), y(v.y), z(v.z) {}
    //vec3d(double r, double phi_rad, double theta_rad);
    ~vec3d() {}
    inline double len() const { return sqrt(x * x + y * y + z * z); }

    vec3d operator= (const vec3d &a);
};



vec3d operator+ (const vec3d &a, const vec3d &b);

vec3d operator+= (vec3d &a, const vec3d &b);

vec3d operator- (const vec3d &a, const vec3d &b);

vec3d operator-= (vec3d &a, const vec3d &b);

vec3d operator- (const vec3d &a);

vec3d operator* (const vec3d &a, double k);

vec3d operator* (double k, const vec3d &a);

vec3d operator/ (const vec3d &a, double k);

vec3d operator/= (vec3d &a, double k);

double operator* (const vec3d &a, const vec3d &b);  // scalar multiplication

vec3d operator^ (const vec3d &a, const vec3d &b);  // vector multiplication


std::istream& operator>> (std::istream& in, vec3d &v);

std::ostream& operator<< (std::ostream& out, vec3d &v);


#endif
