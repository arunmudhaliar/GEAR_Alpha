#version 120
attribute vec4 a_vertex_coord_v4;
attribute vec3 a_normal_coord_v3;

uniform mat4 u_mv_m4x4;
uniform mat4 u_mvp_m4x4;

varying vec3 v_eyespacenormal_v3;

void main()
{
	v_eyespacenormal_v3 = mat3(u_mv_m4x4) * a_normal_coord_v3;
	gl_Position = u_mvp_m4x4 * a_vertex_coord_v4;
}
