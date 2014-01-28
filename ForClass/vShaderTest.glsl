#version 150 



in   vec4 vPosition;
in   vec3 vNormal;

// output values that will be interpolated per-fragment
out  vec3 N;
out  vec3 E;
out  vec3 L;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection*ModelView*vPosition;
	N = vNormal;
    E = vPosition.xyz;
    L = LightPosition.xyz - vPosition.xyz;
    
    if( LightPosition.w == 0.0 ) {
		L = vPosition.xyz;
    }

}
