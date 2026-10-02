#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class MagazineLockedException : public std::runtime_error {
public:
    MagazineLockedException()
        : std::runtime_error("Cannot add page: magazine is locked.") {}
};

class InvalidContentException : public std::runtime_error {
public:
    InvalidContentException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif
