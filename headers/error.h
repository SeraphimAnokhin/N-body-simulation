#ifndef ERROR_H
#define ERROR_H


#include <string>


class Error {
public:
    Error() {}
    Error(const Error &) {}
    virtual ~Error() {}
    virtual std::string what() = 0;
};


class Incorrect_input: public Error {
    std::string msg;
public:
    Incorrect_input(): Error(), msg() {}
    Incorrect_input(std::string msg_): Error(), msg(msg_) {}
    Incorrect_input(const Incorrect_input &II): Error(), msg(II.msg) {}
    virtual ~Incorrect_input() {}
    virtual std::string what() override { return msg; }
};


class Memory_error: public Error {
    std::string msg;
public:
    Memory_error(): Error(), msg() {}
    Memory_error(std::string msg_): Error(), msg(msg_) {}
    Memory_error(const Memory_error &ME): Error(), msg(ME.msg) {}
    virtual ~Memory_error() {}
    virtual std::string what() override { return msg; }
};


#endif
