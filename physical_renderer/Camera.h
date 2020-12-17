#pragma once
#include "glm/glm.hpp"
#include "Ray.h"

#define PI 3.141593

class Camera
{
public:
	glm::vec3 Source, Direction, Right, Up;
	float FOV;

	Camera(glm::vec3 source, glm::vec3 direction, glm::vec3 up, float fov) {
		this->Source = source;
		this->Direction = direction;
		this->Up = up;
		this->Right = glm::cross(direction, up);//*(float)tan(fov/180*PI);
		this->Up = this->Up;// *(float)tan(fov / 180 * PI);
		this->FOV = fov;
	}

	Ray GetRay(float x, float y) {
		glm::vec3 dir = (Direction + x * Right + y * Up) - Source;
		
		return Ray(Source, dir/glm::length(dir));
	}
};

