#pragma once
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
