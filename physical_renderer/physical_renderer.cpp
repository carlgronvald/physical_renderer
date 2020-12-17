// physical_renderer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "Camera.h"
#include "Statics.h"
#include "Sphere.h"
class App {

};

int main()
{
    Camera c(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), 53.5);

    std::cout << vecstr(c.GetRay(-1, -1).Direction) << std::endl;
    Sphere p(glm::vec3(10, 0, 0), 3);
    float t = p.GetCollision(c.GetRay(0, 0));

    int width = 11, height = 11;
    float baseX = -1 + 1 / (float)width;
    float baseY = -1 + 1 / (float)height;
    float xmod = 2 / (float)width;
    float ymod = 2 / (float)height;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {

        }
    }

    std::cout << vecstr(c.GetRay(0, 0).GetLocation(t)) << std::endl;

    return 0;
}

