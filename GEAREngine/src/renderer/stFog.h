#pragma once

#include "../core/vector4.h"

struct stFog
{
	enum EFOG_TYPE
	{
		FOG_LINEAR,
		FOG_EXP,
		FOG_EXP2
	};

	stFog()
	{
		fog_type = FOG_LINEAR;
		fog_density=0.0f;
		//fog_start=200.0f;
		//fog_end=1000.0f;
		fog_start=0.0f;
		fog_end=1.0f;
		calculateScale();
		fog_color.set(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void setFogDensity(float density)	{	fog_density=density; }
	void setFogStart(float start)		{	fog_start=start; }
	void setFogEnd(float end)			{	fog_end=end; }
	void calculateScale()				{	fog_scale=(fog_end-fog_start);	}
	void setFogColor(vector4f clr)		{	fog_color=clr;	}
	void setFogType(EFOG_TYPE type)		{	fog_type=type;	}

    float fog_density;
	float fog_start;
	float fog_end;
	float fog_scale;
	vector4f fog_color;
	EFOG_TYPE fog_type;

};