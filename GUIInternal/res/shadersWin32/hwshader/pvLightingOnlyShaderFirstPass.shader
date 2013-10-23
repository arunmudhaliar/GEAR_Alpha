#ifdef GEAR_VERTEX_SHADER

#version 120

attribute vec4 a_vertex_coord_v4;
attribute vec3 a_normal_coord_v3;

//light
uniform vec4 u_light_diffuse_v4;
uniform vec4 u_light_ambient_v4;
uniform vec4 u_light_specular_v4;
uniform vec3 u_lightposition_v3;

//model
uniform vec4 u_ambient_v4;
uniform vec4 u_diffuse_v4;
uniform vec4 u_specular_v4;
uniform float u_shininess_f1;

uniform mat4 u_modelview_m4x4;
uniform mat4 u_mvp_m4x4;
uniform vec3 u_eyedirection_v3;

varying vec4 v_destinationcolor_v4;

void main()
{
	vec3 n = normalize(mat3(u_modelview_m4x4) * a_normal_coord_v3);
	vec3 l = normalize(mat3(u_modelview_m4x4)*u_lightposition_v3);
	vec3 e = u_eyedirection_v3;//vec3(0, 0, 1);
	vec3 h = normalize(l+e);

	float df = max(0.0, dot(n, l));
	float sf = max(0.0, dot(n, h));
	sf = pow(sf, u_shininess_f1);

	vec4 color = (u_ambient_v4*u_light_ambient_v4) + df * (u_diffuse_v4*u_light_diffuse_v4) + sf * (u_specular_v4*u_light_specular_v4);
	v_destinationcolor_v4 = color;
	
	gl_Position = u_mvp_m4x4 * a_vertex_coord_v4;
}
#elif defined (GEAR_FRAGMENT_SHADER)
varying vec4 v_destinationcolor_v4;
void main()
{
	gl_FragColor = v_destinationcolor_v4;
}
#endif
