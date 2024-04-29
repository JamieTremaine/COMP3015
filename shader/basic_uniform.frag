#version 460

in vec3 Vec;
in vec3 Position;
in vec2 TexCoord;
in float isSkybox;

in vec3 Normal;
in vec3 LightDir[2];
in vec3 ViewDir;

layout (binding = 0) uniform sampler2D Tex1;
layout (binding = 2) uniform sampler2D NormalTex;
layout (location = 0) out vec4 FragColor;

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

vec3 blinnPhong(int lightIndex, vec3 n) {
     vec3 texColour = texture(Tex1, TexCoord).rgb;
     vec3 ambient = Light[lightIndex].La * texColour;

     vec3 s = normalize(LightDir[lightIndex]);
     float sDotN = max(dot(s,n), 0.0);
     vec3 diffuse = texColour * sDotN;
     vec3 spec = vec3(0.0);

     if(sDotN > 0.0) {
            vec3 v = normalize(ViewDir);
            vec3 h = normalize(v + s);
            spec = Material.Ks * pow(max(dot(h,n), 0.0), Material.Shininess);    
        }

     return ambient + (diffuse + spec) * Light[lightIndex].L;
}

void main() {
     vec3 texColour;
        for(int i=0; i<2; i++) {  
            vec3 norm = texture(NormalTex, TexCoord).xyz;
            norm.xy = 2.0 * norm.xy - 1.0;
            texColour += blinnPhong(i, normalize(norm));
        }
        
     texColour = pow(texColour, vec3(1.0/2.2));
     FragColor = vec4(texColour, 1.0);
}
