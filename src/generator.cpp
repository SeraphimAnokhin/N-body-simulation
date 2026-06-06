#include "generator.h"

void Random::generate(const char *out_filename) {
    std::ofstream f;
    f.open(out_filename);

    if (!f.is_open()) {
        throw Memory_error("Failed to open file for writing.");
    }

    f << get_n() << std::endl;

    for (int i = 0; i < get_n(); i++) {
        f << (rand() % RAND_MAX) * (max_mass - min_mass) + min_mass << " ";

        f << (rand() % RAND_MAX) * max_coord * ((rand() % 2 == 0) ? 1 : -1) << " ";
        f << (rand() % RAND_MAX) * max_coord * ((rand() % 2 == 0) ? 1 : -1) << " ";
        f << (rand() % RAND_MAX) * max_coord * ((rand() % 2 == 0) ? 1 : -1) << " ";

        f << (rand() % RAND_MAX) * max_vel * ((rand() % 2 == 0) ? 1 : -1) << " ";
        f << (rand() % RAND_MAX) * max_vel * ((rand() % 2 == 0) ? 1 : -1) << " ";
        f << (rand() % RAND_MAX) * max_vel * ((rand() % 2 == 0) ? 1 : -1) << std::endl;
    }

    f.close();
}

void Cold_compression::generate(const char *out_filename) {
    std::ofstream f;
    f.open(out_filename);

    if (!f.is_open()) {
        throw Memory_error("Failed to open file for writing.");
    }

    f << get_n() << std::endl;

    for (int i = 0; i < get_n(); i++) {
        f << (rand() % RAND_MAX) * (max_mass - min_mass) + min_mass << " ";

        f << (rand() % RAND_MAX) * max_coord * ((rand() % 2 == 0) ? 1 : -1) << " ";
        f << (rand() % RAND_MAX) * max_coord * ((rand() % 2 == 0) ? 1 : -1) << " ";
        f << (rand() % RAND_MAX) * max_coord * ((rand() % 2 == 0) ? 1 : -1) << " ";

        f << 0 << " ";
        f << 0 << " ";
        f << 0 << std::endl;
    }

    f.close();
}

void Two_body::generate(const char *out_filename) {
    std::ofstream f;
    f.open(out_filename);

    if (!f.is_open()) {
        throw Memory_error("Failed to open file for writing.");
    }

    f << 2 << std::endl;
    double r = 1;
    double v = sqrt(6.674e-11) / 2;

    for (int i = 0; i < 2; i++) {
        f << 1 << " ";

        f << -r * ((i == 0) ? 1 : -1) << " ";
        f << 0 << " ";
        f << 0 << " ";

        f << 0 << " ";
        f << v * ((i == 0) ? 1 : -1) << " ";
        f << 0 << std::endl;
    }

    f.close();
}

void Four_body::generate(const char *out_filename) {
    std::ofstream f;
    f.open(out_filename);

    if (!f.is_open()) {
        throw Memory_error("Failed to open file for writing.");
    }

    f << 4 << std::endl;
    double r = 1;
    double w = sqrt(6.674e-11 * (4 + sqrt(2)) / (4 * sqrt(2)) / (r * r * r));
    double v = w * r;

    for (int i = 0; i < 4; i++) {
        f << 1 << " ";

        f << r * cos(i * PI / 2) << " ";
        f << r * sin(i * PI / 2) << " ";
        f << 0 << " ";

        f << v * -sin(i * PI / 2) << " ";
        f << v * cos(i * PI / 2) << " ";
        f << 0 << std::endl;
    }

    f.close();
}
