#pragma once
#include "Color.h"

class Material
{
public:
	Color DiffuseColor, SpecularColor;
	float DiffuseAmount, SpecularAmount;
	float Reflectance;
	float RefractiveIndex;


	Material(Color diffuseColor, float diffuseAmount, Color specularColor, float specularAmount, float reflectance,
		float refractiveIndex = -1) : 
		DiffuseColor(diffuseColor), DiffuseAmount(diffuseAmount), SpecularColor(specularColor), SpecularAmount(specularAmount),
		Reflectance(reflectance), RefractiveIndex(refractiveIndex)
	{

	}

	float GetR0() {
		return (RefractiveIndex - 1) * (RefractiveIndex - 1) / ((RefractiveIndex + 1) * (RefractiveIndex + 1));
	}
};

