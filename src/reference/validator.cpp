

#include "../../reference/validator.hpp"
#include "validator.hpp"

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

ValidationResult Validator::Execute() {
    try
    {
        
        return ValidationResult::Pass();
    }
    catch (ParseException &ex)
    {
        return ValidationResult::Fail(FailureCause::InvalidConnection);
    }
}