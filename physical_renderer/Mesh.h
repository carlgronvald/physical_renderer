#pragma once
#include "Geometry.h"
#include "Sphere.h"
class Mesh :
    public Geometry
{
private:
    Sphere boundingSphere;
public:
	float GetCollision(Ray ray) {
		if (boundingSphere.GetCollision(ray) < 0)
			return -1;

	}

	glm::vec3 GetNormal(glm::vec3 position) {
		return glm::vec3(0);
	}

};

