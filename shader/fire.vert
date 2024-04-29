#version 460

const float PI = 3.14159265359;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexAge;

out vec3 Position;
out vec3 Velocity;
out float Age;
out float TransP;
out vec2 TexCoord;

uniform mat4 Projection;
uniform mat4 ModelViewMatrix;

uniform int Pass;
uniform float Time;
uniform float DeltaT;
uniform vec3 Accel;

uniform float ParticleLifetime;
uniform float ParticleSize = 1.0;
uniform vec3 Emitter = vec3(0);
uniform mat3 EmitterBasis;

uniform sampler1D RandomTex;

const vec3 offsets[] = vec3[](
	vec3(-0.5, -0.5, 0),
	vec3(0.5, -0.5, 0),
	vec3(0.5, 0.5, 0),
	vec3(-0.5, -0.5, 0),
	vec3(0.5, 0.5, 0),
	vec3(-0.5, 0.5, 0)
);

const vec2 textCoords[] = vec2[](
	vec2(0, 0),
	vec2(1, 0),
	vec2(1, 1),
	vec2(0, 0),
	vec2(1, 1),
	vec2(0, 1)
);


vec3 randomInitalVelocity() {
	float velocity = mix(0.1, 0.5, texelFetch(RandomTex, 2 * gl_VertexID, 0).r);
	return EmitterBasis * vec3(0, velocity, 0);
}

vec3 randomInitalPosition() {
	float offset = mix(-2.3, 11.0, texelFetch(RandomTex, 2 * gl_VertexID + 1, 0).r);
	return Emitter + vec3(offset, 0, 0);
}

void update() {

	Age = VertexAge + DeltaT;

	if(VertexAge < 0 || VertexAge > ParticleLifetime) {
		Position = randomInitalPosition();
		Velocity = randomInitalVelocity();
		if(VertexAge > ParticleLifetime) {
			Age = (VertexAge - ParticleLifetime) + DeltaT;
		}
	} else {
		Position = VertexPosition + VertexVelocity * DeltaT;
		Velocity = VertexVelocity + Accel * DeltaT;
	}

}

void render() {
	TransP = 0.0;
	vec3 posCam = vec3(0.0);
	if(VertexAge >= 0.0) {
		posCam = (ModelViewMatrix * vec4(VertexPosition, 1)).xyz + offsets[gl_VertexID] * ParticleSize;
		TransP = clamp(1.0 - VertexAge / ParticleLifetime, 0, 1);
	}

	TexCoord = textCoords[gl_VertexID];
	gl_Position = Projection * vec4(posCam, 1);
}

void main() {
	if (Pass == 1) { update(); }
	else  { render(); }
}