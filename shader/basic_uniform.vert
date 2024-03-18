#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec3 Vec;
out vec3 Position;
out vec3 LightDir[2];
out vec3 ViewDir;
out vec3 Normal;
out vec2 TexCoord;
out float isSkybox;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat3 ProjectionMatrix;
uniform mat4 MVP;
uniform float renderSkyBox;

uniform struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 L;
} Light[2];

uniform struct MaterialInfo {
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
}Material;

void main()
{
     isSkybox = renderSkyBox;
     Vec = VertexPosition;
     TexCoord = VertexTexCoord;
     Position = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

     vec3 normal = normalize(NormalMatrix * VertexNormal);
     vec3 tangent = normalize(NormalMatrix * vec3(VertexTangent));
     vec3 binormal = normalize(cross(normal, tangent)) * VertexTangent.w;

     mat3 toObjectLocal = mat3(
        tangent.x, binormal.x, normal.x,
        tangent.y, binormal.y, normal.y,
        tangent.z, binormal.z, normal.z
     );

      for(int i=0; i<2; i++) {
         LightDir[i] = toObjectLocal * (Light[i].Position.xyz - Position);
      }

     ViewDir = toObjectLocal * normalize(-Position);
     Normal = normal;

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
