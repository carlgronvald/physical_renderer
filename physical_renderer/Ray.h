#pragma once
#include "glm/glm.hpp"

class Ray
{
public:
	glm::vec3 Source;
	glm::vec3 Direction;

	Ray(glm::vec3 source, glm::vec3 direction) {
		this->Source = source;
		this->Direction = direction;
	}

	//Returns Source+t*Direction
	glm::vec3 GetLocation(float t) {
		return Source + t * Direction;
	}
};

