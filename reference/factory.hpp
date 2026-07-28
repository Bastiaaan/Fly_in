#ifndef FACTORY_HPP
# define FACTORY_HPP
#include "fly_in.hpp"
#pragma once

struct Argument
{
    string key;
    variant<int, optional<int>, bool, std::string, optional<string>, char, double, Hub*, Map> value;
    Argument& operator=(Argument &&n) {
        key = move(n.key);
        value = move(n.value);
        return *this;
    }
};

template<class C>
class Factory
{
    public:
        static C create(std::map<int, Argument> &args);
        static std::map<int, Argument> ready_args();
        static int resolveKey(std::string const &key, std::map<int, Argument> &args);
};

#endif