#version 150
 
uniform vec4 LightAmbient, LightDiffuse, LightSpecular;
uniform vec4 MaterialAmbient, MaterialDiffuse, MaterialSpecular;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;

in  vec3 fN;
in  vec3 fE;
in  vec3 fL;
in  vec2 fTexCoord;

out vec4 fColor;

uniform sampler2D texture;

void main()
{
	vec3 NN = normalize(fN);
	vec3 EE = normalize(fE);
	vec3 LL = normalize(fL);
	vec4 ambient, diffuse, specular;
	vec3 H = normalize(LL + EE);
	float Kd = max(dot(LL, NN), 0.0);
	//Kd = dot(LL, NN);
	float Ks = pow(max(dot(NN, H), 0.0), Shininess);
	ambient = 0.1 * texture2D(texture, fTexCoord);
	diffuse = Kd * texture2D(texture, fTexCoord);

	if (dot(LL, NN) < 0.0)
	{
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		specular = Ks * texture2D(texture, fTexCoord);
	}
	fColor = ambient + diffuse + specular;
	fColor.a = 1.0;
}
