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
    std::string line;
    std::string formatted; // pre-built message

    void buildMessage() {
        formatted = errMsg + " at line " + line + ".\n";
        if (cause != FailureCause::NoErrorsGiven) {
            formatted += "Reason: " + std::to_string(static_cast<int>(cause)) + "\n";
        }
    }

public:
    ParseException(std::string const &msg, FailureCause const &c, std::string const &line)
        : errMsg(msg), cause(c), line(line) { buildMessage(); }

    ParseException(std::string const &msg, std::string const &line)
        : errMsg(msg), line(line) { buildMessage(); }

    ParseException(std::string const &msg)
        : errMsg(msg) { buildMessage(); }

    const char *what() const noexcept override {
        return formatted.c_str();
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