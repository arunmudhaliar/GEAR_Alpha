#include "RoundedRectControl.h"

RoundedRectControl::RoundedRectControl()
{
	float rgb_top[4] ={0.21f, 0.21f, 0.21f, 1.0f};
	float rgb_bottom[4] ={0.21f*0.75f, 0.21f*0.75f, 0.21f*0.75f, 1.0f};
	float rgb_delta[4]={(rgb_bottom[0]-rgb_top[0]), (rgb_bottom[1]-rgb_top[1]), (rgb_bottom[2]-rgb_top[2]), (rgb_bottom[3]-rgb_top[3])};

	memcpy(topRGB, rgb_top, sizeof(rgb_top));
	memcpy(bottomRGB, rgb_bottom, sizeof(rgb_bottom));
	memcpy(deltaRGB, rgb_delta, sizeof(rgb_delta));

	noOfSteps = 10;
	roundedLeftTopVertexArray = NULL;
	roundedRightTopVertexArray = NULL;
	roundedRightBottomVertexArray = NULL;
	roundedLeftBottomVertexArray = NULL;

	roundedLeftTopColorArray = NULL;
	roundedRightTopColorArray = NULL;
	roundedRightBottomColorArray = NULL;
	roundedLeftBottomColorArray = NULL;
}

RoundedRectControl::~RoundedRectControl()
{
	GE_DELETE_ARY(roundedLeftTopVertexArray);
	GE_DELETE_ARY(roundedRightTopVertexArray);
	GE_DELETE_ARY(roundedRightBottomVertexArray);
	GE_DELETE_ARY(roundedLeftBottomVertexArray);

	GE_DELETE_ARY(roundedLeftTopColorArray);
	GE_DELETE_ARY(roundedRightTopColorArray);
	GE_DELETE_ARY(roundedRightBottomColorArray);
	GE_DELETE_ARY(roundedLeftBottomColorArray);
}

