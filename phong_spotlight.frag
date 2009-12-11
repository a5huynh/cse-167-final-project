varying vec3 normal, lightDir, spotLightDir, spotReflection, reflection, eye;

uniform float wWidth, wHeight;

uniform sampler2D hatching1;
uniform sampler2D hatching2;

#define PHONG_EXP 128.0

float doHatching( inout vec4 pColor, in sampler2D texture, vec4 color, in float cutoff, in float multiplier ) {
	
	float tIntensity = length( vec3( texture2D( texture, vec2( gl_FragCoord.x / wWidth, gl_FragCoord.y / wHeight ) ) ) );
	
	if( tIntensity < cutoff ) {	
		pColor = color * multiplier;
	}
	
	return (tIntensity);
}

vec2 fragVec() {
	return(vec2( gl_FragCoord.x / wWidth, gl_FragCoord.y / wHeight ));
}

void main() {
	
	float intensity;
	vec4 diffuseColor;
	
	vec4 dColor = gl_Color;// * gl_LightSource[0].diffuse; //gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	
	vec3 n = normalize( normal );
	vec3 e = normalize( eye );

	intensity = dot( lightDir, n );
				
	//if( dColor != vec4( 0, 0, 0, 1 ) ) {
	//	dColor = vec4( 1, 1, 1, 1 );
	//}
	
	if (intensity > 0.95) {
	
		diffuseColor = dColor * texture2D( hatching1, fragVec() );

		doHatching( diffuseColor, hatching1, dColor, 1.5, 1.0 );
		
	} else if (intensity > 0.5) {
	
		diffuseColor = dColor * texture2D( hatching1, fragVec() );

		doHatching( diffuseColor, hatching1, dColor, 1.5, .75 );		
		
	} else if (intensity > 0.25) {
		
		diffuseColor = dColor * texture2D( hatching1, fragVec() );
		
		doHatching( diffuseColor, hatching1, dColor, 1.5, 0.5 );
				
	} else {

		diffuseColor = dColor * texture2D( hatching2, fragVec() );		
		
		doHatching( diffuseColor, hatching2, dColor, 1.5, 0.25 );
				
	}

	gl_FragColor = diffuseColor;
}
