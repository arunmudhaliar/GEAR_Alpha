uniform sampler2D u_diffuse_texture;
varying vec2 v_uvcoord0;
uniform vec4 u_color_v4;

void main()
{
	gl_FragColor = texture2D(u_diffuse_texture, v_uvcoord0)*u_color_v4;
}
