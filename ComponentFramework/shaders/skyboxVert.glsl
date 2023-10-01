#version 450
layout (location = 0) in vec4 vVertex;


out vec3 uvwCoords;
layout(location = 0) uniform mat4 projectionMatrix;
layout(location = 1) uniform mat4 cameraRotationMatrix;

void main(){
	uvwCoords = vec3(vVertex); // vVertex.xyz
	gl_Position = projectionMatrix * cameraRotationMatrix * vVertex;
}