void RoundedRectControl::create(float cx, float cy, float deltaHeight)
{
	rectType=1;

	const float horizontal_vertLst[8] =
	{
		cx,	deltaHeight,
		0,	deltaHeight,
		cx,	(rectType==0)?cy-deltaHeight:cy,
		0,	(rectType==0)?cy-deltaHeight:cy,
	};

	const float horizontal_colorLst[16] =
	{
		topRGB[0]+deltaRGB[0]*(deltaHeight/cy), topRGB[1]+deltaRGB[1]*(deltaHeight/cy), topRGB[2]+deltaRGB[2]*(deltaHeight/cy), topRGB[3]+deltaRGB[3]*(deltaHeight/cy),
		topRGB[0]+deltaRGB[0]*(deltaHeight/cy), topRGB[1]+deltaRGB[1]*(deltaHeight/cy), topRGB[2]+deltaRGB[2]*(deltaHeight/cy), topRGB[3]+deltaRGB[3]*(deltaHeight/cy),
		(rectType==0)?bottomRGB[0]-deltaRGB[0]*(deltaHeight/cy):bottomRGB[0], (rectType==0)?bottomRGB[1]-deltaRGB[1]*(deltaHeight/cy):bottomRGB[1], (rectType==0)?bottomRGB[2]-deltaRGB[2]*(deltaHeight/cy):bottomRGB[2], (rectType==0)?bottomRGB[3]-deltaRGB[3]*(deltaHeight/cy):bottomRGB[3],
		(rectType==0)?bottomRGB[0]-deltaRGB[0]*(deltaHeight/cy):bottomRGB[0], (rectType==0)?bottomRGB[1]-deltaRGB[1]*(deltaHeight/cy):bottomRGB[1], (rectType==0)?bottomRGB[2]-deltaRGB[2]*(deltaHeight/cy):bottomRGB[2], (rectType==0)?bottomRGB[3]-deltaRGB[3]*(deltaHeight/cy):bottomRGB[3]
	};
	
	const float vertical_vertLst[8] =
	{
		cx-deltaHeight,	0,
		deltaHeight,	0,
		cx-deltaHeight,	cy,
		deltaHeight,	cy,
	};

	const float vertical_colorLst[16] =
	{
		topRGB[0], topRGB[1], topRGB[2], topRGB[3],
		topRGB[0], topRGB[1], topRGB[2], topRGB[3],
		bottomRGB[0], bottomRGB[1], bottomRGB[2], bottomRGB[3],
		bottomRGB[0], bottomRGB[1], bottomRGB[2], bottomRGB[3]
	};

	memcpy(horizontalVertexArray, horizontal_vertLst, sizeof(horizontal_vertLst));
	memcpy(horizontalColorArray, horizontal_colorLst, sizeof(horizontal_colorLst));
	memcpy(verticalVertexArray, vertical_vertLst, sizeof(vertical_vertLst));
	memcpy(verticalColorArray, vertical_colorLst, sizeof(vertical_colorLst));

	//reset
	GE_DELETE_ARY(roundedLeftTopVertexArray);
	GE_DELETE_ARY(roundedRightTopVertexArray);
	GE_DELETE_ARY(roundedRightBottomVertexArray);
	GE_DELETE_ARY(roundedLeftBottomVertexArray);

	GE_DELETE_ARY(roundedLeftTopColorArray);
	GE_DELETE_ARY(roundedRightTopColorArray);
	GE_DELETE_ARY(roundedRightBottomColorArray);
	GE_DELETE_ARY(roundedLeftBottomColorArray);
	//

	float delta_angle=90/noOfSteps;
	const int szz=(2+noOfSteps)*2;
	const int cszz=(2+noOfSteps)*4;

	roundedLeftTopVertexArray = new float[szz];
	roundedRightTopVertexArray = new float[szz];
	roundedRightBottomVertexArray = new float[szz];
	roundedLeftBottomVertexArray = new float[szz];

	roundedLeftTopColorArray = new float[cszz];
	roundedRightTopColorArray = new float[cszz];
	roundedRightBottomColorArray = new float[cszz];
	roundedLeftBottomColorArray = new float[cszz];

	//left top
	roundedLeftTopVertexArray[0]=deltaHeight;
	roundedLeftTopVertexArray[1]=deltaHeight;
	roundedLeftTopColorArray[0]=topRGB[0]+deltaRGB[0]*(roundedLeftTopVertexArray[1]/cy);
	roundedLeftTopColorArray[1]=topRGB[1]+deltaRGB[1]*(roundedLeftTopVertexArray[1]/cy);
	roundedLeftTopColorArray[2]=topRGB[2]+deltaRGB[2]*(roundedLeftTopVertexArray[1]/cy);
	roundedLeftTopColorArray[3]=topRGB[3]+deltaRGB[3]*(roundedLeftTopVertexArray[1]/cy);

	float angle=180;
	for(int xx=noOfSteps;xx>=0;xx--)
	{
		roundedLeftTopVertexArray[(xx+1)*2+0]=roundedLeftTopVertexArray[0]+deltaHeight*pxMath::COSF(angle);
		roundedLeftTopVertexArray[(xx+1)*2+1]=roundedLeftTopVertexArray[1]+deltaHeight*pxMath::SINF(angle);

		float color_height=roundedLeftTopVertexArray[(xx+1)*2+1];
		roundedLeftTopColorArray[(xx+1)*4+0]=topRGB[0]+deltaRGB[0]*(color_height/cy);
		roundedLeftTopColorArray[(xx+1)*4+1]=topRGB[1]+deltaRGB[1]*(color_height/cy);
		roundedLeftTopColorArray[(xx+1)*4+2]=topRGB[2]+deltaRGB[2]*(color_height/cy);
		roundedLeftTopColorArray[(xx+1)*4+3]=topRGB[3]+deltaRGB[3]*(color_height/cy);
		angle+=delta_angle;
	}

	//right top
	roundedRightTopVertexArray[0]=cx-deltaHeight;
	roundedRightTopVertexArray[1]=deltaHeight;
	roundedRightTopColorArray[0]=topRGB[0]+deltaRGB[0]*(roundedLeftTopVertexArray[1]/cy);
	roundedRightTopColorArray[1]=topRGB[1]+deltaRGB[1]*(roundedLeftTopVertexArray[1]/cy);
	roundedRightTopColorArray[2]=topRGB[2]+deltaRGB[2]*(roundedLeftTopVertexArray[1]/cy);
	roundedRightTopColorArray[3]=topRGB[3]+deltaRGB[3]*(roundedLeftTopVertexArray[1]/cy);

	angle=270;
	for(int xx=noOfSteps;xx>=0;xx--)
	{
		roundedRightTopVertexArray[(xx+1)*2+0]=roundedRightTopVertexArray[0]+deltaHeight*pxMath::COSF(angle);
		roundedRightTopVertexArray[(xx+1)*2+1]=roundedRightTopVertexArray[1]+deltaHeight*pxMath::SINF(angle);

		float color_height=roundedRightTopVertexArray[(xx+1)*2+1];
		roundedRightTopColorArray[(xx+1)*4+0]=topRGB[0]+deltaRGB[0]*(color_height/cy);
		roundedRightTopColorArray[(xx+1)*4+1]=topRGB[1]+deltaRGB[1]*(color_height/cy);
		roundedRightTopColorArray[(xx+1)*4+2]=topRGB[2]+deltaRGB[2]*(color_height/cy);
		roundedRightTopColorArray[(xx+1)*4+3]=topRGB[3]+deltaRGB[3]*(color_height/cy);
		angle+=delta_angle;
	}

	if(rectType==0)
	{
		//right bottom
		roundedRightBottomVertexArray[0]=cx-deltaHeight;
		roundedRightBottomVertexArray[1]=cy-deltaHeight;
		roundedRightBottomColorArray[0]=topRGB[0]+deltaRGB[0]*(roundedRightBottomVertexArray[1]/cy);
		roundedRightBottomColorArray[1]=topRGB[1]+deltaRGB[1]*(roundedRightBottomVertexArray[1]/cy);
		roundedRightBottomColorArray[2]=topRGB[2]+deltaRGB[2]*(roundedRightBottomVertexArray[1]/cy);
		roundedRightBottomColorArray[3]=topRGB[3]+deltaRGB[3]*(roundedRightBottomVertexArray[1]/cy);

		angle=0;
		for(int xx=noOfSteps;xx>=0;xx--)
		{
			roundedRightBottomVertexArray[(xx+1)*2+0]=roundedRightBottomVertexArray[0]+deltaHeight*pxMath::COSF(angle);
			roundedRightBottomVertexArray[(xx+1)*2+1]=roundedRightBottomVertexArray[1]+deltaHeight*pxMath::SINF(angle);

			float color_height=roundedRightBottomVertexArray[(xx+1)*2+1];
			roundedRightBottomColorArray[(xx+1)*4+0]=topRGB[0]+deltaRGB[0]*(color_height/cy);
			roundedRightBottomColorArray[(xx+1)*4+1]=topRGB[1]+deltaRGB[1]*(color_height/cy);
			roundedRightBottomColorArray[(xx+1)*4+2]=topRGB[2]+deltaRGB[2]*(color_height/cy);
			roundedRightBottomColorArray[(xx+1)*4+3]=topRGB[3]+deltaRGB[3]*(color_height/cy);
			angle+=delta_angle;
		}

		//left bottom
		roundedLeftBottomVertexArray[0]=deltaHeight;
		roundedLeftBottomVertexArray[1]=cy-deltaHeight;
		roundedLeftBottomColorArray[0]=topRGB[0]+deltaRGB[0]*(roundedLeftBottomVertexArray[1]/cy);
		roundedLeftBottomColorArray[1]=topRGB[1]+deltaRGB[1]*(roundedLeftBottomVertexArray[1]/cy);
		roundedLeftBottomColorArray[2]=topRGB[2]+deltaRGB[2]*(roundedLeftBottomVertexArray[1]/cy);
		roundedLeftBottomColorArray[3]=topRGB[3]+deltaRGB[3]*(roundedLeftBottomVertexArray[1]/cy);

		angle=90;
		for(int xx=noOfSteps;xx>=0;xx--)
		{
			roundedLeftBottomVertexArray[(xx+1)*2+0]=roundedLeftBottomVertexArray[0]+deltaHeight*pxMath::COSF(angle);
			roundedLeftBottomVertexArray[(xx+1)*2+1]=roundedLeftBottomVertexArray[1]+deltaHeight*pxMath::SINF(angle);

			float color_height=roundedLeftBottomVertexArray[(xx+1)*2+1];
			roundedLeftBottomColorArray[(xx+1)*4+0]=topRGB[0]+deltaRGB[0]*(color_height/cy);
			roundedLeftBottomColorArray[(xx+1)*4+1]=topRGB[1]+deltaRGB[1]*(color_height/cy);
			roundedLeftBottomColorArray[(xx+1)*4+2]=topRGB[2]+deltaRGB[2]*(color_height/cy);
			roundedLeftBottomColorArray[(xx+1)*4+3]=topRGB[3]+deltaRGB[3]*(color_height/cy);
			angle+=delta_angle;
		}
	}
}

void RoundedRectControl::draw(float x, float y)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	//draw the horizontal rect

	glVertexPointer(2, GL_FLOAT, 0, verticalVertexArray);
	glColorPointer(4, GL_FLOAT, 0, verticalColorArray);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glVertexPointer(2, GL_FLOAT, 0, horizontalVertexArray);
	glColorPointer(4, GL_FLOAT, 0, horizontalColorArray);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glVertexPointer(2, GL_FLOAT, 0, roundedLeftTopVertexArray);
	glColorPointer(4, GL_FLOAT, 0, roundedLeftTopColorArray);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+noOfSteps));

	glVertexPointer(2, GL_FLOAT, 0, roundedRightTopVertexArray);
	glColorPointer(4, GL_FLOAT, 0, roundedRightTopColorArray);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+noOfSteps));

	if(rectType==0)
	{
		glVertexPointer(2, GL_FLOAT, 0, roundedRightBottomVertexArray);
		glColorPointer(4, GL_FLOAT, 0, roundedRightBottomColorArray);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (2+noOfSteps));

		glVertexPointer(2, GL_FLOAT, 0, roundedLeftBottomVertexArray);
		glColorPointer(4, GL_FLOAT, 0, roundedLeftBottomColorArray);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (2+noOfSteps));
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}