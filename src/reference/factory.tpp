
#include "../../reference/factory.hpp"

template<class C>
std::map<int, Argument> Factory<C>::ready_args()
{
    map<int, Argument> args_container;
    if (typeid(C) == typeid(Hub))
    {
        args_container[0] = {.key = "name", .value = string("")};
        args_container[1] = {.key = "x", .value = string("")};
        args_container[2] = {.key = "y", .value = string("")};
        args_container[3] = {.key = "zone", .value = string("")};
        args_container[4] = {.key = "color", .value = string("")};
        args_container[5] = {.key = "max_drones", .value = string("")};
    }
    else if (typeid(C) == typeid(Connection))
    {
        args_container[0] = {.key = "hub1", .value = string("")};
        args_container[1] = {.key = "hub2", .value = string("")};
        args_container[2] = {.key = "mlc", .value = string("")};
    }
    else
        cerr << "Invalid type detected." << endl;
    return args_container;
}

template<class C>
C Factory<C>::create(map<int, Argument> &args)
{
    C _ret;
    if constexpr (is_same_v<C, Hub>)
    {
        _ret.setName(get<string>(args[0].value));
        _ret.setX(stoi(get<string>(args[1].value)));
        _ret.setY(stoi(get<string>(args[2].value)));
        _ret.setZone(get<string>(args[3].value));
        _ret.setColor(get<string>(args[4].value));
        if (!get<string>(args[5].value).empty())
        {
            int conv = stoi(get<string>(args[5].value));
            _ret.setMaxDrones(conv);
        }
    }
    else if constexpr (is_same_v<C, Connection>)
    {
        _ret.setHub1(get<Hub*>(args[0].value));
        _ret.setHub2(get<Hub*>(args[1].value));
        if (!get<string>(args[2].value).empty())
        {
            int conv = stoi(get<string>(args[2].value));
            _ret.setMlc(conv);
            cout << "[optional] max-link-capacity has been saved" << endl;
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