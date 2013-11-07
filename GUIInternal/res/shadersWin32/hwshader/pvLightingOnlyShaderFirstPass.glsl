#ifdef GEAR_VERTEX_SHADER
#version 120

struct Light
{
    vec4    position;
    vec4    ambient;
    vec4    diffuse;
    vec4    specular;
    float   constant_attenuation;
    float   linear_attenuation;
    float   quadratic_attenuation;
    vec3    spot_direction;
    float   spot_cutoff;
    float   spot_exponent;
};
uniform Light light;

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

varying vec4 vOUT_Position;
varying vec3 vOUT_Normal;
varying vec4 vOUT_Color;
varying vec2 vOUT_UVCoord0;
varying vec2 vOUT_UVCoord1;
varying vec2 vOUT_UVCoord2;
varying vec2 vOUT_UVCoord3;

vec4 pointLight ()
{
    float nDotVP;       // normal * light direction
    float nDotR;        // normal * light reflection vector
    float pf;           // power factor
    float attenuation;  // computed attenuation factor
    float d;            // distance from surface to light position
    vec3 VP;            // direction from surface to light position
    vec3 reflection;    // direction of maximum highlights
	vec3 vNormal;
	
#ifdef GEAR_VERTEX_SHADER
	// Get surface normal in eye coordinates
    vNormal = mat3(GEAR_NORMAL_MATRIX) * vIN_Normal;
	
	// Get vertex position in eye coordinates
    vec4 vertexPos = GEAR_MV * vIN_Position;
    vec3 vertexEyePos = vertexPos.xyz / vertexPos.w;
    vec3 vPosition = vertexEyePos;
	VP = vec3 (light.position) - vec3 (vPosition);
#elif defined (GEAR_FRAGMENT_SHADER)
	vNormal = vOUT_Normal;
	VP = vec3 (light.position) - vec3 (vOUT_Position);
#endif
	
    // Compute distance between surface and light position
    d = length (VP);
 
    // Normalize the vector from surface to light position
    VP = normalize (VP);
 
    // Compute attenuation
    attenuation = 1.f / (light.constant_attenuation +
                         light.linear_attenuation * d +
                         light.quadratic_attenuation * d * d);
 
    reflection = normalize (reflect (-normalize (VP), normalize
                (vNormal)));
 
    nDotVP = max (0.f, dot (vNormal, VP));
    nDotR = max (0.f, dot (normalize (vNormal), reflection));
 
    if (nDotVP == 0.f)
        pf = 0.f;
    else
        pf = pow (nDotR, material.shininess);
 
    vec4 ambient = material.ambient * light.ambient * attenuation;
    vec4 diffuse = material.diffuse * light.diffuse * nDotVP * attenuation;
    vec4 specular = material.specular * light.specular * pf * attenuation;
 
    return ambient + diffuse + specular;
}

void main()
{
	vOUT_Color=pointLight();
	gl_Position = GEAR_MVP * vIN_Position;
}
#elif defined (GEAR_FRAGMENT_SHADER)
varying vec4 vOUT_Color;
void main()
{
	gl_FragColor = vOUT_Color;
}
#endif
