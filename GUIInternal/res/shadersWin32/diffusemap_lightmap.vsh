
attribute vec4 a_vertex_coord_v4;
uniform mat4 u_mvp_m4x4;
attribute vec2 a_uv_coord0_v2;
attribute vec2 a_uv_coord1_v2;

varying vec2 v_uvcoord0;
varying vec2 v_uvcoord1;

void main()
{
	gl_Position = u_mvp_m4x4 * a_vertex_coord_v4;
    
    v_uvcoord0=a_uv_coord0_v2;
    v_uvcoord1=a_uv_coord1_v2;
}
