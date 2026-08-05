#pragma once
#include "hub.hpp"
#include "system.hpp"

struct ExecuteState;
class Map;

class Connection
{
    void establish() const;
    public:
        Hub *hub1;
        Hub *hub2;
        std::optional<int> max_link_capacity;
        Connection() = default;
        ~Connection() = default;
        void setHub1(Hub *hub1);
        void setHub2(Hub *hub2);
        void setMlc(optional<int> max_link_capacity);
        ExecuteState validate_connection(Map &map) const;
};
