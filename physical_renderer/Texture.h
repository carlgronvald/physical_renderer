#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include<iostream>


class Texture
{
public:
	//Reads a PNG from file and makes a texture out of it.
	Texture();
//	Texture(std::string file, bool rgba = false);
	Texture(int width, int height, int format, int type, unsigned char* data);
	~Texture();

	unsigned int GetId();
	int getType();
	int getFormat();
	int getWidth();
	int getHeight();
	void Bind();
private:
	unsigned int id;
	int width, height;
	int type, format;
};
