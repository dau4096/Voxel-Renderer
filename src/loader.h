#ifndef LOADER_H
#define LOADER_H


#include "types.h"


namespace load {

std::vector<uint8_t> file(const std::string& path);
void modelsFromFile(const std::string filePath);

}



#endif

