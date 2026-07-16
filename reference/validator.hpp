
#include "fly_in.hpp"

enum class Reason
{
    DuplicateHubName,
    DuplicateHubLoc,
    DuplicateRule,
    DuplicateConnection,
    InvalidConnection,
    MissingNDrones,
};

class Validator
{
    public:
        Validator();
        ~Validator();
        static int seekErrors(Map &map);
        ExecuteState duplicateFound(vector<Hub> &hubs);
        ExecuteState impossibleConnection(vector<Connection> &connections);
        ExecuteState wrongConfig(System &sys);
};