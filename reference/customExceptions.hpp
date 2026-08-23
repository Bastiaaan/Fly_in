#pragma once
#include <exception>
#include <optional>
#include <string>
#include "validator.hpp"

class ParseException : public std::exception
{
private:
    std::string const errMsg;
    FailureCause cause = FailureCause::NoErrorsGiven;
    std::string line;
public:
    ParseException(std::string const &msg, FailureCause const &c, std::string const &line)
        : errMsg(msg), cause(c), line(line) {}

    ParseException(std::string const &msg, std::string const line)
        : errMsg(msg), line(line) {}

    ParseException(std::string const &msg) : errMsg(msg) {};

    const char *what() const noexcept override
    {
        if (cause != FailureCause::NoErrorsGiven)
        {
            //errTemplate += "Reason";
        }
        const std::string errTemplate =
            errMsg + " at line " + line + ".\n";
        return errTemplate.c_str();
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