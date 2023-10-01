#version 460
	in vec3 vertNormal;
	in vec3 vertDir;
	out vec4 fragColor;
	uniform samplerCube skyboxTexture;

	/// Prototypes
	vec4 reflection();
	vec4 refraction();

void main() {
   
	//fragColor = refraction();
	fragColor = reflection();

}


vec4 reflection(){
	 vec3 vReflection = reflect(vertDir, vertNormal);
	 vReflection.x *= -1.0;
	 return texture (skyboxTexture, vReflection);
}   

vec4 refraction(){
	vec3 vRefraction = refract(-vertDir, vertNormal, 1.0/1.52);
	return texture(skyboxTexture, vRefraction);
}
