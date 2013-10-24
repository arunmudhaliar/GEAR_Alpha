#ifdef GEAR_VERTEX_SHADER

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform mat4 GEAR_MV;
uniform mat4 GEAR_MVP;
uniform mat4 GEAR_NORMAL_MATRIX;

attribute vec4 vIN_Position;
attribute vec3 vIN_Normal;
attribute vec4 vIN_Color;
attribute vec2 vIN_UVCoord0;
attribute vec2 vIN_UVCoord1;
attribute vec2 vIN_UVCoord2;
attribute vec2 vIN_UVCoord3;

varying vec4 vOUT_Position;
varying vec3 vOUT_Normal;
varying vec4 vOUT_Color;
varying vec2 vOUT_UVCoord0;
varying vec2 vOUT_UVCoord1;
varying vec2 vOUT_UVCoord2;
varying vec2 vOUT_UVCoord3;

attribute vec4 a_vertex_coord_v4;
attribute vec2 a_uv_coord0_v2;
uniform mat4 u_mvp_m4x4;
varying vec2 v_uv_coord0_v2;

void main()
{
	vOUT_UVCoord0 = vIN_UVCoord0;
	gl_Position = GEAR_MVP * vIN_Position;
}
#elif defined (GEAR_FRAGMENT_SHADER)

varying vec4 vOUT_Color;
varying vec2 vOUT_UVCoord0;
uniform sampler2D sampler2d_diffuse;
void main()
{
	gl_FragColor = texture2D(sampler2d_diffuse, vOUT_UVCoord0);// * v_destinationcolor_v4;
}
#endif
