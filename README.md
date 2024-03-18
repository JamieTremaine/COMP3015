# COMP3015


### Versions
VIsual Studio 2022
Windows 10

## How does it work?
The inital template containing the scene, screnerunner and other helper classes was used. Initally the project follows a similar structure to those of the labs with a Light and Material struct. blinn-phong was implimented in the fragment shader with the ability to take in a diffuse texture and a normal texture. It is also able to impliment multiple lights, in this project two light sources have been used.

The scene is made up entierly of custom meshes, some like the cobblestone floor have been tiled multiple times. The rock meshes surrounding the scene are placed with a random offset and rotation, each time the scene is played a slightly different arrangment will be created.

Both keyboard and mouse controlshave been implemented using OpenGL callbacks. The scene and scenerunner files have been customized to create a callback on key presses, mouse button or mouse cursor changes which is then passed to the scene. A custom camera was also created to manage camera movement on keypress or mouse movement. Keyboard controls are WASD for strafing movement, QE for left/right panning RF for up/down movement and XC for up down panning. Mouse panning is on right mouse click and hold.

The vast majority of code is in the scenebasic_uniform files with some reusable components (camera) in their own files. A scene manages their own keyboard and mouse inputs, changes to scenerunner only catch the inputs and then pass them directly to the scene to manage. This allows the scene to be in control of its inputs, allowing for different keybindings between different scenes (for CW2). This is the same with the camera which alters and returns the view, it can be used in another scene without any changes.

Although the shader supports normal maps not all of the models used have a normal map. A common workaround that I used was to create a tiny 4x4 image that could be used as a placeholder normal map when one was not being used.

A skybox is also created using the same shader as the rest of the models. A float flag is used when the active nexture is the skybox texture and different shading approach is taken. The flag is then set to 0 when the skybox has finished rendering.

### shaders
bliin-phong has been implmented in the fragment shader. Firstly normals, tangents and binomials are calculated in the vertex shader. Light directions for each light source are also calculated using the light position and the current position, then stored in an array. Other variables that are used in the fragment shader such as the vertex position are then initialized. A float called 'isSkybox' is used in the fragment shader to differenciate between skybox textures and other model textures. Skybox textures do not pass through binn-phong wheras all other items do. As mentioned blinn-phong shading is used in this shader, first the colour of the texture at the current position is added to the light ambient to create the ambient light. The diffuse is calculated slightly differently to apply toon shading. The floor of the dot product of the light direction and normal multiplied by the number of levels is then also multiplied by the scale factor of the number of levels to create the toon shading effect. All items that pass through the shader have gamma correction applied before they are returned. Toon shading has also been applied to the shader with three levels.

### scene
The scene is of a dragon attacking a tower at dusk. It contains a tower model, a dragon model, tiled cobblstone ground and tiled rocks bordering the edge of the scene. A soft red glow shines from behind the tower to indicate the burning village that the dragon has just laid waste to. A second light pans across the scene to indicate dusk turning into night. The skybox is of a dark stormy night to set the scene of a dragon attack on a cold night.


