Shader "Diffuse" {
Properties {
	_Color ("Main Color", Color) = (1,1,1,1)
	_MainTex ("Base (RGB)", Tex2D) = "white" {}
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
			return texture2D(sampler2d_MainTex, uv_out_MainTex);
		}
	}
}
}
}