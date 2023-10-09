#version 450
out vec4 FragColor;
in vec2 UV;
uniform sampler2D _Texture;
uniform sampler2D _NoiseTexture;
uniform float iTime;

float noise = texture(_NoiseTexture, UV).r;
vec2 uv = UV + noise * 0.1f * sin(iTime * 0.4);

void main(){
	FragColor = texture(_Texture, uv);
}