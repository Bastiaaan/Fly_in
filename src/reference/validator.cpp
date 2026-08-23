
#include "factory.tpp"
#include "validator.hpp"
#include "hub.hpp"
#include "system.hpp"

ExecuteState Validator::saveRec(string &buffer, unsigned int const line)
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
    std::string errorTemplate = "";
    errorTemplate += causeToString(why) + ": ";
    errorTemplate += msg;
    errorTemplate += line > 0 ? " at line " + std::to_string(line) : "";
    this->errors[this->errors.size()] = errorTemplate;
}

bool Validator::isPassed()
{
    return this->errors.size() == 0;
}

std::vector<std::string> Validator::releaseErrors()
{
    std::vector<std::string> errors;
    for (auto const &error : this->errors)
        errors.push_back(error.second);
    return errors;
}