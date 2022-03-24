#version 330 core
in vec2 ftexCoord;
in vec3 fColor;
in vec3 vertexCoord;

out vec4 fragment;


uniform sampler2D texture;
void main(){
	fragment = texture(texture, ftexCoord);
}