#pragma once
#include "Geometry.h"
#include "Box.h"
class Voxelgrid :
    public Geometry
{
private:
	int W, H, D;
	char* Grid;
	glm::vec3 Position;
	Box BoundingBox;

public:

	Voxelgrid(glm::vec3 position, int w, int h, int d, Material mat) : Geometry(mat), W(w), H(h), D(d), Position(position), BoundingBox(position,w,d,h,mat) {
		Grid = new char[w * h * d];
	}

	~Voxelgrid() {
		delete[] Grid;
	}

	float GetCollision(Ray ray) {
		float firstCol = BoundingBox.GetCollision(ray);
		if (firstCol < 0)
			return -1;

		glm::vec3 rayPosition = ray.Source;
		float t = 0;

		if (!BoundingBox.Inside(ray.Source)) {
			rayPosition = ray.GetLocation(firstCol);
			t = firstCol;
		}

		return -1;
	}

	glm::vec3 GetNormal(glm::vec3 position) {

	}
};

