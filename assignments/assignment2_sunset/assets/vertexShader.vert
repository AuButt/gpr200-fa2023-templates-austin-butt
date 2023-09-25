	#version 450
	layout(location = 0) in vec3 vPos;
	layout(location = 1) in vec2 uvIN;
	out vec2 myuv;
	out vec3 coords;
	void main(){
		myuv = uvIN;
		gl_Position = vec4(vPos,1.0);
	}