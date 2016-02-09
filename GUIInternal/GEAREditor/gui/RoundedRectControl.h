#pragma once

#include "geGUIBase.h"

class RoundedRectControl
{
public:
	RoundedRectControl();
	~RoundedRectControl();

	void create(float cx, float cy, float deltaHeight);
	void draw(float x, float y);

private:
	int rectType;
	float topRGB[4];
	float bottomRGB[4];
	float deltaRGB[4];
	
	float horizontalVertexArray[8];
	float horizontalColorArray[16];
	float verticalVertexArray[8];
	float verticalColorArray[16];
	int noOfSteps;
	float* roundedLeftTopVertexArray;
	float* roundedRightTopVertexArray;
	float* roundedRightBottomVertexArray;
	float* roundedLeftBottomVertexArray;

	float* roundedLeftTopColorArray;
	float* roundedRightTopColorArray;
	float* roundedRightBottomColorArray;
	float* roundedLeftBottomColorArray;
};