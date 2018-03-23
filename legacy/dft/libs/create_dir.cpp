#pragma once 

#include <cstdlib>
#include <string>

bool create_dir(const std::string& path) {
	return system((std::string("mkdir ")+path).c_str()) == -1;	
}