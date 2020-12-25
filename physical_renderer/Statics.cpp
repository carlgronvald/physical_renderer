#pragma once
#include "stdafx.h"
#include "Statics.h"

std::string readFile(const char* filename) {
    std::string text;
    std::ifstream myfile(filename);

    myfile.seekg(0, std::ios::end);
    text.reserve(myfile.tellg());
    myfile.seekg(0, std::ios::beg);

    text.assign((std::istreambuf_iterator<char>(myfile)),
        std::istreambuf_iterator<char>());
    return text;
}

std::string vecstr(glm::vec3 vec) {
    std::string result = "";
    result += "<";
    result += std::to_string(vec[0]);
    result += ",";
    result += std::to_string(vec[1]);
    result += ",";
    result += std::to_string(vec[2]);
    result += ">";
    return result;
}

double millis() {
    return clock() / (double)CLOCKS_PER_SEC;
}