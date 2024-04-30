# COMP3015


### Versions
VIsual Studio 2022 (Version 17.8.5)
Windows 10 (19045.4046)

## How does it work?
The inital template containing the scene, screnerunner and other helper classes was used. Initally the project follows a similar structure to those of the labs with a Light and Material struct. blinn-phong was implimented in the fragment shader with the ability to take in a diffuse texture and a normal texture. It is also able to impliment multiple lights, in this project two light sources have been used.

The scene is made up entierly of custom meshes, some like the cobblestone floor have been tiled multiple times. The rock meshes surrounding the scene are placed with a random offset and rotation, each time the scene is played a slightly different arrangment will be created.

Both keyboard and mouse controlshave been implemented using OpenGL callbacks. The scene and scenerunner files have been customized to create a callback on key presses, mouse button or mouse cursor changes which is then passed to the scene. A custom camera was also created to manage camera movement on keypress or mouse movement. Keyboard controls are WASD for strafing movement, QE for left/right panning RF for up/down movement and XC for up down panning. Mouse panning is on right mouse click and hold.

The vast majority of code is in the scenebasic_uniform files with some reusable components (camera) in their own files. A scene manages their own keyboard and mouse inputs, changes to scenerunner only catch the inputs and then pass them directly to the scene to manage. This allows the scene to be in control of its inputs, allowing for different keybindings between different scenes (for CW2). This is the same with the camera which alters and returns the view, it can be used in another scene without any changes.

Although the shader supports normal maps not all of the models used have a normal map. A common workaround that I used was to create a tiny 4x4 image that could be used as a placeholder normal map when one was not being used.

A skybox is also created using the same shader as the rest of the models. A float flag is used when the active nexture is the skybox texture and different shading approach is taken. The flag is then set to 0 when the skybox has finished rendering.

### Shaders
bliin-phong has been implmented in the fragment shader. Firstly normals, tangents and binomials are calculated in the vertex shader. Light directions for each light source are also calculated using the light position and the current position, then stored in an array. Other variables that are used in the fragment shader such as the vertex position are then initialized. A float called 'isSkybox' is used in the fragment shader to differenciate between skybox textures and other model textures. Skybox textures do not pass through binn-phong wheras all other items do. As mentioned blinn-phong shading is used in this shader, first the colour of the texture at the current position is added to the light ambient to create the ambient light. The diffuse is calculated slightly differently to apply toon shading. The floor of the dot product of the light direction and normal multiplied by the number of levels is then also multiplied by the scale factor of the number of levels to create the toon shading effect. All items that pass through the shader have gamma correction applied before they are returned. Toon shading has also been applied to the shader with three levels.

### Scene
The scene is of a dragon attacking a tower at dusk. It contains a tower model, a dragon model, tiled cobblstone ground and tiled rocks bordering the edge of the scene. A soft red glow shines from behind the tower to indicate the burning village that the dragon has just laid waste to. A second light pans across the scene to indicate dusk turning into night. The skybox is of a dark stormy night to set the scene of a dragon attack on a cold night.

### Usage
All models and textures come packed within the project. No external dependancies are required (apart from glfw). It is assumed that any OpenGL dependancies, GLFW, GLAD etc are in the user/public folder as required.

## CW2

### Vertex Animations
A particle fountain and a fire particle effect have been crated for the scene. Both are present in different shaders named particle and fire respectively. Both make use of their own buffers and can function independantly of one another. A shared function called initBuffers has been created where bufferes are passed as input parameters, vertex arrays are then created inside this function for the buffers that have been supplied. This function is called when the scene is initialized for both the particle fountain and fire particles. Both shaders implement transform feedback, once again a shared function has been created for this purpose that takes buffers as input parameters.

### Point sprites
Point sprites have been implemented for this scene. They have been implemented in their own shaders named 'point'. The number of sprites can be set in the custructor and the size of each sprite by setting a uniform value.

### Disintegration
Disintigration has been added to the scene, unlike other features disintigration has been implemented on top of the scenebasic_uniform shader created in CW1. Disintigration can optionally be toggled on or off in this shader through a flag uniform value 'Noise'. This allows any previously created mesh using this shader to optionally implement disintegration. high and low threshhold uniform values can then be set when the noise flag is set to provide discarding between a range of values.         


### Model accreditations

 - https://free3d.com/3d-model/low-poly-rock-for-games-9753.html
 - https://www.turbosquid.com/3d-models/game-ready-house-1431718
 - https://free3d.com/3d-model/cobblestones-3-86328.html
 - https://free3d.com/3d-model/dragon-18476.html
 - https://www.turbosquid.com/3d-models/photogrammetry-agisoft-model-1263969




