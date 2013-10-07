uniform sampler2D u_diffuse_texture;
varying vec2 v_uvcoord0;

void main()
{
	gl_FragColor = texture2D(u_diffuse_texture, v_uvcoord0);
}
