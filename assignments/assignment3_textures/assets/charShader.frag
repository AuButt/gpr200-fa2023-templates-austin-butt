#version 450
out vec4 FragColor;
in vec2 UV;
uniform sampler2D _CharTexture;

void main(){
	FragColor = texture(_CharTexture, UV);
}