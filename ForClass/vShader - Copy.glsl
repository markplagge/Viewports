



in   vec4 vPosition;
in   vec3 vNormal;

// output values that will be interpolated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
	//mat4 invModelView = transpose(ModelView);
	fN = vNormal;
    fE = vPosition.xyz;
    fL = LightPosition.xyz;
    
    if( LightPosition.w != 0.0 ) {
		//fL = normalize(LightPosition.xyz - (ModelView * vPosition).xyz);
		fL = normalize( LightPosition.xyz - vPosition.xyz);
    }

    gl_Position = Projection*ModelView*vPosition;
}
