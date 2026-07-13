#ifndef CONNECTION_HPP
# define CONNECTION_HPP
#include "fly_in.hpp"
#include "abstractions.hpp"
#include "hub.hpp"

class Connection
{
    public:
        std::string conn_name;
        std::optional<int> max_link_capacity;
        Connection(std::string const &hub1, std::string const &hub2, std::optional<int> mlc);
        ~Connection();

    private:
        bool validate_connection();
        void establish();
};

class ConnectionData : public Data<Connection>
{
    public:
        ConnectionData();
        ~ConnectionData();
    private:
        string hub1;
        string hub2;
        std::optional<int> mlc;
};

#endif