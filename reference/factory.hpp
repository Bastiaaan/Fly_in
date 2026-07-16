#ifndef FACTORY_HPP
# define FACTORY_HPP
#include "fly_in.hpp"

struct Argument
{
    string key;
    string value;
};

template<class C>
class Factory
{
    public:
        static C create(std::map<int, Argument> &args);
        static std::map<int, Argument> ready_args();
};

#endif