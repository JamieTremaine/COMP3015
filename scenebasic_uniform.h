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
#include "helper/random.h"
#include "helper/grid.h"
#include "helper/particleutils.h"
#include "helper/noisetex.h"
#include "camera.h"

class SceneBasic_Uniform : public Scene
{
private:
    Camera camera;
    SkyBox sky;
    float tPrev;
    float angle;
    float emitterAngle;
    float rotSpeed;
    std::unique_ptr<ObjMesh> dragon;
    std::unique_ptr<ObjMesh> floor;
    std::unique_ptr<ObjMesh> castle;
    std::unique_ptr<ObjMesh> rock;
    std::unique_ptr<ObjMesh> tree;
    GLSLProgram prog, skyboxShader, particleProg, fireProg, spriteProg;
    std::vector<GLuint> textures;
    GLuint skyboxTex;
    float rockOffset[15];
    float rockRotation[15];
    float time, deltaT;
    double mouseX, mouseY;
    bool mousePressed;
    Random randUtil;
    GLuint posBufParticle[2], velBufParticle[2], ageParticle[2], drawBuf, feedbackParticle[2], particleArray[2];
    GLuint posBufFire[2], velBufFire[2], ageParticleFire[2], feedbackParticleFire[2], particleArrayFire[2];
    int nParticles, nParticlesFire;
    int numSprites;
    float* locations;
    GLuint sprites;


    void setMatrices(GLSLProgram &);
    void compile();

    void initBuffers(GLuint* posBuf, GLuint* velBuf, GLuint* age, GLuint* feedback, GLuint* particleArray, int nParticles, float particleLifetime);
    void particleTransformFeedback(GLuint* feedback, GLuint* particleArray, GLuint drawBuf, int nParticles);

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
