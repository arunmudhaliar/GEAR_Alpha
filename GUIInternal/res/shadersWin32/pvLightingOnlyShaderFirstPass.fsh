#define BASIC_SHADER 0
#define PERVERTEX_LIGHTING_SHADER 1
#define PERPIXEL_LIGHTING_SHADER 0

#define WIN32

#if BASIC_SHADER

//varying mediump vec2 v_tex_uv;

//uniform sampler2D u_diffuse_texture;

void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);	//texture2D(u_diffuse_texture, v_tex_uv);
}

#elif PERVERTEX_LIGHTING_SHADER

#ifdef WIN32
varying vec4 v_destinationcolor_v4;
#else
varying vec4 v_destinationcolor_v4;
#endif

void main()
{
	gl_FragColor = v_destinationcolor_v4;
}

#elif PERPIXEL_LIGHTING_SHADER

uniform vec3 u_lightposition_v3;

varying vec3 v_eyespacenormal_v3;

void main()
{
	highp vec3 n = normalize(v_eyespacenormal_v3);
	highp vec3 l = normalize(u_lightposition_v3);
	highp vec3 e = vec3(0, 0, 1);
	highp vec3 h = normalize(l+e);

	float df = max(0.0, dot(n, l));
	float sf = max(0.0, dot(n, h));
	sf = pow(sf, a_shininess_f1);

	vec3 color = a_ambient_v3 + df * a_diffuse_v3 + sf * a_specular_v3;
	//vec3 color = df * a_diffuse_v3;
	v_destinationcolor_v4 = vec4(color, 1);
}

#endif