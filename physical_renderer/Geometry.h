#pragma once
#include "Ray.h"

class Geometry
{
public:
	//Returns the t=value where collision happens between given ray and the geometry
	//t<0 = no collision
	virtual float GetCollision(Ray ray) = 0;
};

