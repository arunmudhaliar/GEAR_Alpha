uniform vec3 u_lightposition_v3;

uniform float u_shininess_f1;
uniform vec4 u_ambient_v4;
uniform vec4 u_diffuse_v4;
uniform vec4 u_specular_v4;

uniform vec4 u_light_diffuse_v4;
uniform vec4 u_light_ambient_v4;
uniform vec4 u_light_specular_v4;
uniform vec3 u_eyedirection_v3;

varying vec3 v_eyespacenormal_v3;

void main()
{
	vec3 n = normalize(v_eyespacenormal_v3);
	vec3 l = normalize(u_lightposition_v3);
	vec3 e = u_eyedirection_v3;
	vec3 h = normalize(l+e);

	float df = max(0.0, dot(n, l));
	float sf = max(0.0, dot(n, h));
	sf = pow(sf, u_shininess_f1);

	vec4 color = (u_ambient_v4*u_light_diffuse_v4) + df * (u_diffuse_v4*u_light_ambient_v4) + sf * (u_specular_v4*u_light_specular_v4);
	//vec4 color = df * a_diffuse_v4;
	gl_FragColor = color;
}