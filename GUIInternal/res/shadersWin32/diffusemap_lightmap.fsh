uniform sampler2D u_lightmap_texture;
uniform sampler2D u_diffuse_texture;

varying vec2 v_uvcoord0;
varying vec2 v_uvcoord1;

void main()
{
	gl_FragColor = texture2D(u_diffuse_texture, v_uvcoord0) * texture2D(u_lightmap_texture, v_uvcoord1);
	//gl_FragColor = texture2D(u_diffuse_texture, v_uvcoord0);
}
