//
// Created by brogaar on 2-7-26.
//

#include "../reference/fly_in.hpp"

template<class C>
Data<C> Data<C>::init() {
    Data data;
    if (typeid(C) == typeid(Connection))
    {
        data = ConnectionData();
        data.setLimit(3);
    }
    else if (typeid(C) == typeid(Hub))
    {
        data = HubData();
        data.setLimit(6);
    }
    else
        data = nullptr;
    return data;
}

template <class C>
void Data<C>::setLimit(int id)
{
    this->limit = id;
}

template<class C>
template<typename T>
void Data<C>::insert(T &type)
{
    if (this->index < this->limit)
    {
        get<this->index>();
        ++this->index;
    }
    else {
        cerr << "ERROR INSERTING DATA ROW FROM MAP" << endl;
    }
}

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

string join(const vector<string>& parts, const char delimiter, const unsigned int size) {
    if (parts.empty()) return "";
    const unsigned int limit = (size == 0 || size > parts.size()) ? parts.size() : size;
    std::string result = parts[0];
    for (unsigned int i = 1; i < limit; ++i) {
        result += delimiter;
        result += parts[i];
    }
    return result;
}