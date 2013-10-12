#version 120

attribute vec4 a_vertex_coord_v4;
attribute vec2 a_uv_coord0_v2;
uniform mat4 u_mvp_m4x4;
varying vec2 v_uv_coord0_v2;

void main()
{
	v_uv_coord0_v2 = a_uv_coord0_v2;
	gl_Position = u_mvp_m4x4 * a_vertex_coord_v4;
}
