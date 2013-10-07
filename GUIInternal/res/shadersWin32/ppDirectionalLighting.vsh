//#version 150
//#pragma debug(on)

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

attribute vec4 a_vertex_coord_v4;
attribute vec3 a_normal_coord_v3;
attribute vec2 a_uv_coord0_v2;

attribute vec3 a_eyePos_v3;
attribute vec4 a_diffuse_v4;
attribute vec4 a_ambient_v4;
attribute vec4 a_specular_v4;

attribute vec3 a_light_diffuse_v3;
attribute vec3 a_light_ambient_v3;

uniform mat4 u_mvp_m4x4;
uniform vec3 u_lightPos_v3;
uniform mat4 u_flags_m4x4;

varying vec3 v_diffuse, v_ambient, v_specular;
varying vec3 v_normal, v_lightDir, v_halfVector;
varying vec2 v_uvcoord0;

void main()
{
    //v_flags_m4x4=u_flags_m4x4;
	/* first transform the normal into eye space and 
	normalize the result */
	v_normal = normalize(a_normal_coord_v3);
	
	/* now normalize the light's direction. Note that 
	according to the OpenGL specification, the light 
	is stored in eye space. Also since we're talking about 
	a directional light, the position field is actually direction */
    v_lightDir = normalize(u_lightPos_v3);

	/* Normalize the halfVector to pass it to the fragment shader */
	vec3 hv=a_eyePos_v3+u_lightPos_v3;
	v_halfVector = normalize(hv);

	/* Compute the diffuse, ambient and globalAmbient terms */
	v_diffuse = a_diffuse_v4.rgb * a_light_diffuse_v3;
	v_specular=a_specular_v4.rgb;
	v_ambient = a_ambient_v4.rgb * a_light_ambient_v3;
	//ambient += gl_LightModel.ambient * ambient;

	//shininess=a_shininess_f1;
	//lightSpecular=a_light_specular_v3;
	
	v_uvcoord0=a_uv_coord0_v2;
	gl_Position = u_mvp_m4x4 * a_vertex_coord_v4;
}
