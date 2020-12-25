#pragma once
#include "glm/glm.hpp"
#include "Geometry.h"

class Sphere : public Geometry
{
public:
	glm::vec3 Center;
	float Radius;

	Sphere(glm::vec3 center, float radius, Material mat) : Geometry(mat), Center(center), Radius(radius) { }

	float GetCollision(Ray ray) {
		glm::vec3 D = ray.Direction;
		glm::vec3 Delta = ray.Source - Center;
		float delta = (glm::dot(D,Delta)*glm::dot(D,Delta)-glm::dot(D,D)*(glm::dot(Delta,Delta)-Radius*Radius));
		if (delta >= 0) {
			if(-(glm::dot(D, Delta) + sqrt(delta)) / glm::dot(D, D) >= 0)
				return -(glm::dot(D, Delta) + sqrt(delta)) / glm::dot(D, D);
			return -(glm::dot(D, Delta) - sqrt(delta)) / glm::dot(D, D);
		}
		return -1;
	}

	glm::vec3 GetNormal(glm::vec3 location) {
		return glm::normalize(location - Center);
	}

};

