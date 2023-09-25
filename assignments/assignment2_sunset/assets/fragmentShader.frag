
	#version 450
	out vec4 FragColor;
	uniform float iTime;
	uniform vec3 sunColor;
	uniform vec3 skyColorTop;
	uniform vec3 skyColorBot;
    uniform vec3 iResolution;
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
    vec2 center = vec2(0, sin(iTime) - 0.6 );
    float radius = 0.3;
    
    float d = distance(center,uv);
    float t = smoothstep(radius,radius + 0.1, d);
  
    vec3 color = mix(skyColorBot,skyColorTop,uv.y);
    
    float setter = smoothstep(-1.7, 1.0, sin(iTime)); // first value lower than possible so doesnt go pitch black
    vec3 skyColor = (color * setter);
    
    vec3 col = mix(sunColor, skyColor, t);

    float aspect = iResolution.x / iResolution.y;
    //Amount of smoothness for anti-aliasing
    float px = 2.0*aspect/iResolution.y;
    
    //rectangle
    float a = 1.564; //rotation
    vec2 p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.7,-1.5); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.3,0.1),0.0)));
    
    //rectangle 2
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.8,0.-1.33); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.2,0.09),0.0)));
    
    //Rounded rectangle 1
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.6,0.-1.33); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.2,0.09), 0.2)));
    
    //rectangle 3
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.9,0.4); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.15,0.05),0.0)));
    
    //rectangle 4
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.65,1.0); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.4,0.2),0.0)));
    
    //Rounded rectangle 2
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.2,1.15); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.4,0.02), 0.24)));
    
    //rectangle 5
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.8,0.0); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.2,0.09),0.0)));
    
    //rectangle 6
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.8,-0.2); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.3,0.09),0.0)));
    
    //rectangle 6
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.6,-0.8); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.5,0.09),0.0)));
    
    //rectangle 7
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.85,-0.95); //position
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.2,0.09),0.0)));
    
    //Rounded rectangle 3
    a = -1.0; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(1.0,0.38); //position (axis)
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.2,0.1), 0.24)));
    
    //rectangle 8
    a = 1.564; //rotation
    p = mat2(cos(a),-sin(a),sin(a),cos(a))*uv; //bounds with resolution
    p-=vec2(-0.85,1.5); //position
    //                  color                                               stretch
    col = mix(col,vec3(0.1,0.1,0.1) * (setter+ 0.4),smoothstep(px,-px,roundedRectSDF(p,vec2(0.2,0.09),0.0)));
    
    // Output to screen
	FragColor = vec4(col,1.0);

	}