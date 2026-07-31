#pragma once
# include "validator.hpp"

enum class FailureCause;

class ParseException : public std::exception
{
    private:
        std::string errMsg;
        std::optional<FailureCause> cause;
    public:
        ParseException(std::string const &msg, FailureCause &cause) : errMsg(msg), cause(cause) {}
        const std::string what() noexcept {
            return "Error while parsing:\n" + this->errMsg;
        }
        ParseException(std::string const &msg) : errMsg(msg) {}
};

class AlgoException : public std::exception
{
    private:
        std::string errMsg;
    public:
        AlgoException(const std::string msg) : errMsg(msg) {}
        const std::string what() noexcept
        {
            return "Error while handling drones: " + this->errMsg;
        }
};

class StateException : public std::exception
{
    private:
        std::string errTemplate;
    public:
        StateException(std::string const &msg) : errTemplate(msg) {}
};