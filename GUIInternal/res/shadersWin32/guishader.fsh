uniform sampler2D u_diffuse_texture;
varying vec2 v_uvcoord0;

void main()
{
	gl_FragColor = texture2D(u_diffuse_texture, v_uvcoord0);
}

/*
uniform float time;
void main(void)
{
    mediump vec2 tc = v_uvcoord0;
    mediump vec2 p = -1.0 + 2.0 * tc;
    mediump float len = length(p);
    mediump vec2 uv = tc + (p/len)*cos(len*12.0-time*4.0)*0.03;
    mediump vec3 col = texture2D(u_diffuse_texture, uv).xyz;
    gl_FragColor = vec4(col,1.0);
}
*/