#include <iostream>
#include <fstream>
#include <vector>
#include "hub.hpp"

class Data {
	public:
		int nb_drone;
		void read_data(std::string path);
		std::string get_path(int difficulty, int option);

};

Data::read_data(std::string path)
{
	ofstream MyFile(this->get_path);
	std::cout << "reading map" << std::endl;
	MyFile.close();
};

Data::get_path(int difficulty, int option)
{
	return "";
};