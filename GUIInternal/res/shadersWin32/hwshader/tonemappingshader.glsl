#ifdef GEAR_VERTEX_SHADER
attribute vec4 a_vertex_coord_v4;
uniform mat4 u_mvp_m4x4;
attribute vec2 a_uv_coord0_v2;
varying vec2 v_uvcoord0;

void main()
{
	gl_Position = u_mvp_m4x4 * a_vertex_coord_v4;
    
    v_uvcoord0=a_uv_coord0_v2;
}

#elif defined (GEAR_FRAGMENT_SHADER)
uniform sampler2D u_diffuse_texture;
varying vec2 v_uvcoord0;
uniform sampler2D u_bloomblur_texture;
uniform float u_exposure;

void main()
{
	const float gamma = 2.2;
    vec3 hdrColor = texture2D(u_diffuse_texture, v_uvcoord0).rgb;      
    vec3 bloomColor = texture2D(u_bloomblur_texture, v_uvcoord0).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * u_exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    vec4 color = vec4(result, 1.0);
	
	gl_FragColor = color;
}
#endif
