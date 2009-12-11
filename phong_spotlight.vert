// Vertex shader for per-pixel diffuse shading.The
// teapot is a good object to test this shader, since
// the .obj file includes surface normals.

// The shader computes the unit light direction and unit
// surface normal, which are passed to the fragment
// shader as varying variables.

varying vec3 normal, lightDir, spotLightDir, reflection, spotReflection, eye;
uniform sampler2D tex;

void main() {	
	// Note that gl_LightSource, gl_NormalMatrix, and gl_Normal
	// are pre-defined variables that access the current OpenGL
	// state.
	
	vec4 ecPos  = gl_ModelViewMatrix * gl_Vertex;
	
	normal			= normalize( gl_NormalMatrix * gl_Normal );	
	
	lightDir		= normalize( vec3( gl_LightSource[0].position - ecPos ) );
	spotLightDir	= normalize( vec3( gl_LightSource[1].position - ecPos ) );
	
	reflection		= normalize( reflect( lightDir, normal ) );
	spotReflection  = normalize( reflect( spotLightDir, normal ) );
	
	eye		    = -vec3( gl_ModelViewMatrix * gl_Vertex );
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	
	gl_FrontColor = gl_Color;
	
	// ftransform() is a built-in function that applies all
	// transformations (i.e., modelview and 
	// projection) to a vertex.
	gl_Position = ftransform();
}
