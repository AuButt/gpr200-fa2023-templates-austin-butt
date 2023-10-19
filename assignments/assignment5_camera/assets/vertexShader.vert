#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec3 Normal;
uniform mat4 _Model;
uniform mat4 _CamView;
uniform mat4 _CamProj;

void main(){
	Normal = vNormal;
	gl_Position =  _CamProj * _CamView * _Model * vec4(vPos,1.0);
}