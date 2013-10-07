
attribute vec4 a_vertex_coord_v4;
uniform mat4 u_mvp_m4x4;
uniform mat3 u_flags_m3x3;

attribute vec3 a_diffuse_v3;
attribute vec3 a_ambient_v3;
attribute vec3 a_specular_v3;

varying vec4 v_destinationcolor_v4;

void main()
{
	vec3 color = a_ambient_v3 + a_diffuse_v3 + a_specular_v3;
	v_destinationcolor_v4 = vec4(color, 1);

	gl_Position = u_mvp_m4x4 * a_vertex_coord_v4;
}
