#version 460



    layout(location = 0) in vec4 vVertex;
    layout(location = 1) in vec4 vNormal;

    out vec3 vertNormal;
    out vec3 vertDir;

    uniform mat4 projectionMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 modelMatrix;
    uniform mat3 normalMatrix;


void main() {
        vertNormal =  /*normalMatrix **/ vNormal.xyz ;
        vec3 vertPos = vec3(modelMatrix * vVertex);
        vertDir = normalize(vertPos);
        gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}