#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "glm/glm.hpp"

std::string readFile(const char* filename);

std::string vecstr(glm::vec3 vec);
double millis();