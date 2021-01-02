#pragma once
#include "glm/glm.hpp"
#include "Ray.h"
#include <iostream>
#include <fstream>
#include "Statics.h"

#define PI 3.141593

class Camera
{
private:
	glm::vec3 Source, Direction, Right, Up;
	float Yaw, Pitch;

	void UpdateVectors() {
		this->Direction = glm::vec3(cos(Yaw) * cos(Pitch), sin(Pitch), sin(Yaw) * cos(Pitch));
		this->Up = glm::vec3(cos(Yaw) * cos(Pitch+PI/2), sin(Pitch+PI/2), sin(Yaw) * cos(Pitch+PI/2));
		this->Right = glm::cross(Direction, Up);

	}
public:
	float FOV;

	Camera(glm::vec3 source, float yaw, float pitch, float fov) {
		this->Source = source;
		this->Yaw = yaw;
		this->Pitch = pitch;
		UpdateVectors();
		this->FOV = fov;

	}

	Ray GetRay(float x, float y) {
		glm::vec3 dir = (Direction + x * Right + y*Up);
		
		return Ray(Source, dir/glm::length(dir));
	}

	void Translate(glm::vec3 translation) {
		Source += translation.x * Right + translation.y * Up + translation.z * Direction;
	}

	void Rotate(float yaw, float pitch) {
		this->Yaw += yaw;
		this->Pitch += pitch;
		UpdateVectors();
	}

};

