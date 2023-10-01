#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 lightDir;
layout(location = 2) in vec3 lightDir2;
layout(location = 3) in vec3 eyeDir; 
layout(location = 4) in vec2 textureCoords;


uniform sampler2D myTexture;
void main() {

    vec4 ks = vec4(0.678, 0.039, 0.941, 0.0);
    vec4 ks2 = vec4(0.113, 0.176, 0.803, 0.0);
    vec4 kd = vec4(0.678, 0.039, 0.941,0.0);
    vec4 kd2 = vec4(0.011, 0.176, 0.407, 0.0);
     vec4 kt = texture(myTexture, textureCoords);
    vec4 ka = 0.1 * kt;
    float diff = max(dot(vertNormal, lightDir), 0.0);
    float diff2 = max(dot(vertNormal, lightDir2), 0.0);

    /// Reflection is based incedent which means a vector from the light source
    /// not the direction to the light source
    vec3 reflection = normalize(reflect(-lightDir, vertNormal));
    vec3 reflection2 = normalize(reflect(-lightDir2, vertNormal));
    float spec = max(dot(eyeDir, reflection), 0.0);
    float spec2 = max(dot(eyeDir, reflection2), 0.0);
    spec = pow(spec,14.0);
    spec2 = pow(spec2, 14.0);
    fragColor +=  (diff * kd) + (spec * ks);
    fragColor +=  (diff2 * kd2) + (spec2 * ks2);
    fragColor += ka;
}