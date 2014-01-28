


// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;

out vec4 fColor;

uniform vec4 LightAmbient, LightDiffuse, LightSpecular;
uniform vec4 MaterialAmbient, MaterialDiffuse, MaterialSpecular;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;

void main() 
{ 
    // Normalize the input lighting vectors
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    //vec3 L = normalize(fL);
	mat4 invModelView = transpose(ModelView);
	vec3 L;
	if(LightPosition.w != 0.0)
	{
		L = normalize((invModelView * LightPosition).xyz - E);
	}
	else
	{
		L = normalize((invModelView * LightPosition).xyz);
	}

    vec3 H = normalize( L + E );
    
    vec4 ambient = LightAmbient * MaterialAmbient;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd * LightDiffuse * MaterialDiffuse;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks * LightSpecular * MaterialSpecular;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) {
		specular = vec4(0.0, 0.0, 0.0, 1.0);
    }

    fColor = ambient + diffuse + specular;
    fColor.a = 1.0;
} 

