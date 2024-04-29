#version 460

layout (location = 0) out vec4 FragColour;
in float TransP;
in vec2 TexCoord;

uniform sampler2D ParticleTex;

void main() {
    FragColour = texture(ParticleTex, TexCoord);
    FragColour.a *= TransP;
}