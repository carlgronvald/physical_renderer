#pragma once
#include "Geometry.h"
class Box :
    public Geometry
{
private:
	int W, H, D;
	glm::vec3 Position;
	//Bounds[0] is the minimum x,y,z position, Bounds[1] is the maximum.
	glm::vec3 Bounds[2];
public:

	Box(glm::vec3 position, int w, int h, int d, Material mat) : Geometry(mat), W(w), H(h), D(d), Position(position) {
		Bounds[0] = Position - glm::vec3(W, H, D) * 0.5f;
		Bounds[1] = Position + glm::vec3(W, H, D) * 0.5f;
	}

	float GetCollision(Ray ray) {
		int xsign, ysign, zsign;
		xsign = ray.Direction.x > 0 ? 0 : 1;
		ysign = ray.Direction.y > 0 ? 0 : 1;
		zsign = ray.Direction.z > 0 ? 0 : 1;
		// Create t bounds. Iteratively refine bounds, and check if bounds allow collision.
		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (Bounds[xsign].x - ray.Source.x) / ray.Direction.x;
		tmax = (Bounds[1 - xsign].x - ray.Source.x) / ray.Direction.x;

		tymin = (Bounds[ysign].y - ray.Source.y) / ray.Direction.y;
		tymax = (Bounds[1 - ysign].y - ray.Source.y) / ray.Direction.y;
		
		if (tmin > tymax || tmax < tymin) {
			return -1;
		}
		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax) {
			tmax = tymax;
		}

		tzmin = (Bounds[zsign].z - ray.Source.z) / ray.Direction.z;
		tzmax = (Bounds[1 - zsign].z - ray.Source.z) / ray.Direction.z;

		if (tmin > tzmax || tmax < tzmin) {
			return -1;
		}
		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		if (tmin > 0)
			return tmin;
		return tmax;

	}

	//Return a normal pointing away from the box.
	glm::vec3 GetNormal(glm::vec3 position) {
		//return glm::normalize(Position - position);
		
		glm::vec3 normal(0);
		if (position.x >= Bounds[1].x - 0.00001)
			normal.x = 1;
		else if (position.x <= Bounds[0].x + 0.00001)
			normal.x = -1;
		if (position.y >= Bounds[1].y - 0.00001)
			normal.y = 1;
		else if (position.y <= Bounds[0].y + 0.00001)
			normal.y = -1;
		if (position.z >= Bounds[1].z - 0.00001)
			normal.z = 1;
		else if (position.z <= Bounds[0].z + 0.00001)
			normal.z = -1;
		return glm::normalize(normal);
	}

	//Returns whether a given position is inside the box or not.
	bool Inside(glm::vec3 position) {
		if (position.x < Bounds[0].x)
			return false;
		if (position.x > Bounds[1].x)
			return false;
		if (position.y < Bounds[0].y)
			return false;
		if (position.y > Bounds[1].y)
			return false;
		if (position.z < Bounds[0].z)
			return false;
		if (position.z > Bounds[1].z)
			return false;
		return true;
	}
};

