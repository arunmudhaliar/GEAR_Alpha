
attribute vec4 a_vertex_coord_v4;
attribute vec2 a_uv_coord0_v2;

uniform mat4 u_mvp_m4x4;

varying vec2 v_uvcoord0;

void main()
{
    v_uvcoord0=a_uv_coord0_v2;
	gl_Position = u_mvp_m4x4 * a_vertex_coord_v4;
}
