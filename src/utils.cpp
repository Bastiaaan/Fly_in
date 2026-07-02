//
// Created by brogaar on 2-7-26.
//

#include "../reference/fly_in.hpp"

vector<string> split(const string &s, char delimiter, unsigned int size)
{
    vector<string> parts;
    stringstream ss(s);
    string item;
    if (size == 0)
    {
        while(std::getline(ss, item, delimiter))
            parts.push_back(item);
    }
    else
    {
        while(std::getline(ss, item, delimiter) && size != 0)
        {
            parts.push_back(item);
            size--;
        }
    }
    return parts;
}

string join(const vector<string>& parts, char delimiter, unsigned int size) {
    if (parts.empty()) return "";
    unsigned int limit = (size == 0 || size > parts.size()) ? parts.size() : size;
    std::string result = parts[0];
    for (unsigned int i = 1; i < limit; ++i) {
        result += delimiter;
        result += parts[i];
    }
    return result;
}