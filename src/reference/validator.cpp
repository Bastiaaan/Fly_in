
#include "factory.tpp"
#include "validator.hpp"
#include "hub.hpp"
#include "system.hpp"

ExecuteState Validator::saveRec(string &buffer, unsigned int line)
{
    try
    {
        if (buffer.empty())
            throw logic_error("Record cannot be empty.");
        if (!this->mapRows[line].empty())
            throw logic_error("line #" + any_cast<string>(line) + " is already saved");
        this->mapRows[line] = buffer;
        return ExecuteState::Ok(1);
    }
    catch(exception &ex)
    {
        return ExecuteState::Fail(ex.what());
    }
}

void Validator::saveError(FailureCause const &why, std::string const &msg, unsigned int line)
{
    std::cout << "Error while parsing;\n" << msg
              << " at line " << line << std::endl
              << causeToString(why) << std::endl; 
}

std::vector<FailureCause> Validator::Execute()
{
    ValidationResult res;
    std::vector<FailureCause> errors;
    try
    {
        for (auto [rule, record] : this->mapRows)
        {
            std::cout << TextFormat("rule #%d: -- %s --", rule, record.c_str()) << std::endl;
        }
        res = ValidationResult::Pass();
    }
    catch (ParseException &ex)
    {
        res = ValidationResult::Fail(FailureCause::InvalidConnection);
    }
    return errors;
}