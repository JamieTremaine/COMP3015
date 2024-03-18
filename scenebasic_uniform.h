#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "glm/glm.hpp"

#include "helper/plane.h"
#include "helper/objmesh.h"

#include "helper/teapot.h"

#include "helper/cube.h"

#include "helper/skybox.h"
#include "camera.h"

class SceneBasic_Uniform : public Scene
{
private:
    Camera camera;
    SkyBox sky;
    float tPrev;
    float angle;
    float rotSpeed;
    std::unique_ptr<ObjMesh> dragon;
    std::unique_ptr<ObjMesh> floor;
    std::unique_ptr<ObjMesh> castle;
    std::unique_ptr<ObjMesh> rock;
    GLSLProgram prog;
    std::vector<GLuint> textures;
    GLuint skyboxTex;
    float rockOffset[15];
    float rockRotation[15];
    double mouseX, mouseY;
    bool mousePressed;

    void setMatrices();
    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
    void handleKeyPress(int key);
    void handleMouseMove(double xPos, double yPos);
    void handleMouseButtonPress(int button, int action);
};

#endif // SCENEBASIC_UNIFORM_H
