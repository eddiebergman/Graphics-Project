#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vTexCoord;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec4 vColor;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

out vec3 modelVertexPosition;
out vec3 modelVertexNormal;

void main(){
	
	modelVertexPosition = vec3(model * vec4(vPosition, 1.0));
	modelVertexNormal = mat3(transpose(inverse(model))) * vNormal;
		
	gl_Position = proj * view * model * vec4(vPosition, 1.0);
}