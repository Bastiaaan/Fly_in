
#pragma once
# include "fly_in.hpp"

struct ExecuteState;

enum class FailureCause
{
    DuplicateHubName,
    DuplicateHubLoc,
    DuplicateRule,
    DuplicateConnection,
    InvalidConnection,
    ImpossibleConnection,
    MissingNbDrones,
    IncorrectLocValue,
    IncorrectNameValue,
    NoStartHubFound,
    NoEndHubFound,
    MultiStartHubs,
    MultiEndHubs
};

struct ValidationResult
{
    std::optional<FailureCause> cause;
    bool passed = false;
    static ValidationResult Pass()
    {
        ValidationResult res;
        res.passed = true;
        return res;
    }
    static ValidationResult Fail(FailureCause cause)
    {
        ValidationResult res;
        res.passed = false;
        return res;
    }
};

class Validator
{
    private:
        std::map<int, std::string> mapRows;

    public:
        Validator() = default;
        ~Validator() = default;
        ExecuteState saveRec(std::string &buffer, unsigned int line);
        ValidationResult Execute();
        static void readCause(FailureCause const &why)
        {
            switch(why)
            {
                case FailureCause::DuplicateHubName:
                    std::cout << "duplicate hub-name found" << std::endl;
                    break;
                case FailureCause::DuplicateConnection:
                    std::cout << "duplicate connection found" << std::endl;
                    break;
                case FailureCause::DuplicateHubLoc:
                    std::cout << "duplicate hub location found" << std::endl;
                    break;
                case FailureCause::DuplicateRule:
                    std::cout << "duplicate rule found" << std::endl;
                    break;
                case FailureCause::MissingNbDrones:
                    std::cout << "nb_drones rule is required" << std::endl;
                    break;
                default:
                    std::cout << "" << std::endl;
                    break;
            }
        }
};
