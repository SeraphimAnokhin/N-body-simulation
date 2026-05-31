#include "parser.h"

void Parser::parse(const char *filename, std::vector<double> &m, std::vector<vec3d> &r0, std::vector<vec3d> &v0) {
    std::ifstream f;
    f.open(filename);

    if (!f.is_open()) {
        throw Memory_error("Failed to open file for reading.");
    }

    int n_bodies;
    f >> n_bodies;

    m = std::vector<double>(n_bodies);
    r0 = std::vector<vec3d>(n_bodies);
    v0 = std::vector<vec3d>(n_bodies);

    for (int i = 0; i < n_bodies; i++) {
        f >> m[i] >> r0[i] >> v0[i];
    }
}
