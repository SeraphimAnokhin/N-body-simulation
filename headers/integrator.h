#ifndef INTEGRATOR_H
#define INTEGRATOR_H


#include <vector>
#include <thread>
#include "vec3d.h"
#include "potentials.h"


class Accelerometer {
    void calc_chunk(const std::vector<double> &m, const std::vector<vec3d> &state, Potential *P, int idx_start, int idx_finish,
                    std::vector<vec3d> &res, bool calc_energy, double *energy);
public:
    Accelerometer() {}
    ~Accelerometer() {}
    std::vector<vec3d> calc_accelerations(const std::vector<double> &m, const std::vector<vec3d> &state, int n_threads, Potential *P,
                                          bool calc_energy, double *energy);
};


enum Integrator_type {
    euler,
    leapfrog,
    rk4,
    adaptive_rk4
};


class Integrator {
protected:
    Accelerometer A;
public:
    virtual std::vector<vec3d> integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                                bool calc_energy, double *energy) = 0;
    Integrator(): A() {}
    virtual ~Integrator() {}
};


class Euler: public Integrator {
public:
    Euler(): Integrator() {};
    virtual ~Euler() {}
    virtual std::vector<vec3d> integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                                bool calc_energy, double *energy) override;
};


class Leapfrog: public Integrator {
public:
    Leapfrog(): Integrator() {};
    virtual ~Leapfrog() {}
    virtual std::vector<vec3d> integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                                bool calc_energy, double *energy) override;
};


class RK4: public Integrator {
public:
    RK4(): Integrator() {};
    virtual ~RK4() {}
    virtual std::vector<vec3d> integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                                bool calc_energy, double *energy) override;
};


class Adaptive_RK4: public Integrator {
public:
    Adaptive_RK4(): Integrator() {};
    virtual ~Adaptive_RK4() {}
    virtual std::vector<vec3d> integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                                bool calc_energy, double *energy) override;
};


#endif
