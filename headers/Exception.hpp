#pragma once
#include <exception>
#include <string>

class Exception: public std::runtime_error
{
public:
    Exception(const std::string &arg = "") : runtime_error(arg) {}
};

class IOfailed : public Exception
{
    public:
        IOfailed(const std::string &arg = "") : Exception(arg) {}
};

class TooManyPowerups : public Exception
{
    public:
        TooManyPowerups(const std::string &arg = "") : Exception(arg) {}
};