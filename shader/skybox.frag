#version 460

in vec3 Vec;

layout (binding = 0) uniform samplerCube SkyboxTex;

layout (location = 0) out vec4 FragColour;

void main() {
    vec3 texColour = texture(SkyboxTex, normalize(-Vec)).rgb;
  
    texColour = pow(texColour, vec3(1.0/2.2));
    FragColour = vec4(texColour, 1.0);
}