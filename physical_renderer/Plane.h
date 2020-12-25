#pragma once
#include "Geometry.h"
class Plane :
	public Geometry
{
private:
	glm::vec3 Point, Normal;
public:

	Plane(glm::vec3 point, glm::vec3 normal, Material mat) : Geometry(mat), Point(point), Normal(normal) {

	}

	float GetCollision(Ray ray) {
		float denom = glm::dot(Normal, ray.Direction);
		if (denom < -1e-6) {
			return glm::dot(Point - ray.Source, Normal) / denom;
		}

		return -1;
	}

	glm::vec3 GetNormal(glm::vec3 position) {
		return Normal;
	}
};

