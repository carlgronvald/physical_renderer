#pragma once
#include "Ray.h"
#include "Material.h"

class Geometry
{
private:
	Material mat;
public:
	//Returns the t=value where collision happens between given ray and the geometry
	//t<0 = no collision
	virtual float GetCollision(Ray ray) = 0;
	virtual glm::vec3 GetNormal(glm::vec3 position) = 0;
	Material* GetMaterial() {
		return &mat;
	}

	Geometry(Material mat) : mat(mat) {

	}
};

