#ifndef MODEL_H
#define MODEL_H


#include <vector>
#include "vec3d.h"
#include "integrator.h"
#include "error.h"
#include "potentials.h"


class Model {
    int n_bodies;
    double time_step;
    double time_lim;
    double state_save_step;
    int n_threads;
    std::vector<double> m;
    std::vector<vec3d> r0;
    std::vector<vec3d> v0;

    std::vector<std::vector<vec3d>> states;
    std::vector<double> calc_time;
    std::vector<double> energy;
    std::vector<vec3d> angular_momentum;

    Integrator *I;
    Potential *P;

    vec3d calc_angular_momentum(std::vector<vec3d> state);
public:
    Model();
    ~Model();

    inline void set_n_bodies(int n) { n_bodies = n; }
    inline void set_time_step(double dt) { time_step = dt; }
    inline void set_time_lim(double t) { time_lim = t; }
    inline void set_state_save_step(double dt) { state_save_step = dt; }
    inline void set_n_threads(int n) { n_threads = n; }
    inline void set_initial_state(std::vector<double> mass, std::vector<vec3d> pos, std::vector<vec3d> vel) {
        m = mass;
        r0 = pos;
        v0 = vel;
    }
    void set_integrator_type(Integrator_type type);
    void set_potential_type(Potential_type type);
    void set_potential_params(std::vector<double> params);
    
    inline int get_n_bodies() { return n_bodies; }
    inline double get_time_step() { return time_step; }
    inline double get_time_lim() { return time_lim; }
    inline double get_state_save_step() { return state_save_step; }
    inline int get_n_threads() { return n_threads; }


    void start_calculation();
    // void pause_calculation();
    // void stop_calculation();

    

};


#endif
