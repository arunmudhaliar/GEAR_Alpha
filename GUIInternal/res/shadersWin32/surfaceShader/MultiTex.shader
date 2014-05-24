Shader "MultiTex" {
Properties {
	_Color ("Main Color", Color) = (1,1,1,1)
	_MainTex ("Base (RGB) Gloss (A)", Tex2D) = "white" {}
	_BumpMap ("Normal Map", Tex2D) = "bump" {}
}

SubShader {
__Pass{
	__vertex{
		attribute vec2 uv_in_MainTex;
		varying vec2 uv_out_MainTex;
		vec4 vertex_function()
		{
			uv_out_MainTex = uv_in_MainTex;
			return GEAR_MVP * vIN_Position;
		}
	}

	__fragment{
		varying vec2 uv_out_MainTex;
		uniform sampler2D sampler2d_MainTex;

		vec4 fragment_function()
		{
			return vec4(texture2D(sampler2d_MainTex, uv_out_MainTex).rgb, 1);
		}
	}
}

__Pass{
	__includeModule
	{
		PointLightStruct
		MaterialStruct
	}
	
	__vertex{
		// User-specified properties
		attribute vec2 uv_in_BumpMap;
		varying vec2 uv_out_BumpMap;
		attribute vec2 uv_in_MainTex;
		varying vec2 uv_out_MainTex;

		vec4 vertex_function()
		{
            uv_out_BumpMap = uv_in_BumpMap;
			uv_out_MainTex = uv_in_MainTex;
			return GEAR_MVP * vIN_Position;
		}
	}

	__fragment{
	    uniform sampler2D sampler2d_BumpMap;
	    uniform sampler2D sampler2d_MainTex;
		varying vec2 uv_out_BumpMap;
		varying vec2 uv_out_MainTex;

		vec4 fragment_function()
		{
 
            return vec4((texture2D(sampler2d_MainTex, vec2(sin(radians(Time.time.x)), uv_out_MainTex.s))+texture2D(sampler2d_BumpMap, uv_out_BumpMap)).rgb, 1);
		}
	}
}
}
}