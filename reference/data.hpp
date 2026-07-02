
#ifndef DATA_HPP
# define DATA_HPP

#include "fly_in.hpp"
#include "system.hpp"

// template<typename T>
class Data {
	public:
        // T *dest;
		int nb_drones;
		void read_data(std::string &path);
		string get_path(int difficulty, int option);
        bool map_exists(std::string &difficulty, int level);
        // void map_meta_data(T type);
};

#endif