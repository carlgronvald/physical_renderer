#pragma once
#include "glm/glm.hpp"
#include "Geometry.h"

class Sphere : Geometry
{
public:
	glm::vec3 Center;
	float Radius;

	Sphere(glm::vec3 center, float radius) : Center(center), Radius(radius) { }

	float GetCollision(Ray ray) {
		glm::vec3 D = ray.Direction;
		glm::vec3 Delta = ray.Source - Center;
		float delta = (glm::dot(D,Delta)*glm::dot(D,Delta)-glm::dot(D,D)*(glm::dot(Delta,Delta)-Radius*Radius));
		if (delta >= 0) {
			return -(glm::dot(D, Delta) + sqrt(delta)) / glm::dot(D, D);
		}
		return -1;
	}

};

