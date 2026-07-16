
#include "../../reference/factory.hpp"

template<class C>
std::map<int, Argument> Factory<C>::ready_args()
{
    map<int, Argument> args_container;
    if (typeid(C) == typeid(Hub))
    {
        args_container[0] = {"name", ""};
        args_container[1] = {"x", ""};
        args_container[2] = {"y", ""};
        args_container[3] = {"zone", ""};
        args_container[4] = {"color", ""};
        args_container[5] = {"max_drones", ""};
    }
    else if (typeid(C) == typeid(Connection))
    {
        args_container[0] = {"hub1", ""};
        args_container[1] = {"hub2", ""};
        args_container[2] = {"mlc", ""};
    }
    else
        cerr << "Invalid type detected." << endl;
    return args_container;
}

template<class C>
C Factory<C>::create(std::map<int, Argument> &args)
{
    C _ret;
    if constexpr (is_same_v<C, Hub>)
    {
        _ret.setName(args[0].value);
        _ret.setX(std::stoi(args[1].value));
        _ret.setY(std::stoi(args[2].value));
        _ret.setZone(args[3].value);
        _ret.setColor(args[4].value);
        if (args[5].value != "")
        {
            optional<int> conv = std::stoi(args[5].value);
            _ret.setMaxDrones(conv);
        }
    }
    else if constexpr (is_same_v<C, Connection>)
    {
        _ret.setHub1(args[0].value);
        _ret.setHub2(args[1].value);
        if (args[2].value != "")
        {
            optional<int> conv = std::stoi(args[2].value);
            _ret.setMlc(conv);
        }
    }
    else if constexpr (is_same_v<C, Map>)
    {
        _ret.setName(args[0].value);
        _ret.setDifficulty(args[1].value);
        _ret.setSrcPath(args[2].value);
    }
    return _ret;
}