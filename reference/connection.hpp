#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include "fly_in.hpp"
#include "system.hpp"
#include "map.hpp"
#pragma once

class Connection
{
    public:
        Hub *hub1;
        Hub *hub2;
        std::optional<int> max_link_capacity;
        Connection();
        ~Connection();
        void setHub1(Hub *hub1);
        void setHub2(Hub *hub2);
        void setMlc(optional<int> mlc);
    private:
        ExecuteState validate_connection(Map &map);
        ExecuteState establish();
};

#endif