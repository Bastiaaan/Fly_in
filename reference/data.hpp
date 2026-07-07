
#ifndef DATA_HPP
# define DATA_HPP

#include "fly_in.hpp"
#include "system.hpp"

template<class T>
class Data {
    private:
        T value{};
	public:
        Data(const T& value);
        Data() = default;
        ~Data() = default;
};

#endif