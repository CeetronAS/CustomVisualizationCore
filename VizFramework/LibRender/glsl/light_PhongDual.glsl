
uniform vec3 u_ecLightPosition;
uniform vec3 u_ecLightPosition2;
uniform float u_ambientIntensity;

varying vec3 v_ecPosition;
varying vec3 v_ecNormal; 

//--------------------------------------------------------------------------------------------------
/// lightFragment() - inShadowFactor: 0..1 (1 not in shadow, 0 totally in shadow)
//--------------------------------------------------------------------------------------------------
vec4 lightFragment(vec4 srcFragColor, float shadowFactor)
{
	const float specularIntensity = 0.5;

	// Viewing vector (from point to eye)
	// Since we are in eye space, the eye pos is at (0, 0, 0)
	vec3 V = normalize(-v_ecPosition);
	vec3 N = normalize(v_ecNormal);

	// Light vector (from point to light source)
	vec3 L1 = normalize(u_ecLightPosition - v_ecPosition);
	vec3 L2 = normalize(u_ecLightPosition2 - v_ecPosition);

	vec3 R1 = normalize(reflect(-L1, N));
	vec3 R2 = normalize(reflect(-L2, N));

	vec3 ambient = srcFragColor.rgb*u_ambientIntensity;

	vec3 diffuse = (0.25 + shadowFactor*0.75)*0.8*srcFragColor.rgb*(1.0 - u_ambientIntensity)*max(dot(N, L1), 0.0);
	diffuse += (0.25 + shadowFactor*0.75)*0.2*srcFragColor.rgb*(1.0 - u_ambientIntensity)*max(dot(N, L2), 0.0);

	//vec3 specular = vec3(0,0,0);
	vec3 specular = shadowFactor*0.8*vec3(specularIntensity*pow(max(dot(R1, V), 0.0), 20.0));
	specular += shadowFactor*0.2*vec3(specularIntensity*pow(max(dot(R2, V), 0.0), 20.0));

	return vec4(ambient + diffuse + specular, srcFragColor.a);
}
