#ifndef FACTORY_HPP
# define FACTORY_HPP
#include "fly_in.hpp"

template<class C>
class Factory
{
    public:
        static C create();
};

#endif