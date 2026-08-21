#pragma once
#include <exception>
#include <optional>
#include <string>
#include "validator.hpp"

class ParseException : public std::exception
{
private:
    std::string errMsg;
    FailureCause cause = FailureCause::NoErrorsGiven;
public:
    ParseException(std::string const &msg, FailureCause const &c)
        : errMsg(msg), cause(c) {}

    ParseException(std::string const &msg)
        : errMsg(msg) {}

    const char *what() const noexcept override
    {
        if (cause != FailureCause::NoErrorsGiven)
        {
            //errTemplate += "Reason";
        }
        return errMsg.c_str();
    }
};

class AlgoException : public std::exception
{
    private:
        std::string errMsg;
    public:
        AlgoException(std::string const &msg)
			: errMsg("Couldn't proceed algorithm:\n" + msg) {}

        const char *what() const noexcept override
        {
            return errMsg.c_str();
        }
};

class StateException : public std::exception
{
    private:
        std::string errTemplate;
    public:
        StateException(std::string const &msg) : errTemplate(msg) {}
};