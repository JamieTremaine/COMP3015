# COMP3015


### Versions
VIsual Studio 2022
Windows 10

### How does it work?
The inital template containing the scene, screnerunner and other helper classes was used. Initally the project follows a similar structure to those of the labs with a Light and Material struct. blinn-phong was implimented in the fragment shader with the ability to take in a diffuse texture and a normal texture. It is also able to impliment multiple lights, in this project two light sources have been used.

The scene is made up entierly of custom meshes, some like the cobblestone floor have been tiled multiple times. The rock meshes surrounding the scene are placed with a random offset and rotation, each time the scene is played a slightly different arrangment will be created.

Both keyboard and mouse controlshave been implemented using OpenGL callbacks. The scene and scenerunner files have been customized to create a callback on key presses, mouse button or mouse cursor changes which is then passed to the scene. A custom camera was also created to manage camera movement on keypress or mouse movement. Keyboard controls are WASD for strafing movement, QE for left/right panning RF for up/down movement and XC for up down panning. Mouse panning is on right mouse click and hold.

The vast majority of code is in the scenebasic_uniform files with some reusable components (camera) in their own files. A scene manages their own keyboard and mouse inputs, changes to scenerunner only catch the inputs and then pass them directly to the scene to manage. This allows the scene to be in control of its inputs, allowing for different keybindings between different scenes (for CW2). This is the same with the camera which alters and returns the view, it can be used in another scene without any changes.

Although the shader supports normal maps n ot all of the models used have a normal map. A common workaround that I used was to create a tiny 4x4 image that could be used as a placeholder normal map when one was not being used.
