#ifndef PARSER_H
#define PARSER_H


#include <fstream>
#include <vector>
#include <string>
#include "vec3d.h"
#include "error.h"


class Parser {
public:
    static void parse(std::string filename, std::vector<double> &m, std::vector<vec3d> &r0, std::vector<vec3d> &v0);
    static void save(std::string filename, std::vector<std::vector<vec3d>> states);
};


#endif
