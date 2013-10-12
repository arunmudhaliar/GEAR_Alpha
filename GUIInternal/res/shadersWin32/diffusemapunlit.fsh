varying vec4 v_destinationcolor_v4;
varying vec2 v_uv_coord0_v2;

uniform sampler2D u_diffuse_texture;

void main()
{
	gl_FragColor = texture2D(u_diffuse_texture, v_uv_coord0_v2);// * v_destinationcolor_v4;
}
