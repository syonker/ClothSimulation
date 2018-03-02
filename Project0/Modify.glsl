////////////////////////////////////////
// Modify.glsl
////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

#ifdef VERTEX_SHADER

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 ModelMtx=mat4(1);
uniform mat4 ModelViewProjMtx=mat4(1);

////////////////////////////////////////
// Vertex shader
////////////////////////////////////////

void main() {
	gl_Position=ModelViewProjMtx * vec4(Position,1);

	fragPosition=vec3(ModelMtx * vec4(Position,1));
	fragNormal=vec3(ModelMtx * vec4(Normal,0));
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef FRAGMENT_SHADER

in vec3 fragPosition;
in vec3 fragNormal;

//uniform vec3 AmbientColor=vec3(0.2);
uniform vec3 AmbientColor;
uniform vec3 LightDirection=normalize(vec3(0,0,1));
uniform vec3 LightColor=vec3(1,1,0);
uniform vec3 DiffuseColor=vec3(0.5);
uniform vec3 LightDirection2=normalize(vec3(1,0,0));
uniform vec3 LightColor2=vec3(0,1,1);

out vec3 finalColor;

////////////////////////////////////////
// Fragment shader
////////////////////////////////////////

void main() {
	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance=AmbientColor + LightColor * max(0,dot(LightDirection,fragNormal));

	vec3 irradiance2=AmbientColor + LightColor2 * max(0,dot(LightDirection2,fragNormal));

	irradiance = (irradiance+irradiance2)*0.5f;

	// Diffuse reflectance
	vec3 reflectance=irradiance * DiffuseColor;

	// Gamma correction
	gl_FragColor=vec4(sqrt(reflectance),1);
}

#endif

////////////////////////////////////////////////////////////////////////////////
