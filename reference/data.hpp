
#ifndef DATA_HPP
# define DATA_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "hub.hpp"

template<typename T>
class Meta
{

};

class Data {
	public:
		int nb_drones;
		void read_data(std::string path);
		std::string get_path(int difficulty, int option);

};

Data::read_data(std::string path)
{
	std::cout << "reading map" << this->get_path << std::endl;
	ofstream MyFile(this->get_path);
	MyFile.close();
};

Data::get_path(int difficulty, int option)
{
    std::string _path = "../maps/"
	return "whatever";
};

#endif