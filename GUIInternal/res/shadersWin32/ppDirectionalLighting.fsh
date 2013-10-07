/*
 #define OBJBASE_FLAG_USE_TM                     0   (0,0)
 //material
 #define MATERIAL_FLAG_USE_AMBIENT_MAP			2   (0,2)
 #define MATERIAL_FLAG_USE_DIFFUSE_MAP			3   (0,3)
 #define MATERIAL_FLAG_USE_SPECULAR_CLR_MAP      4   (1,0)
 #define MATERIAL_FLAG_USE_SPECULAR_MAP          5   (1,1)
 #define MATERIAL_FLAG_USE_GLOSSINESS_MAP		6   (1,2)
 #define MATERIAL_FLAG_USE_SELF_ILLUMINATION_MAP	7   (1,3)
 #define MATERIAL_FLAG_USE_OPACITY_MAP			8   (2,0)
 #define MATERIAL_FLAG_USE_FILTER_CLR_MAP		9   (2,1)
 #define MATERIAL_FLAG_USE_NORMAL_MAP			10  (2,2)
 #define MATERIAL_FLAG_USE_REFLECTION_MAP		11  (2,3)
 #define MATERIAL_FLAG_USE_REFRACTION_MAP		12  (3,0)
 //mesh
 //lighting
 #define OTHER_FLAG_USE_LIGHTING                 15  (3,3) //last flag
 */

uniform sampler2D u_diffuse_texture;

uniform vec3 u_light_specular_v3;
uniform float u_shininess;
uniform mat4 u_flags_m4x4;

varying vec3 v_diffuse, v_ambient, v_specular;
varying vec3 v_normal, v_lightDir, v_halfVector;
//varying float shininess;
varying vec2 v_uvcoord0;

void main()
{
	vec3 n, halfV;
	float NdotL, NdotHV;
	
	/* The ambient term will always be present */
	vec3 color = v_ambient;
	
	/* a fragment shader can't write a varying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(v_normal);
	
	/* compute the dot product between normal and ldir */
	NdotL = max(dot(n, v_lightDir),0.0);

	if (NdotL > 0.0) {
			color += v_diffuse * NdotL;
			halfV = normalize(v_halfVector);
			NdotHV = max(dot(n,halfV),0.0);
			color += v_specular * 
					u_light_specular_v3 * 
					pow(NdotHV, u_shininess);
		}

	vec4 color_for_gpu;
	color_for_gpu.rgb=color;
	color_for_gpu.a=1.0;
	if(u_flags_m4x4[0][3] >= 0.5)
	{
		color_for_gpu = texture2D(u_diffuse_texture, v_uvcoord0) * color_for_gpu;
	}

    gl_FragColor =  color_for_gpu;
}