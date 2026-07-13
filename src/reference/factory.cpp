

#include "./../../reference/fly_in.hpp"

template<class C>
C Factory<C>::create()
{
    return C();
}
