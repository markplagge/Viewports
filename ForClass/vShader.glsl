#version 150 



in   vec4 vPosition;
in   vec3 vNormal;
in   vec3 vTangent;
in   vec2 vTexCoord;

// output values that will be interpolated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;
out  vec2 fTexCoord;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
	//If you are using a Mac that does not support inverse and transpose use the following line instead
	//mat3 normalMatrix = transpose(gl_NormalMatrix);
	//you may have to rewrite the transpose from what I gave in class from mat4 to mat3

	mat3 normalMatrix = inverse(transpose(mat3(ModelView)));
    gl_Position = Projection*ModelView*vPosition;
	fN = normalMatrix * vNormal;
    fE = normalMatrix * vPosition.xyz;
    fL = LightPosition.xyz - (ModelView * vPosition).xyz;
    
    if( LightPosition.w == 0.0 ) {
		fL = (LightPosition).xyz;
    }
	fTexCoord = vTexCoord;

}
