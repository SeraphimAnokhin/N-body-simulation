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
    bool ready;
    double progress;
    bool do_calculations;
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

    void start_calculation();
    inline void stop_calculation() { do_calculations = false; }

    inline bool is_ready() { return ready; }
    inline double get_progress() { return progress; }

    inline std::vector<std::vector<vec3d>> get_states() { return states; }
    inline std::vector<double> get_calc_time() { return calc_time; }
    inline std::vector<double> get_energy() { return energy; }
    inline std::vector<vec3d> get_angular_momentum() { return angular_momentum; }
};


#endif
