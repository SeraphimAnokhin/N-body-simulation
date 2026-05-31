#ifndef PARSER_H
#define PARSER_H


#include <fstream>
#include <vector>
#include "vec3d.h"
#include "error.h"


class Parser {
public:
    static void parse(const char *filename, std::vector<double> &m, std::vector<vec3d> &r0, std::vector<vec3d> &v0);
};


#endif
