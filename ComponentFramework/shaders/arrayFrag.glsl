#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;


const int maxLights = 2;
uniform int numLights = maxLights;

layout(location = 0) in vec3 vertNormal;
//layout(location = 2) in vec3 lightDir2;
layout(location = 1) in vec3 eyeDir; 
layout(location = 2) in vec2 textureCoords;
layout(location = 3) in vec3 lightDir[maxLights];

uniform sampler2D myTexture;

void main() {

    vec4 ks[maxLights];
    vec4 kd[maxLights];
     ks[0] = vec4(0.1, 0.6, 0.8, 0.2);
     kd[0] = vec4(0.1, 0.1, 0.7, 0.1);
     ks[1] = vec4(0.8, 0.3, 0.3, 0.4);
     kd[1] = vec4(0.3, 0.0, 0.5, 0.3);
 
 vec4 kt = texture(myTexture, textureCoords);
 vec4 ka = 0.1 * kt;
 for (int i = 0; i < numLights; i++){

   float  diff = max(dot(vertNormal, lightDir[i]), 0.0);
    vec3 reflection = normalize(reflect(-lightDir[i], vertNormal));
    float spec = max(dot(eyeDir, reflection), 0.0);
    spec = pow(spec,14.0);
    fragColor += (diff * kt * (kd[i])) + (spec * ks[i]); 
    }
 fragColor += ka;
}