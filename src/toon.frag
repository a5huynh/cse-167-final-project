varying vec3 normal, lightDir, eyeVector;
uniform sampler1D edgeramp;
uniform sampler2D hatch1, hatch2, hatch3, hatch4;
uniform int windowWidth, windowHeight;
uniform bool outlinesOn, texturesOn;

void main()
{
	float edge, diffuse;
	vec4 color;
	vec3 n = normalize(normal);
	vec3 l = normalize(lightDir);
	vec3 e = normalize(eyeVector);
	
	vec2 coord = vec2(gl_FragCoord.x / float(windowWidth), gl_FragCoord.y / float(windowHeight));
	
	edge    = max(dot(n, e), 0.0);
	diffuse = max(dot(n, l), 0.0);
	
	if (diffuse > 0.75) {
		color = gl_Color * 1.0;
		if (texturesOn)
			color = texture2D(hatch4, coord) * gl_Color;
	}
	else if (diffuse > 0.5) {
		color = gl_Color * 0.6;
		if (texturesOn)
			color = texture2D(hatch3, coord) * gl_Color;
	}
	else if (diffuse > 0.25) {
		color = gl_Color * 0.4;
		if (texturesOn)
			color = texture2D(hatch2, coord) * gl_Color;
	}
	else {
		color = gl_Color * 0.2;
		if (texturesOn)
			color = texture2D(hatch1, coord) * gl_Color;
	}
	
	
	if (outlinesOn) {
		color = texture1D(edgeramp, edge) * color;
	}
	
	gl_FragColor = color;
 }
