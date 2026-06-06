#include "model.h"

Model::Model(): n_bodies(),
                time_step(),
                time_lim(),
                state_save_step(),
                n_threads(),
                m(), r0(), v0(),
                states(), calc_time(), energy(), angular_momentum(),
                I(), P(),
                ready(true), progress(0), do_calculations(true) {}

Model::~Model() {
    if (I) {
        delete I;
        I = nullptr;
    }
    if (P) {
        delete P;
        P = nullptr;
    }
}

void Model::set_integrator_type(Integrator_type type) {
    if (I) {
        delete I;
        I = nullptr;
    }

    switch (type) {
    case Integrator_type::euler:
        I = new Euler;
        break;
    case Integrator_type::leapfrog:
        I = new Leapfrog;
        break;
    case Integrator_type::rk4:
        I = new RK4;
        break;
    default:
        break;
    }
}

void Model::set_potential_type(Potential_type type) {
    if (P) {
        delete P;
        P = nullptr;
    }

    switch (type) {
    case Potential_type::newton:
        P = new Newton;
        break;
    default:
        break;
    }
}

void Model::start_calculation() {
    if (!ready) {
        return;
    }

    states = std::vector<std::vector<vec3d>>();
    angular_momentum = std::vector<vec3d>();
    energy = std::vector<double>();
    calc_time = std::vector<double>();

    ready = false;
    do_calculations = true;

    if (P == nullptr) {
        throw Incorrect_input("Set potential type!");
    }
    if (I == nullptr) {
        throw Incorrect_input("Set integrator type!");
    }

    // for (int i = 0; i < n_bodies; i++) {
    //     std::cout << m[i] << " " << r0[i] << " " << v0[i] << "\n";
    // }

    int n_steps = int(time_lim / time_step);
    int save_step = int(state_save_step / time_step);

    std::vector<vec3d> curr_state(2 * n_bodies);
    std::vector<vec3d> state_for_save(n_bodies);

    if (r0.size() < n_bodies) {
        throw Incorrect_input("Set initial positions!");
    }
    if (v0.size() < n_bodies) {
        throw Incorrect_input("Set initial velocities!");
    }

    for (int i = 0; i < n_bodies; i++) {
        curr_state[i] = r0[i];
        curr_state[n_bodies + i] = v0[i];
    }

    double curr_energy;
    bool calc_energy = false;

    for (int i = 0; (i < n_steps) && do_calculations; i++) {
        progress = 1.0 * i / n_steps;
        auto start = std::chrono::high_resolution_clock::now();
        if (i % save_step == 0) {
            for (int j = 0; j < n_bodies; j++) {
                state_for_save[j] = curr_state[j];
            }
            states.push_back(state_for_save);
            angular_momentum.push_back(calc_angular_momentum(curr_state));
            calc_energy = true;
        }

        try {
            curr_state = I->integration_step(m, curr_state, time_step, n_threads, P, calc_energy, &curr_energy);
        }
        catch (...) {
            throw;
        }

        if (i % save_step == 0) {
            energy.push_back(curr_energy);
            calc_energy = false;
        }
        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        calc_time.push_back(duration);
    }
    ready = true;
}

vec3d Model::calc_angular_momentum(std::vector<vec3d> state) {
    vec3d ans(0, 0, 0);
    for (int i = 0; i < n_bodies; i++) {
        ans += m[i] * state[i] ^ state[n_bodies + i];
    }
    return ans;
}
