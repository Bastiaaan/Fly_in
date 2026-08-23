
#include "factory.hpp"

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
        args_container[2] = {.key = "max_link_capacity", .value = string("")};
        args_container[3] = {.key = "map", .value = string("")};
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
        if (resolveKey("name", args) != -1)
            _ret.setName(std::get<std::string>(args[resolveKey("name", args)].value));
        if (resolveKey("x", args) != -1)
            _ret.setX(stoi(std::get<std::string>(args[resolveKey("x", args)].value)));
        if (resolveKey("y", args) != -1)
            _ret.setY(stoi(std::get<std::string>(args[resolveKey("y", args)].value)));
        if (resolveKey("zone", args) != -1)
            _ret.setZone(std::get<std::string>(args[resolveKey("zone", args)].value));
        if (resolveKey("color", args) != -1)
            _ret.setColor(std::get<std::string>(args[resolveKey("color", args)].value));
        if (resolveKey("max_drones", args) != -1
            && !get<string>(args[resolveKey("max_drones", args)].value).empty())
        {
            int conv = stoi(get<string>(args[5].value));
            _ret.setMaxDrones(conv);
        }
        else
            _ret.setMaxDrones(1);
    }
    else if constexpr (is_same_v<C, Connection>)
    {
        if (resolveKey("hub1", args) != -1)
            _ret.setHub1(get<Hub*>(args[resolveKey("hub1", args)].value));
        if (resolveKey("hub2", args) != -1)
            _ret.setHub2(get<Hub*>(args[resolveKey("hub2", args)].value));
        if (resolveKey("max_link_capacity", args) != -1 && !get<string>(args[2].value).empty())
        {
            unsigned int conv = stoi(get<string>(args[2].value));
            _ret.setMlc(conv);
        }
        else
            _ret.setMlc(1);
    }
    else if constexpr (is_same_v<C, Map>)
    {
        _ret.setName(args[0].value);
        _ret.setDifficulty(args[1].value);
        _ret.setSrcPath(args[2].value);
    }
    return _ret;
}

template<class C>
int Factory<C>::resolveKey(const std::string &key, std::map<int, Argument> &args)
{
    auto fromKey = [&args](std::string const &_key) -> int {
        int index = 0;
        for (auto &arg: args)
        {
            if (arg.second.key == _key)
                return index;
            index++;
        }
        return -1;
    };
    return fromKey(key);
}

template<class C>
std::string Factory<C>::getVal(std::map<int, Argument> &args, const std::string &key)
{
    int at = resolveKey(key, args);
    if (at < 0)
        return "";
    return get<std::string>(args.at(at).value);
}