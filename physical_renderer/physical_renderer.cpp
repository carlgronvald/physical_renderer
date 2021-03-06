// physical_renderer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#include "Camera.h"
#include "Statics.h"
#include "Sphere.h"
#include "Texture.h"
#include "Shader.h"
#include "Buffer.h"
#include "Geometry.h"
#include "Plane.h"
#include "Color.h"
#include "Light.h"
#include "Box.h"

#include "Test.h"




#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


struct Collision {
    int collidedGeometry;
    float t;

    Collision(int collidedGeometry, float t) : collidedGeometry(collidedGeometry), t(t) {

    }
};



class App {
private:
    GLFWwindow* window;
    int width = 700, height = 700;
    int raycastHeight = 100, raycastWidth = 100;
    Texture raycastTexture;
    Camera camera;
    Shader debugShader;
    Buffer dqBuffer;
    float deltaTime, lastTime;
    std::vector<Geometry*> geometries;
    std::vector<Light*> lights;
    int curPic = 0;
    int threadCount = 4;

    TestEquations eqTest;

    void SavePicture() {
        unsigned char* pixels;
        int screenStats[4];

        //Generate pixel array
        pixels = new unsigned char[width*height * 3];
        glReadPixels(0, 0, width,height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        stbi_write_png(("screenshots/shot" + std::to_string(curPic++) + ".png").c_str(), width,height, 3, pixels, width * 3);
        std::cout << "Wrote file " << "screenshots/shot" << curPic - 1 << ".png" << std::endl;
        delete[] pixels;
    }

    void SaveCamera() {
        std::ofstream wf("camera.dat", std::ios::out | std::ios::binary);
        wf.write((char*)&camera, sizeof(Camera));
        std::cout << "Saved Camera!" << std::endl;
    }

    void LoadCamera() {
        std::ifstream rf("camera.dat", std::ios::out | std::ios::binary);
        rf.read((char*)&camera, sizeof(Camera));
        std::cout << "Loaded Camera!" << std::endl;
    }
    int InitTextures() {
        raycastTexture = Texture(raycastWidth, raycastHeight, GL_RGB, GL_RGB, NULL);

        return 0;
    }

    int InitShaders() {
        debugShader = Shader("Shaders/DebugVertexShader.vertexshader", "Shaders/DebugFragmentShader.fragmentshader");
        debugShader.CreateUniformLocation("tex");
        return 0;
    }
    
    int Init() {

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(width, height, "Physical Renderer", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glewExperimental = true;
        /* Now we can init GLEW, since we have a valid context */
        if (glewInit() != GLEW_OK) {
            std::cout << "Error" << std::endl;
        }

        glPointSize(8);
        glClearColor(105 / 255.f, 189 / 255.f, 216 / 255.f, 1); //105/255.f, 189/255.f, 216/255.f
        glDepthRange(0.1, 100);
        //        glEnable(GL_DEPTH_TEST);
        //        glDepthFunc(GL_LESS);


        if (InitTextures() != 0)
            return -1;
        if (InitShaders() != 0) {
            return -1;
        }
        
        float debugQuad[18] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
        };

        dqBuffer = Buffer(sizeof(float) * 3, 6, &debugQuad[0], 0);

        geometries = std::vector<Geometry*>(0);
        
        Material matte = Material(Color(1.0,1.0,1.0), 0.66, Color(1, 1, 1), 0.34, 0.05);
        Material reflective = Material(Color(0.4, 0.5, 0.3), 0.5, Color(1, 1, 1), 0.5, 0.7);
        Material completeReflection = Material(Color(0.05, 0.1, 0.05), 0.8, Color(1, 1, 1), 0.2, 0.9);
        Material dielectric = Material(Color(0.4, 0.5, 0.3), 0.5, Color(1, 1, 1), 0.5, 0.3, 11.1); // Shadows are broken for dielectric
        Material matte2 = Material(Color(1.0, 1.0, 1.0), 0.8, Color(1, 1, 1), 0.2, 0.01);
        Material matte3 = Material(Color(1.0, 1.0, 1.0), 0.8, Color(1, 1, 1), 0.2, 0.01);

        geometries.push_back(new Sphere(glm::vec3(25, 0, 0), 4, reflective));
        geometries.push_back(new Sphere(glm::vec3(24, -9, 0), 4, reflective));
        geometries.push_back(new Sphere(glm::vec3(32, 3, 0), 3, reflective));
        geometries.push_back(new Sphere(glm::vec3(18, -3, 8), 4, matte2));
        geometries.push_back(new Sphere(glm::vec3(27, 2, -8), 6, reflective));
        geometries.push_back(new Box(glm::vec3(35, 2, -8), 6,3,4, reflective));
        geometries.push_back(new Plane(glm::vec3(0, 10, 0), glm::vec3(0,-1,0), completeReflection));
        geometries.push_back(new Plane(glm::vec3(0, -50, 0), glm::vec3(0, 1, 0), completeReflection));
        geometries.push_back(new Plane(glm::vec3(-50, 0, 0), glm::vec3(1, 0, 0), matte));
        geometries.push_back(new Plane(glm::vec3(0, 0, 50), glm::vec3(0, 0, -1), matte2));
        geometries.push_back(new Plane(glm::vec3(0, 0, -50), glm::vec3(0, 0, 1), matte3));
        geometries.push_back(new Plane(glm::vec3(50, 0, 0), glm::vec3(-1, 0, 0), matte));

        lights = std::vector<Light*>(0);
        lights.push_back(new Light(glm::vec3(10, -20, 0), Color(0,0,0.1)));
        lights.push_back(new Light(glm::vec3(40, -10, 0), Color(1, 1, 1)));
        lights.push_back(new Light(glm::vec3(0, -10, 40), Color(0.2, 0.2, 0.0)));


        return 0;
    }

    glm::vec3 RandomlyRotatedVector(glm::vec3 in) {
        glm::vec3 c1(0);
        if (abs(glm::dot(in, glm::vec3(0, 1, 0))) < 0.7) {
            c1 = glm::vec3(0, 1, 0);
            c1 = glm::normalize(c1 - glm::dot(c1, in) * in);
        }
        else {
            c1 = glm::vec3(1, 0, 0);
            c1 = glm::normalize(c1 - glm::dot(c1, in) * in);
        }
        glm::vec3 c2 = glm::cross(in, c1);

        float yaw = (RandomNumber()) * PI;
        float pitch = (RandomNumber()) * PI;
        return (cos(yaw) * c1 + sin(yaw) * c2) * cos(pitch) + in * sin(pitch);
    }

    Collision GetCollision(Ray r, int ignoredGeom=-1) {

        float t = -1;
        float tmp = 0;
        int collidedGeometry = 0;
        for (int loop = 0; loop < geometries.size(); loop++) {
            if (loop == ignoredGeom)
                continue;
            tmp = geometries[loop]->GetCollision(r);
            if (t < 0) {
                t = tmp;
                collidedGeometry = loop;
            }
            if (tmp >= 0 && tmp < t) {
                t = tmp;
                collidedGeometry = loop;
            }
        }

        return Collision(collidedGeometry, t);
    }

    glm::vec3 RefractedDir(glm::vec3 dir, glm::vec3 norm, float costheta, float earlyN, float lateN) {
        float inner = 1 - (earlyN * earlyN * (1 - pow(glm::dot(dir, norm), 2))) / (lateN * lateN);
        if (inner < 0)
            return glm::vec3(0);
        return glm::normalize(earlyN * (dir - norm * (glm::dot(dir, norm))) / lateN - norm*inner);
    }

    Color InternalRefract(glm::vec3 loc, glm::vec3 t, int geometry, int jumps) {

        Material* m = geometries[geometry]->GetMaterial();
        Ray internalRay = Ray(loc + 0.0001f * t, t);

        float col = geometries[geometry]->GetCollision(internalRay);
        loc = internalRay.GetLocation(col);
        glm::vec3 norm = geometries[geometry]->GetNormal(loc);
        float costheta = glm::dot(t, norm);
        glm::vec3 dir = RefractedDir(t, norm, costheta, m->RefractiveIndex, 1);
        if (dir == glm::vec3(0))
            if (jumps > 0)
                return InternalRefract(loc, t - 2 * glm::dot(t, -norm) * (-norm), geometry, jumps - 1);
            else
                return m->DiffuseColor;
        return CastRay(Ray(loc, -dir), jumps, geometry);
    }

    Color Refract(glm::vec3 loc, glm::vec3 dir, glm::vec3 norm, float costheta, int geometry, int jumps) {

        Material* m = geometries[geometry]->GetMaterial();
        glm::vec3 t = RefractedDir(dir, norm, costheta, 1, m->RefractiveIndex);

        return InternalRefract(loc, t, geometry, jumps);
        /*
        Ray internalRay = Ray(loc + 0.0001f * t, t);

        float col = geometries[geometry]->GetCollision(internalRay);
        loc = internalRay.GetLocation(col);
        norm = geometries[geometry]->GetNormal(loc);
        costheta = glm::dot(t, norm);
        dir = RefractedDir(t, norm, costheta, m->RefractiveIndex, 1);
        if (dir == glm::vec3(0))
            return Refract(loc, t-2*glm::dot(t, norm)*norm, norm, )
            return Color(0, 0, 0);
        return CastRay(Ray(loc, dir), jumps, geometry);*/
    }

    Color CastRay(Ray r, int jumps, int excludedGeom=-1) {
        float color[3];
        float mod;

        color[0] = 0; color[1] = 0; color[2] = 0;
        mod = 1;
        bool collide = true;

        Collision c = GetCollision(r, excludedGeom);
        if (c.t >= 0.0) { //Collided with something.
            glm::vec3 loc = r.GetLocation(c.t);
            glm::vec3 norm = geometries[c.collidedGeometry]->GetNormal(loc);
            Material* m = (geometries[c.collidedGeometry]->GetMaterial());

            glm::vec3 dir = r.Direction;
            dir = dir - 2 * glm::dot(dir, norm) * norm;
            Color addColor = Color(0, 0, 0);
            if (jumps > 0) {
                if (m->RefractiveIndex < 0) {
                    addColor = CastRay(Ray(loc, dir), jumps - 1, c.collidedGeometry);
                }
                else {
                    float costheta = glm::dot(-dir, norm); //TODO: REFLECTION CHANCE
                    //if (RandomNumber() < m->GetR0() + (1 - m->GetR0()) * pow((1 - costheta), 5))
                    //    addColor = CastRay(Ray(loc, dir), jumps - 1, c.collidedGeometry);
                    //else
                    addColor = Refract(loc, r.Direction, norm, costheta, c.collidedGeometry, jumps - 1);
                }
            }
            
            Color objColor = Color(0, 0, 0,c.collidedGeometry);

            for (int loop = 0; loop < lights.size(); loop++) {

                dir = lights[loop]->GetLocation() - loc;
                float len = glm::length(dir);
                dir = dir / len;
                r = Ray(loc, dir);
                c = GetCollision(r, objColor.Geom);
                if (c.t < 0.0 || c.t > len) {
                    glm::vec3 h = (-r.Direction + dir) * 0.5f;

                    Color cDiff(0, 0, 0);

                    float specular = pow(glm::dot(norm, h), 3);
                    if (specular < 0)
                        specular = 0;


                    float diffuse = glm::dot(norm, dir);
                    if (diffuse < 0.3)
                        diffuse = 0.3;
                    cDiff = m->DiffuseColor * diffuse;
                     
                    Color cSpec = m->SpecularColor * specular;

                    //return Color(0, 1, diffuse);
                    objColor = objColor + (cDiff * m->DiffuseAmount + cSpec * m->SpecularAmount) * lights[loop]->GetColor();
                }
            }
            return objColor * (1 - m->Reflectance) + addColor * m->Reflectance;
        }
        else { //Didn't collide with anything
            return Color(0, 0, 0.5);
        }
        return Color(0, 0, 0);
    }

    float RandomNumber() {
        return (float)rand() / RAND_MAX;
    }

    void PartialRaycast(int w, int h, int startX, int endX, char* raycastData, int AAlevel = 1, int jumps = 0) {

        float baseX = -1 + 1 / (float)w;
        float baseY = -1 + 1 / (float)h;
        float xmod = 2 / (float)w;
        float ymod = 2 / (float)h;

        double time = millis();

        float color[3];
        float mod;
        Ray r(glm::vec3(0), glm::vec3(0));
        for (int i = startX; i < endX; i++) {
            for (int j = 0; j < h; j++) {
                Color c(0, 0, 0);
                if (AAlevel == 1) {
                    c = CastRay(camera.GetRay(baseX + i * xmod, baseY + j * ymod), jumps);
                }
                else {
                    for (float k = 0; k < AAlevel; k++) {
                        for (float s = 0; s < AAlevel; s++) {
                            Color add = CastRay(camera.GetRay(baseX + (i + k / AAlevel) * xmod, baseY + (j + s / AAlevel) * ymod), jumps);
                            //TODO: I think everything may be pushed 0.5 pixels in both directions because of AA stuff.
                            if (add.Geom <22)
                                c = c + add%1;
                            else
                                c = c + Color(1, 1, 1);
                        }
                    }
                    c = c * (1.0 / (AAlevel * AAlevel));
                }
                raycastData[(i + j * w) * 3] = c.R * 255;
                raycastData[(i + j * w) * 3 + 1] = c.G * 255;
                raycastData[(i + j * w) * 3 + 2] = c.B * 255;
            }
            if (millis() - time > 1) {
                std::cout << "At i=" << i << std::endl;
                time = millis();
            }
        }
    }

    void Raycast(int w, int h, int AAlevel=1, int jumps=0) {

        double time = millis();

        char* raycastData = new char[w * h * 3];

        std::vector<std::thread*> threads(0);
        for (int i = 0; i < threadCount; i++) {
            threads.push_back(new std::thread(&App::PartialRaycast, this, w, h, i * w / threadCount, (i + 1) * w / threadCount, &raycastData[0], AAlevel, jumps));
        }
        
        for (int i = 0; i < threadCount; i++) {
            threads[i]->join();
            delete threads[i];
        }
        raycastTexture.Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, raycastTexture.getFormat(), w, h, 0, raycastTexture.getType(), GL_UNSIGNED_BYTE, raycastData);

        std::cout << "Finished in " << millis() - time << " seconds" << std::endl;

        delete[] raycastData;
    }

