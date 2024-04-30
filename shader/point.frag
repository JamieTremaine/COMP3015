#version 460 

in vec2 TexCoord;

layout(binding = 0) uniform sampler2D SpriteTex;

layout(location = 0) out vec4 FragColour;


void main() {
	FragColour = texture(SpriteTex, TexCoord);
}