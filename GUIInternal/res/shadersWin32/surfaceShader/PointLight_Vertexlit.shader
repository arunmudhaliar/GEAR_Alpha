Shader "PointLight-VertexLit" {

SubShader {
	__includeModule{	PointLightStruct MaterialStruct	}
	
	__vertex{
		vec4 pointLight()
		{
			float nDotVP;       // normal * light direction
			float nDotR;        // normal * light reflection vector
			float pf;           // power factor
			float attenuation;  // computed attenuation factor
			float d;            // distance from surface to light position
			vec3 VP;            // direction from surface to light position
			vec3 reflection;    // direction of maximum highlights
			vec3 vNormal;
			
			// Get surface normal in eye coordinates
			vNormal = mat3(GEAR_NORMAL_MATRIX) * vIN_Normal;
			
			// Get vertex position in eye coordinates
			vec4 vertexPos = GEAR_MV * vIN_Position;
			vec3 vertexEyePos = vertexPos.xyz / vertexPos.w;
			vec3 vPosition = vertexEyePos;
			VP = vec3 (light.position) - vec3 (vPosition);

			
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
		
		varying vec4 vOUT_Color;
		vec4 vertex_function()
		{
			vOUT_Color=pointLight();
			return GEAR_MVP * vIN_Position;
		}
	}

	__fragment{
		varying vec4 vOUT_Color;
		vec4 fragment_function()
		{
			return vOUT_Color;
		}
	}
}
}