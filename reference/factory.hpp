#pragma once
# include "fly_in.hpp"

class Hub;
struct Argument
{
    std::string key;
    std::variant<int, std::optional<int>, bool, std::string, std::optional<std::string>, char, double, Hub*> value;
    Argument& operator=(Argument &&n) {
        key = std::move(n.key);
        value = std::move(n.value);
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
