#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;
uniform float iTime;
void main(){
	UV = vUV;
	gl_Position = vec4(vPos + sin(iTime*2) ,5);
}