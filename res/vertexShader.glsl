#version 330 core
layout (location = 0) in vec3 aposition;
layout (location = 1) in vec2 atexCoord;
out vec2 ftexCoord;
out vec3 vertexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 chunk;

void main(){
	gl_Position = projection * view * model * vec4(aposition+chunk, 1.0);
	vertexCoord = aposition;
	ftexCoord = atexCoord;
}