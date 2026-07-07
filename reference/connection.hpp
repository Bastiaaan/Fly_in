
#include "fly_in.hpp"
#include "hub.hpp"

class Connection
{
    private:
        bool validate_connection();
        void establish();
    public:
        std::string conn_name;
        std::optional<int> max_link_capacity;
        Connection(std::string &hub1, std::string &hub2, std::optional<int> mlc);
        ~Connection();
};