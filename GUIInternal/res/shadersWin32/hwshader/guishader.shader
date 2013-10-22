#ifdef GEAR_VERTEX_SHADER

attribute vec2 a_vertex_coord_v2;
uniform mat4 u_mvp_m4x4;
attribute vec2 a_uv_coord0_v2;

varying vec2 v_uvcoord0;

void main()
{
	gl_Position = u_mvp_m4x4 * vec4(a_vertex_coord_v2, 0.0, 1.0);
    
    v_uvcoord0=a_uv_coord0_v2;
}

#elif defined (GEAR_FRAGMENT_SHADER)

uniform sampler2D u_diffuse_texture;
varying vec2 v_uvcoord0;

void main()
{
	gl_FragColor = texture2D(u_diffuse_texture, v_uvcoord0);
}
#endif
