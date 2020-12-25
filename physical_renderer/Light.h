#pragma once
#include "glm/glm.hpp"
#include "Color.h"
class Light
{
private:
	glm::vec3 Location;
	Color color;
public:
	Light(glm::vec3 location, Color color) : Location(location), color(color) {

	}

	glm::vec3 GetLocation() {
		return Location;
	}

	Color GetColor() {
		return color;
	}
};

