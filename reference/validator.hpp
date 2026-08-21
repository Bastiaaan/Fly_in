
#pragma once
# include <optional>
# include "factory.hpp"
# include "fly_in.hpp"
# include "system.hpp"

struct ExecuteState;

enum class FailureCause
{
    NoErrorsGiven = 0,
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
    static ValidationResult Fail(FailureCause const &cause)
    {
        ValidationResult res;
        res.cause.value() = cause;
        res.passed = false;
        return res;
    }
};

class Validator
{
    private:
        std::map<int, std::string> mapRows;
        std::map<int, std::string> errors;

    public:
        Validator() = default;
        ~Validator() = default;
        ExecuteState saveRec(std::string &buffer, unsigned int line);
        void saveError(FailureCause const &why, std::string const &msg, unsigned int line);
        std::vector<FailureCause> Execute();
        static std::string causeToString(FailureCause const &why)
        {
            switch (why)
            {
                case FailureCause::DuplicateHubName:
                    return "duplicate hub-name found";
                case FailureCause::DuplicateHubLoc:
                    return "duplicate hub location found";
                case FailureCause::DuplicateRule:
                    return "duplicate rule found";
                case FailureCause::DuplicateConnection:
                    return "duplicate connection found";
                case FailureCause::InvalidConnection:
                    return "invalid connection found";
                case FailureCause::ImpossibleConnection:
                    return "impossible connection found";
                case FailureCause::MissingNbDrones:
                    return "nb_drones rule is required";
                case FailureCause::IncorrectLocValue:
                    return "incorrect location value";
                case FailureCause::IncorrectNameValue:
                    return "incorrect name value";
                case FailureCause::NoStartHubFound:
                    return "no start hub found";
                case FailureCause::NoEndHubFound:
                    return "no end hub found";
                case FailureCause::MultiStartHubs:
                    return "multiple start hubs found";
                case FailureCause::MultiEndHubs:
                    return "multiple end hubs found";
                case FailureCause::NoErrorsGiven:
                default:
                    return "";
    }
}
};
