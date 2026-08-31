#pragma once
#include <exception>
#include <optional>
#include <string>

class ParseException : public std::exception
{
private:
    std::string errMsg;
    std::string line;
    std::string formatted; // pre-built message

    void buildMessage() {
        formatted = line.empty()
            ? errMsg + "\n"
            : errMsg + " at line " + line + ".\n";
    }

public:
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