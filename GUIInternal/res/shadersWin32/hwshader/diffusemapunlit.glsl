#ifdef GEAR_VERTEX_SHADER

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
uniform Material material;

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
attribute vec2 uv_in_MainTex;


varying vec4 vOUT_Position;
varying vec3 vOUT_Normal;
varying vec4 vOUT_Color;
varying vec2 vOUT_UVCoord0;
varying vec2 vOUT_UVCoord1;
varying vec2 vOUT_UVCoord2;
varying vec2 vOUT_UVCoord3;
varying vec2 uv_out_MainTex;

vec4 vertex_function()
{
	vOUT_UVCoord0 = vIN_UVCoord0;
	return GEAR_MVP * vIN_Position;
}

void main()
{
	gl_Position = vertex_function();
}
#elif defined (GEAR_FRAGMENT_SHADER)

varying vec2 vOUT_UVCoord0;
uniform sampler2D sampler2d_diffuse;

vec4 fragment_function()
{
	return texture2D(sampler2d_diffuse, vOUT_UVCoord0);
}

void main()
{
	gl_FragColor = fragment_function();
}
#endif
