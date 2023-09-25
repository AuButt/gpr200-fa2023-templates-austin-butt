
	#version 450
	out vec4 FragColor;
	uniform float iTime;
	uniform vec3 sunColor;
	uniform vec3 skyColorTop;
	uniform vec3 skyColorBot;
    uniform vec3 iResolution;
    uniform float sunRadius;
    uniform float sunSpeed;
    uniform vec3 bgColor;
    uniform float waveSpeed;
	in vec2 myuv;

    float roundedRectSDF( in vec2 p, in vec2 b, in float r )
    {
    vec2 q = abs(p)-b+r;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r;
    }

	void main(){

	vec2 uv = myuv;

	//remaps space to -1,-1 -> 1,1
    uv = uv * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;  //resolution
    //circle
    vec2 center = vec2(0, sin(iTime * sunSpeed) - 0.6 );
    float radius = sunRadius;
    
    float d = distance(center,uv);
    float t = smoothstep(radius,radius + 0.1, d);
  
    vec3 color = mix(skyColorBot,skyColorTop,uv.y);
    
    float setter = smoothstep(-1.7, 1.0, sin(iTime* sunSpeed)); // first value lower than possible so doesnt go pitch black
    vec3 skyColor = (color * setter);
    
    vec3 col = mix(sunColor, skyColor, t);

    float aspect = iResolution.x / iResolution.y;
    //Amount of smoothness for anti-aliasing
    float px = 2.0*aspect/iResolution.y;
    
    float wave = -0.7 + sin(uv.x + iTime*waveSpeed)*0.05;
    wave = step(wave,uv.y);
    col = mix(bgColor, col, wave);
    
    // Output to screen
	FragColor = vec4(col,1.0);

	}