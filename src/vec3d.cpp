#include "vec3d.h"


// vec3d::vec3d(double r, double phi_rad, double theta_rad) {
//     x = r * cos(phi_rad) * cos(theta_rad);
//     y = r * sin(phi_rad) * cos(theta_rad);
//     z = r * sin(theta_rad);
// }

vec3d vec3d::operator=(const vec3d &a) {
    x = a.x;
    y = a.y;
    z = a.z;
    return vec3d(x, y, z);
}


vec3d operator+ (const vec3d &a, const vec3d &b) {
    return vec3d(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3d operator+=(vec3d &a, const vec3d &b) {
    return a = a + b;
}

vec3d operator- (const vec3d &a, const vec3d &b) {
    return vec3d(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3d operator-=(vec3d &a, const vec3d &b) {
    return a = a - b;
}

vec3d operator- (const vec3d &a) {
    return vec3d(-a.x, -a.y, -a.z);
}

vec3d operator* (const vec3d &a, double k) {
    return vec3d(a.x * k, a.y * k, a.z * k);
}

vec3d operator* (double k, const vec3d &a) {
    return vec3d(a.x * k, a.y * k, a.z * k);
}

vec3d operator/ (const vec3d &a, double k) {
    return vec3d(a.x / k, a.y / k, a.z / k);
}

vec3d operator/= (vec3d &a, double k) {
    return a = a / k;
}

double operator* (const vec3d &a, const vec3d &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3d operator^(const vec3d &a, const vec3d &b) {
    return vec3d(a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x);
}

std::istream &operator>>(std::istream &in, vec3d &v) {
    in >> v.x >> v.y >> v.z;
    return in;
}

std::ostream &operator<<(std::ostream &out, vec3d &v) {
    out << v.x << " " << v.y << " " << v.z;
    return out;
}
