#include "potentials.h"


vec3d Newton::force(double m1, vec3d r1, double m2, vec3d r2) {
    vec3d r = r2 - r1;
    double d = r.len();
    return r * (G * m1 * m2 / (d * d * d + EPSILON));
}

double Newton::energy(double m1, vec3d r1, double m2, vec3d r2) {
    double d = (r2 - r1).len();
    return -G * m1 * m2 / (d + EPSILON);
}
