#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>

#include "helper/glutils.h"
#include "helper/texture.h"

#include <GLFW/glfw3.h>

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() :
    tPrev(0), angle(90.0f), rotSpeed(glm::pi<float>()/8.0f), camera(), sky(100.0f) {
    castle = ObjMesh::load("media/castle.obj", false, true);
    dragon = ObjMesh::load("media/dragon.obj", false, true);
    floor = ObjMesh::load("media/cobble.obj", false, true);
    rock = ObjMesh::load("media/rock.obj", false, true);
}

void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    textures.push_back(Texture::loadTexture("media/texture/dragon.png"));
    textures.push_back(Texture::loadTexture("media/texture/cobble.jpg"));
    textures.push_back(Texture::loadTexture("media/texture/castle.png"));
    textures.push_back(Texture::loadTexture("media/texture/castle-norm.png"));
    textures.push_back(Texture::loadTexture("media/texture/unbound_normal.png"));
    textures.push_back(Texture::loadTexture("media/texture/rock.jpg"));


    skyboxTex = Texture::loadHdrCubeMap("media/texture/cube/sky-hdr/sky");

    model = mat4(1.0f);
    view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    projection = mat4(1.0f);

    prog.setUniform("Light[0].L", vec3(0.475f, 0.149f, 0.031f));
    prog.setUniform("Light[0].La", vec3(0.05f));

    prog.setUniform("Light[1].L", vec3(0.945f, 0.173f, 0.051f));
    prog.setUniform("Light[1].La", vec3(0.05f));

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.45f, 0.45f, 0.45f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Shininess", 10.0f);

    for (int i = 0; i < 15; i++) {
         rockOffset[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (-0.1f - 0.1f));
         rockRotation[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 360.0f);
    }
}

void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t) {
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) {
        deltaT = 0.0f;
    }
    tPrev = t;
    angle += 0.01f * deltaT;
    if (this->m_animate) {
        angle += rotSpeed * deltaT;
        if (angle > glm::two_pi<float>()) {
            angle -= glm::two_pi<float>();
        }
   }

}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = camera.update();

    prog.setUniform("Light[0].Position", view * glm::vec4(10.0f * cos(angle), 1.0f, 10.0f * sin(angle), 1.0f));
    prog.setUniform("Light[1].Position", view * glm::vec4(14.0f, 2.0f, 20.0f, 1.0f));


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    prog.setUniform("renderSkyBox", 1.0f);

    model = mat4(1.0f);
    setMatrices();
    sky.render();

    prog.setUniform("renderSkyBox", 0.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[4]);

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.45f, 0.45f, 0.45f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Shininess", 10.0f);
 
    model = mat4(1.0f);
    model = glm::translate(model, vec3(5.0f, 3.0f, 7.0f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    setMatrices();
    dragon->render();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[4]);

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Shininess", 50.0f);


    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 20; j++)
        {
            model = mat4(1.0f);
            model = glm::translate(model, vec3(i * 1.0f, 0.0f, j * 1.0f));
            model = glm::scale(model, vec3(0.25f, 0.25f, 0.25f));
            setMatrices();
            floor->render();
        }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[2]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[3]);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(12.0f, 0.0f, 16.0f));
    model = glm::scale(model, vec3(0.25f, 0.25f, 0.25f));
    model = glm::rotate(model, 180.0f, vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    castle->render();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[5]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[3]);

    float rockZ = -1.0f;
    for (int i = 0; i < 15; i++) {
        model = mat4(1.0f);
        model = glm::translate(model, vec3(12.0f + rockOffset[i], 0.2f, rockZ));
        model = glm::scale(model, vec3(0.25f, 0.25f, 0.25f));
        model = glm::rotate(model, rockRotation[i], vec3(0.0f, 1.0f, 0.0f));
        setMatrices();
        rock->render();

        rockZ += 1.0f;
    }

    float rockX = -1.0f;
    for (int i = 0; i < 14; i++) {
        model = mat4(1.0f);
        model = glm::translate(model, vec3(rockX, 0.2f, 19.0f + rockOffset[i]));
        model = glm::scale(model, vec3(0.25f, 0.25f, 0.25f));
        model = glm::rotate(model, rockRotation[i], vec3(0.0f, 1.0f, 0.0f));
        setMatrices();
        rock->render();

        rockX += 1.0f;
    }



}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;

    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::handleKeyPress(int key) {
    switch (key)
    {
        case GLFW_KEY_W:
            camera.moveForward();
            break;
        case GLFW_KEY_A:
            camera.moveLeft();
            break;
        case GLFW_KEY_S:
            camera.moveBackward();
            break;
        case GLFW_KEY_D:
            camera.moveRight();
            break;
        case GLFW_KEY_Q:
            camera.rotateLeft(false);
            break;
        case GLFW_KEY_E:
            camera.rotateRight(false);
            break;
        case GLFW_KEY_R:
            camera.moveUp();
            break;
        case GLFW_KEY_F:
            camera.moveDown();
            break;
        case GLFW_KEY_Z:
            camera.rotateUp(false);
            break;
        case GLFW_KEY_X:
            camera.rotateDown(false);
            break;
        default:
            break;
    }
}

void SceneBasic_Uniform::handleMouseMove(double xPos, double yPos) {
    if (mousePressed == true) {
        if (xPos != mouseX) {
            if (xPos > mouseX) {
                camera.rotateRight(true);
            }
            else {
                camera.rotateLeft(true);
            }
        }
        if (yPos != mouseY) {
            if (yPos > mouseY) {
                camera.rotateUp(true);
            }
            else {
                camera.rotateDown(true);
            }
        }
        mouseX = xPos;
        mouseY = yPos;
    }
}

void SceneBasic_Uniform::handleMouseButtonPress(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}
