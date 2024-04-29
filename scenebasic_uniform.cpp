#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>
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
    time(0.0f), angle(90.0f), emitterAngle(0.0f), drawBuf(1), 
    deltaT(0.0f), rotSpeed(glm::pi<float>()/8.0f), camera(), sky(100.0f), nParticles(4000), nParticlesFire(4000){

    castle = ObjMesh::load("media/castle.obj", false, true);
    dragon = ObjMesh::load("media/dragon.obj", false, true);
    floor = ObjMesh::load("media/cobble.obj", false, true);
    rock = ObjMesh::load("media/rock.obj", false, true);
}

void SceneBasic_Uniform::initScene()
{
    compile();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    textures.push_back(Texture::loadTexture("media/texture/dragon.png"));
    textures.push_back(Texture::loadTexture("media/texture/cobble.jpg"));
    textures.push_back(Texture::loadTexture("media/texture/castle.png"));
    textures.push_back(Texture::loadTexture("media/texture/castle-norm.png"));
    textures.push_back(Texture::loadTexture("media/texture/unbound_normal.png"));
    textures.push_back(Texture::loadTexture("media/texture/rock.jpg"));
    textures.push_back(Texture::loadTexture("media/texture/fire.png"));
    textures.push_back(ParticleUtils::createRandomTex1D(nParticles * 3));
    textures.push_back(ParticleUtils::createRandomTex1D(nParticlesFire * 3));

    const float particleLifetime = 2.0f;
    initBuffers(posBufParticle, velBufParticle, ageParticle, feedbackParticle, particleArray, nParticles, 2.5f);

    particleProg.use();
    particleProg.setUniform("RandomTex", 1);
    particleProg.setUniform("ParticleTex", 0);
    particleProg.setUniform("ParticleLifetime", particleLifetime);
    particleProg.setUniform("ParticleSize", 0.1f);
    particleProg.setUniform("Accel", vec3(1.0f, -5.0f, -5.0f));
    particleProg.setUniform("Emitter", vec3(-20, 10.8, 25));
    particleProg.setUniform("EmitterBasis", ParticleUtils::makeArbitraryBasis(vec3(100, 0, 0)));

    const float fireLifetime = 1.0f;
    initBuffers(posBufFire, velBufFire, ageParticleFire, feedbackParticleFire, particleArrayFire, nParticlesFire, 5.5f);

    fireProg.use();
    fireProg.setUniform("RandomTex", 1);
    fireProg.setUniform("ParticleTex", 0);
    fireProg.setUniform("ParticleLifetime", fireLifetime);
    fireProg.setUniform("ParticleSize", 0.5f);
    fireProg.setUniform("Accel", vec3(0.0f, 5.0f, 0.0f));
    fireProg.setUniform("Emitter", vec3(1.0f, 0.1f, -1.0f));
    fireProg.setUniform("EmitterBasis", ParticleUtils::makeArbitraryBasis(vec3(100, 0, 0)));

    skyboxTex = Texture::loadHdrCubeMap("media/texture/cube/sky-hdr/sky");

    projection = mat4(1.0f);

    prog.use();
    prog.setUniform("Light[0].L", vec3(0.475f, 0.149f, 0.031f));
    prog.setUniform("Light[0].La", vec3(0.07f));

    prog.setUniform("Light[1].L", vec3(0.945f, 0.173f, 0.051f));
    prog.setUniform("Light[1].La", vec3(0.07f));

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


        skyboxShader.compileShader("shader/skybox.vert");
        skyboxShader.compileShader("shader/skybox.frag");
        skyboxShader.link();

        particleProg.compileShader("shader/particle.vert");
        particleProg.compileShader("shader/particle.frag");

        GLuint progHandle = particleProg.getHandle();
        const char* outputNames[] = { "Position", "Velocity", "Age" };
        glTransformFeedbackVaryings(progHandle, 3, outputNames, GL_SEPARATE_ATTRIBS);

        particleProg.link();

        fireProg.compileShader("shader/fire.vert");
        fireProg.compileShader("shader/fire.frag");

        GLuint fireProgHandle = fireProg.getHandle();
        glTransformFeedbackVaryings(fireProgHandle, 3, outputNames, GL_SEPARATE_ATTRIBS);

        fireProg.link();
        fireProg.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t) {
    deltaT = t - time;;

    angle += 0.01f * deltaT;
    if (this->m_animate) {
        angle += rotSpeed * deltaT;
        if (angle > glm::two_pi<float>()) {
            angle -= glm::two_pi<float>();
        }
    }

    time = t;
    emitterAngle = std::fmod(emitterAngle + 0.01f, glm::two_pi<float>());

}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    particleProg.use();
    particleProg.setUniform("Time", time);
    particleProg.setUniform("DeltaT", deltaT);
    particleProg.setUniform("Pass", 1);

    particleTransformFeedback(feedbackParticle, particleArray, drawBuf, nParticles);

    fireProg.use();
    fireProg.setUniform("Time", time);
    fireProg.setUniform("DeltaT", deltaT);
    fireProg.setUniform("Pass", 1);

    particleTransformFeedback(feedbackParticleFire, particleArrayFire, drawBuf, nParticlesFire);

    view = camera.update();

    prog.use();
    prog.setUniform("Light[0].Position", view * glm::vec4(10.0f * cos(angle), 1.0f, 10.0f * sin(angle), 1.0f));
    prog.setUniform("Light[1].Position", view * glm::vec4(14.0f, 2.0f, 20.0f, 1.0f));

    skyboxShader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[4]);

    model = mat4(1.0f);
    setMatrices(skyboxShader);
    sky.render();

    prog.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[4]);

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.45f, 0.45f, 0.45f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Shininess", 10.0f);
 
    model = mat4(1.0f);
    model = glm::translate(model, vec3(3.0f, 3.0f, 11.0f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    model = glm::rotate(model, 200.0f, vec3(0.0f, -1.0f, 0.0f));
    setMatrices(prog);
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
            setMatrices(prog);
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
    setMatrices(prog);
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
        setMatrices(prog);
        rock->render();

        rockZ += 1.0f;
    }

    float rockX = -1.0f;
    for (int i = 0; i < 14; i++) {
        model = mat4(1.0f);
        model = glm::translate(model, vec3(rockX, 0.2f, 19.0f + rockOffset[i]));
        model = glm::scale(model, vec3(0.25f, 0.25f, 0.25f));
        model = glm::rotate(model, rockRotation[i], vec3(0.0f, 1.0f, 0.0f));
        setMatrices(prog);
        rock->render();

        rockX += 1.0f;
    }

    particleProg.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[6]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, textures[7]);

    particleProg.setUniform("Pass", 2);
    setMatrices(particleProg);

    model = mat4(1.0f);

    glDepthMask(GL_FALSE);
    glBindVertexArray(particleArray[drawBuf]);
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    fireProg.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[6]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, textures[8]);

    fireProg.setUniform("Pass", 2);
    setMatrices(fireProg);

    model = mat4(1.0f);

    glDepthMask(GL_FALSE);
    glBindVertexArray(particleArrayFire[drawBuf]);
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    drawBuf = 1 - drawBuf;
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;

    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices(GLSLProgram & program)
{
    mat4 mv = view * model;
    program.setUniform("ModelViewMatrix", mv);
    program.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    program.setUniform("MVP", projection * mv);
    program.setUniform("Projection", projection);
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

void SceneBasic_Uniform::initBuffers(GLuint* posBuf, GLuint* velBuf, GLuint* age, GLuint* feedback, GLuint* particleArray, int nParticles, float particleLifetime) {
    glGenBuffers(2, posBuf);
    glGenBuffers(2, velBuf);
    glGenBuffers(2, age);
    int size = nParticles * 3 * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, age[0]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, age[1]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), 0, GL_DYNAMIC_COPY);

    std::vector<GLfloat> tempData(nParticles);
    float rate = particleLifetime / nParticles;
    for (int i = 0; i < nParticles; i++) {
        tempData[i] = rate * (i - nParticles);
    }
    Random::shuffle(tempData);
    
    glBindBuffer(GL_ARRAY_BUFFER, age[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), tempData.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(2, particleArray);
    glBindVertexArray(particleArray[0]);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, age[0]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(particleArray[1]);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, age[1]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    glGenTransformFeedbacks(2, feedback);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void SceneBasic_Uniform::particleTransformFeedback(GLuint* feedback, GLuint* particleArray, GLuint drawBuf, int nParticles) {
    glEnable(GL_RASTERIZER_DISCARD);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
    glBeginTransformFeedback(GL_POINTS);
    glBindVertexArray(particleArray[1 - drawBuf]);
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    glVertexAttribDivisor(2, 0);
    glDrawArrays(GL_POINTS, 0, nParticles);
    glBindVertexArray(0);
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);
}
