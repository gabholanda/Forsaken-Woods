#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 lightDir;
layout(location = 2) in vec3 eyeDir; 
layout(location = 3) in vec2 textureCoords;


uniform sampler2D myTexture;
void main() {

    vec4 ks = vec4(0.4, 0.4, 0.6, 0.0);
    vec4 kd = vec4(0.1, 0.1, 0.7, 0.0);
    vec4 kt = texture(myTexture, textureCoords);
    vec4 ka = 0.1 * kt;
    float diff = max(dot(vertNormal, lightDir), 0.0);

    /// Reflection is based incedent which means a vector from the light source
    /// not the direction to the light source
    vec3 reflection = normalize(reflect(-lightDir, vertNormal));
    float spec = max(dot(eyeDir, reflection), 0.0);
    spec = pow(spec,14.0);
   fragColor = ka + (diff * kt) + (spec * ks);
}