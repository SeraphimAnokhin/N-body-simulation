#include "integrator.h"

std::vector<vec3d> Euler::integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                           bool calc_energy, double *energy) {
    
    int n_bodies = m.size();
    if (state.size() != 2 * n_bodies) {
        throw Incorrect_input("Arrays size mismatch!");
    }
    std::vector<vec3d> res(2 * n_bodies);

    std::vector<vec3d> accelerations = A.calc_accelerations(m, state, n_threads, P, calc_energy, energy);
    
    for (int i = 0; i < n_bodies; i++) {
        res[i] = state[i] + time_step * state[n_bodies + i];
        res[n_bodies + i] = state[n_bodies + i] + time_step * accelerations[i];
    }

    return res;
}

std::vector<vec3d> Leapfrog::integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                              bool calc_energy, double *energy) {
    return std::vector<vec3d>();
}

std::vector<vec3d> RK4::integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                         bool calc_energy, double *energy) {
    return std::vector<vec3d>();
}

std::vector<vec3d> Adaptive_RK4::integration_step(const std::vector<double> &m, const std::vector<vec3d> &state, double time_step, int n_threads, Potential *P,
                                                  bool calc_energy, double *energy) {
    return std::vector<vec3d>();
}


void Accelerometer::calc_chunk(const std::vector<double> &m, const std::vector<vec3d> &state, Potential *P, int idx_start, int idx_finish,
                               std::vector<vec3d> &res, bool calc_energy, double *energy) {
    
    int n_bodies = m.size();
    vec3d tmp;

    for (int i = idx_start; i < idx_finish; i++) {
        res[i] = vec3d(0, 0, 0);
    }

    for (int i = idx_start; i < idx_finish; i++) {
        for (int j = 0; j < idx_start; j++) {
            res[i] += P->force(m[i], state[i], m[j], state[j]);
        }
        for (int j = i + 1; j < idx_finish; j++) {
            tmp = P->force(m[i], state[i], m[j], state[j]);        // forces 1->2 and 2->1 are have opposite directions
            res[i] += tmp;
            res[j] -= tmp;
        }
        for (int j = idx_finish; j < n_bodies; j++) {
            res[i] += P->force(m[i], state[i], m[j], state[j]);
        }

        res[i] /= m[i];
    }

    if (calc_energy) {
        *energy = 0;

        for (int i = idx_start; i < idx_finish; i++) {
            for (int j = 0; j < idx_start; j++) {
                *energy += P->energy(m[i], state[i], m[j], state[j]);
            }
            for (int j = i + 1; j < idx_finish; j++) {
                *energy += 2 * P->energy(m[i], state[i], m[j], state[j]);   // energy of 1 in field of 2 is equal to energy of 2 in field of 1
            }
            for (int j = idx_finish; j < n_bodies; j++) {
                *energy += P->energy(m[i], state[i], m[j], state[j]);
            }
    
            *energy /= 2;  // check later
        }
    }
}

std::vector<vec3d> Accelerometer::calc_accelerations(const std::vector<double> &m, const std::vector<vec3d> &state, int n_threads,
                                                     Potential *P, bool calc_energy, double *energy)
{

    int n_bodies = m.size();
    if (state.size() != 2 * n_bodies) {
        throw Incorrect_input("Arrays size mismatch!");
    }

    std::vector<std::thread> threads(n_threads);
    int step = n_bodies / n_threads;

    double energies[n_threads];
    std::vector<vec3d> res(n_bodies);

    for (int i = 0; i < n_threads - 1; i++) {
        threads[i] = std::thread([=, &res, &energies] () {this->calc_chunk(m, state, P, i * step, (i + 1) * step, res, calc_energy, energies + i);});
        //threads[i] = std::thread(&Accelerometer::calc_chunk, m, state, P, i * step, (i + 1) * step, res, calc_energy, energies + i);
    }
    threads[n_threads - 1] = std::thread([=, &res, &energies] () {this->calc_chunk(m, state, P, (n_threads - 2) * step, n_bodies, res, calc_energy, energies + n_threads - 1);});
    //threads[n_threads - 1] = std::thread(&Accelerometer::calc_chunk, m, state, P, (n_threads - 2) * step, n_bodies, res, calc_energy,
    //                                     energies + n_threads - 1);

    for (int i = 0; i < n_threads; i++) {
        threads[i].join();
    }

    if (calc_energy) {
        *energy = 0;
        for (int i = 0; i < n_threads; i++) {
            *energy += energies[i];
        }
    }

    return res;
}
