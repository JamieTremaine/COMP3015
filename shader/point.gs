#version 460

layout(points) in;
layout(triangle_Strip, max_vertices = 4) out;

uniform float Size2;

uniform mat4 Projection;
out vec2 TexCoord;

void main() {
	mat4 m = Projection;

	gl_Position = m * (vec4(-Size2, -Size2, 0.0, 0.0) + gl_in[0].gl_Position);
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();

	gl_Position = m * (vec4(Size2, -Size2, 0.0, 0.0) + gl_in[0].gl_Position);
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();
	
	gl_Position = m * (vec4(-Size2, Size2, 0.0, 0.0) + gl_in[0].gl_Position);
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();
	
	gl_Position = m * (vec4(Size2, Size2, 0.0, 0.0) + gl_in[0].gl_Position);
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();

}