    void RunTest(int w, int h, Test &test) {
        double time = millis();
        char* data = new char[w * h * 3];
        test.run(w, h, data, 2);

        raycastTexture.Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, raycastTexture.getFormat(), w, h, 0, raycastTexture.getType(), GL_UNSIGNED_BYTE, data);

        std::cout << "Finished in " << millis() - time << " seconds" << std::endl;

        delete[] data;
    }


    void DrawTextureOnScreen(Texture texture, int x, int y, int width, int height) {

        glViewport(x, y, width, height);

        debugShader.Bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.GetId());
        glUniform1i(debugShader.GetUniformLocation("tex"), 0);

        dqBuffer.Bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        dqBuffer.Unbind();
    }

    void Controls() {
        deltaTime = millis() - lastTime;
        lastTime = millis();
        bool needsUpdate = false;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.Translate(glm::vec3(+5*deltaTime, 0, 0));
            needsUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.Translate(glm::vec3(-5 * deltaTime, 0, 0));
            needsUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.Translate(glm::vec3(0, 0, 5 * deltaTime));
            needsUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.Translate(glm::vec3(0, 0, -5 * deltaTime));
            needsUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            camera.Rotate(1 * deltaTime,0);
            needsUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            camera.Rotate(-1 * deltaTime,0);
            needsUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            camera.Rotate(0, 1 * deltaTime);
            needsUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            camera.Rotate(0, -1 * deltaTime);
            needsUpdate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            SaveCamera();
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            LoadCamera();
            needsUpdate = true;
        }

        //Raycasts below
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            Raycast(width,height,2,24);
        }
        else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
            Raycast(raycastWidth, raycastHeight, 10,9);
        }
        else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            SavePicture();
        }
        else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            RunTest(width, height, eqTest);
        }
        else if (needsUpdate)
            Raycast(raycastWidth,raycastHeight, 1, 3);
    }

public:
    int Run() {
        if (Init() != 0)
            return -1;
        
        Raycast(raycastWidth,raycastHeight);

        while (!glfwWindowShouldClose(window)) {

            Controls();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, width, height);

            DrawTextureOnScreen(raycastTexture, 0, 0, width, height);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        return 0;
    }

    App() : camera(glm::vec3(0, 0, 0), 0,0, 53.5) {
        lastTime = millis();
    }
};

int main()
{
    App app;
    
    return app.Run();
    
    return 0;
}

