#ifndef GENERATOR_H
#define GENERATOR_H


#include <cmath>
#include <fstream>
#include "error.h"


#define PI 3.14159


class Generator {
    int n;
public:
    Generator() {}
    inline void set_n(int n_) { n = n_; }
    inline int get_n() { return n; }
    virtual void generate(const char *out_filename) = 0;
    virtual ~Generator() {}
};


class Random: public Generator {
    double min_mass, max_mass, max_coord, max_vel;
public:
    Random(): Generator(), min_mass(0.1), max_mass(1e40), max_coord(1e20), max_vel(1e5) {}
    virtual void generate(const char *out_filename) override;
    virtual ~Random() {}
};


class Two_body: public Generator {
public:
    virtual void generate(const char *out_filename) override;
};


class Four_body: public Generator {
public:
    virtual void generate(const char *out_filename) override;
};


class Cold_compression: public Generator {
    double min_mass, max_mass, max_coord, max_vel;
public:
    Cold_compression(): Generator(), min_mass(0.1), max_mass(1e40), max_coord(1e20) {}
    virtual void generate(const char *out_filename) override;
};


#endif
