varying vec3 normal, lightDir, eyeVector;
varying float dist;

void main()
{	
	vec4 ecPos;
	vec3 aux;

	normal = gl_NormalMatrix * gl_Normal;

	// compute light's direction for point light
	ecPos = gl_ModelViewMatrix * gl_Vertex;
	aux = vec3(gl_LightSource[0].position - ecPos);
	lightDir = normalize(aux);
	dist = length(aux);

	eyeVector = -vec3(gl_ModelViewMatrix * gl_Vertex);

	gl_Position = ftransform();
	
	gl_FrontColor = gl_Color;
}
