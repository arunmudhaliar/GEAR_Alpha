#ifndef ROUNDEDRECTCONTROL_H
#define ROUNDEDRECTCONTROL_H

#include "geGUIBase.h"

class RoundedRectControl
{
public:
	RoundedRectControl();
	~RoundedRectControl();

	void create(float cx, float cy, float deltaHeight);
	void draw(float x, float y);

private:
	int m_type;
	float m_rgb_top[4];
	float m_rgb_bottom[4];
	float m_rgb_delta[4];
	
	float m_horizontal_vertLst[8];
	float m_horizontal_colorLst[16];
	float m_vertical_vertLst[8];
	float m_vertical_colorLst[16];
	int m_step;
	float* m_prounded_left_top_vertList;
	float* m_prounded_right_top_vertList;
	float* m_prounded_right_bottom_vertList;
	float* m_prounded_left_bottom_vertList;

	float* m_prounded_left_top_colorList;
	float* m_prounded_right_top_colorList;
	float* m_prounded_right_bottom_colorList;
	float* m_prounded_left_bottom_colorList;
};

#endif