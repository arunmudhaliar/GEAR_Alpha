#include "RoundedRectControl.h"

RoundedRectControl::RoundedRectControl()
{
	float rgb_top[4] ={0.21f, 0.21f, 0.21f, 1.0f};
	float rgb_bottom[4] ={0.21f*0.75f, 0.21f*0.75f, 0.21f*0.75f, 1.0f};
	float rgb_delta[4]={(rgb_bottom[0]-rgb_top[0]), (rgb_bottom[1]-rgb_top[1]), (rgb_bottom[2]-rgb_top[2]), (rgb_bottom[3]-rgb_top[3])};

	memcpy(m_rgb_top, rgb_top, sizeof(rgb_top));
	memcpy(m_rgb_bottom, rgb_bottom, sizeof(rgb_bottom));
	memcpy(m_rgb_delta, rgb_delta, sizeof(rgb_delta));

	m_step = 10;
	m_prounded_left_top_vertList = NULL;
	m_prounded_right_top_vertList = NULL;
	m_prounded_right_bottom_vertList = NULL;
	m_prounded_left_bottom_vertList = NULL;

	m_prounded_left_top_colorList = NULL;
	m_prounded_right_top_colorList = NULL;
	m_prounded_right_bottom_colorList = NULL;
	m_prounded_left_bottom_colorList = NULL;
}

RoundedRectControl::~RoundedRectControl()
{
	GE_DELETE_ARY(m_prounded_left_top_vertList);
	GE_DELETE_ARY(m_prounded_right_top_vertList);
	GE_DELETE_ARY(m_prounded_right_bottom_vertList);
	GE_DELETE_ARY(m_prounded_left_bottom_vertList);

	GE_DELETE_ARY(m_prounded_left_top_colorList);
	GE_DELETE_ARY(m_prounded_right_top_colorList);
	GE_DELETE_ARY(m_prounded_right_bottom_colorList);
	GE_DELETE_ARY(m_prounded_left_bottom_colorList);
}

void RoundedRectControl::create(float cx, float cy, float deltaHeight)
{
	m_type=1;

	const float horizontal_vertLst[8] =
	{
		cx,	deltaHeight,
		0,	deltaHeight,
		cx,	(m_type==0)?cy-deltaHeight:cy,
		0,	(m_type==0)?cy-deltaHeight:cy,
	};

	const float horizontal_colorLst[16] =
	{
		m_rgb_top[0]+m_rgb_delta[0]*(deltaHeight/cy), m_rgb_top[1]+m_rgb_delta[1]*(deltaHeight/cy), m_rgb_top[2]+m_rgb_delta[2]*(deltaHeight/cy), m_rgb_top[3]+m_rgb_delta[3]*(deltaHeight/cy),
		m_rgb_top[0]+m_rgb_delta[0]*(deltaHeight/cy), m_rgb_top[1]+m_rgb_delta[1]*(deltaHeight/cy), m_rgb_top[2]+m_rgb_delta[2]*(deltaHeight/cy), m_rgb_top[3]+m_rgb_delta[3]*(deltaHeight/cy),
		(m_type==0)?m_rgb_bottom[0]-m_rgb_delta[0]*(deltaHeight/cy):m_rgb_bottom[0], (m_type==0)?m_rgb_bottom[1]-m_rgb_delta[1]*(deltaHeight/cy):m_rgb_bottom[1], (m_type==0)?m_rgb_bottom[2]-m_rgb_delta[2]*(deltaHeight/cy):m_rgb_bottom[2], (m_type==0)?m_rgb_bottom[3]-m_rgb_delta[3]*(deltaHeight/cy):m_rgb_bottom[3],
		(m_type==0)?m_rgb_bottom[0]-m_rgb_delta[0]*(deltaHeight/cy):m_rgb_bottom[0], (m_type==0)?m_rgb_bottom[1]-m_rgb_delta[1]*(deltaHeight/cy):m_rgb_bottom[1], (m_type==0)?m_rgb_bottom[2]-m_rgb_delta[2]*(deltaHeight/cy):m_rgb_bottom[2], (m_type==0)?m_rgb_bottom[3]-m_rgb_delta[3]*(deltaHeight/cy):m_rgb_bottom[3]
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
		m_rgb_top[0], m_rgb_top[1], m_rgb_top[2], m_rgb_top[3],
		m_rgb_top[0], m_rgb_top[1], m_rgb_top[2], m_rgb_top[3],
		m_rgb_bottom[0], m_rgb_bottom[1], m_rgb_bottom[2], m_rgb_bottom[3],
		m_rgb_bottom[0], m_rgb_bottom[1], m_rgb_bottom[2], m_rgb_bottom[3]
	};

	memcpy(m_horizontal_vertLst, horizontal_vertLst, sizeof(horizontal_vertLst));
	memcpy(m_horizontal_colorLst, horizontal_colorLst, sizeof(horizontal_colorLst));
	memcpy(m_vertical_vertLst, vertical_vertLst, sizeof(vertical_vertLst));
	memcpy(m_vertical_colorLst, vertical_colorLst, sizeof(vertical_colorLst));

	//reset
	GE_DELETE_ARY(m_prounded_left_top_vertList);
	GE_DELETE_ARY(m_prounded_right_top_vertList);
	GE_DELETE_ARY(m_prounded_right_bottom_vertList);
	GE_DELETE_ARY(m_prounded_left_bottom_vertList);

	GE_DELETE_ARY(m_prounded_left_top_colorList);
	GE_DELETE_ARY(m_prounded_right_top_colorList);
	GE_DELETE_ARY(m_prounded_right_bottom_colorList);
	GE_DELETE_ARY(m_prounded_left_bottom_colorList);
	//

	float delta_angle=90/m_step;
	const int szz=(2+m_step)*2;
	const int cszz=(2+m_step)*4;

	m_prounded_left_top_vertList = new float[szz];
	m_prounded_right_top_vertList = new float[szz];
	m_prounded_right_bottom_vertList = new float[szz];
	m_prounded_left_bottom_vertList = new float[szz];

	m_prounded_left_top_colorList = new float[cszz];
	m_prounded_right_top_colorList = new float[cszz];
	m_prounded_right_bottom_colorList = new float[cszz];
	m_prounded_left_bottom_colorList = new float[cszz];

	//left top
	m_prounded_left_top_vertList[0]=deltaHeight;
	m_prounded_left_top_vertList[1]=deltaHeight;
	m_prounded_left_top_colorList[0]=m_rgb_top[0]+m_rgb_delta[0]*(m_prounded_left_top_vertList[1]/cy);
	m_prounded_left_top_colorList[1]=m_rgb_top[1]+m_rgb_delta[1]*(m_prounded_left_top_vertList[1]/cy);
	m_prounded_left_top_colorList[2]=m_rgb_top[2]+m_rgb_delta[2]*(m_prounded_left_top_vertList[1]/cy);
	m_prounded_left_top_colorList[3]=m_rgb_top[3]+m_rgb_delta[3]*(m_prounded_left_top_vertList[1]/cy);

	float angle=180;
	for(int xx=m_step;xx>=0;xx--)
	{
		m_prounded_left_top_vertList[(xx+1)*2+0]=m_prounded_left_top_vertList[0]+deltaHeight*pxMath::COSF(angle);
		m_prounded_left_top_vertList[(xx+1)*2+1]=m_prounded_left_top_vertList[1]+deltaHeight*pxMath::SINF(angle);

		float color_height=m_prounded_left_top_vertList[(xx+1)*2+1];
		m_prounded_left_top_colorList[(xx+1)*4+0]=m_rgb_top[0]+m_rgb_delta[0]*(color_height/cy);
		m_prounded_left_top_colorList[(xx+1)*4+1]=m_rgb_top[1]+m_rgb_delta[1]*(color_height/cy);
		m_prounded_left_top_colorList[(xx+1)*4+2]=m_rgb_top[2]+m_rgb_delta[2]*(color_height/cy);
		m_prounded_left_top_colorList[(xx+1)*4+3]=m_rgb_top[3]+m_rgb_delta[3]*(color_height/cy);
		angle+=delta_angle;
	}

	//right top
	m_prounded_right_top_vertList[0]=cx-deltaHeight;
	m_prounded_right_top_vertList[1]=deltaHeight;
	m_prounded_right_top_colorList[0]=m_rgb_top[0]+m_rgb_delta[0]*(m_prounded_left_top_vertList[1]/cy);
	m_prounded_right_top_colorList[1]=m_rgb_top[1]+m_rgb_delta[1]*(m_prounded_left_top_vertList[1]/cy);
	m_prounded_right_top_colorList[2]=m_rgb_top[2]+m_rgb_delta[2]*(m_prounded_left_top_vertList[1]/cy);
	m_prounded_right_top_colorList[3]=m_rgb_top[3]+m_rgb_delta[3]*(m_prounded_left_top_vertList[1]/cy);

	angle=270;
	for(int xx=m_step;xx>=0;xx--)
	{
		m_prounded_right_top_vertList[(xx+1)*2+0]=m_prounded_right_top_vertList[0]+deltaHeight*pxMath::COSF(angle);
		m_prounded_right_top_vertList[(xx+1)*2+1]=m_prounded_right_top_vertList[1]+deltaHeight*pxMath::SINF(angle);

		float color_height=m_prounded_right_top_vertList[(xx+1)*2+1];
		m_prounded_right_top_colorList[(xx+1)*4+0]=m_rgb_top[0]+m_rgb_delta[0]*(color_height/cy);
		m_prounded_right_top_colorList[(xx+1)*4+1]=m_rgb_top[1]+m_rgb_delta[1]*(color_height/cy);
		m_prounded_right_top_colorList[(xx+1)*4+2]=m_rgb_top[2]+m_rgb_delta[2]*(color_height/cy);
		m_prounded_right_top_colorList[(xx+1)*4+3]=m_rgb_top[3]+m_rgb_delta[3]*(color_height/cy);
		angle+=delta_angle;
	}

	if(m_type==0)
	{
		//right bottom
		m_prounded_right_bottom_vertList[0]=cx-deltaHeight;
		m_prounded_right_bottom_vertList[1]=cy-deltaHeight;
		m_prounded_right_bottom_colorList[0]=m_rgb_top[0]+m_rgb_delta[0]*(m_prounded_right_bottom_vertList[1]/cy);
		m_prounded_right_bottom_colorList[1]=m_rgb_top[1]+m_rgb_delta[1]*(m_prounded_right_bottom_vertList[1]/cy);
		m_prounded_right_bottom_colorList[2]=m_rgb_top[2]+m_rgb_delta[2]*(m_prounded_right_bottom_vertList[1]/cy);
		m_prounded_right_bottom_colorList[3]=m_rgb_top[3]+m_rgb_delta[3]*(m_prounded_right_bottom_vertList[1]/cy);

		angle=0;
		for(int xx=m_step;xx>=0;xx--)
		{
			m_prounded_right_bottom_vertList[(xx+1)*2+0]=m_prounded_right_bottom_vertList[0]+deltaHeight*pxMath::COSF(angle);
			m_prounded_right_bottom_vertList[(xx+1)*2+1]=m_prounded_right_bottom_vertList[1]+deltaHeight*pxMath::SINF(angle);

			float color_height=m_prounded_right_bottom_vertList[(xx+1)*2+1];
			m_prounded_right_bottom_colorList[(xx+1)*4+0]=m_rgb_top[0]+m_rgb_delta[0]*(color_height/cy);
			m_prounded_right_bottom_colorList[(xx+1)*4+1]=m_rgb_top[1]+m_rgb_delta[1]*(color_height/cy);
			m_prounded_right_bottom_colorList[(xx+1)*4+2]=m_rgb_top[2]+m_rgb_delta[2]*(color_height/cy);
			m_prounded_right_bottom_colorList[(xx+1)*4+3]=m_rgb_top[3]+m_rgb_delta[3]*(color_height/cy);
			angle+=delta_angle;
		}

		//left bottom
		m_prounded_left_bottom_vertList[0]=deltaHeight;
		m_prounded_left_bottom_vertList[1]=cy-deltaHeight;
		m_prounded_left_bottom_colorList[0]=m_rgb_top[0]+m_rgb_delta[0]*(m_prounded_left_bottom_vertList[1]/cy);
		m_prounded_left_bottom_colorList[1]=m_rgb_top[1]+m_rgb_delta[1]*(m_prounded_left_bottom_vertList[1]/cy);
		m_prounded_left_bottom_colorList[2]=m_rgb_top[2]+m_rgb_delta[2]*(m_prounded_left_bottom_vertList[1]/cy);
		m_prounded_left_bottom_colorList[3]=m_rgb_top[3]+m_rgb_delta[3]*(m_prounded_left_bottom_vertList[1]/cy);

		angle=90;
		for(int xx=m_step;xx>=0;xx--)
		{
			m_prounded_left_bottom_vertList[(xx+1)*2+0]=m_prounded_left_bottom_vertList[0]+deltaHeight*pxMath::COSF(angle);
			m_prounded_left_bottom_vertList[(xx+1)*2+1]=m_prounded_left_bottom_vertList[1]+deltaHeight*pxMath::SINF(angle);

			float color_height=m_prounded_left_bottom_vertList[(xx+1)*2+1];
			m_prounded_left_bottom_colorList[(xx+1)*4+0]=m_rgb_top[0]+m_rgb_delta[0]*(color_height/cy);
			m_prounded_left_bottom_colorList[(xx+1)*4+1]=m_rgb_top[1]+m_rgb_delta[1]*(color_height/cy);
			m_prounded_left_bottom_colorList[(xx+1)*4+2]=m_rgb_top[2]+m_rgb_delta[2]*(color_height/cy);
			m_prounded_left_bottom_colorList[(xx+1)*4+3]=m_rgb_top[3]+m_rgb_delta[3]*(color_height/cy);
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

	glVertexPointer(2, GL_FLOAT, 0, m_vertical_vertLst);
	glColorPointer(4, GL_FLOAT, 0, m_vertical_colorLst);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glVertexPointer(2, GL_FLOAT, 0, m_horizontal_vertLst);
	glColorPointer(4, GL_FLOAT, 0, m_horizontal_colorLst);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glVertexPointer(2, GL_FLOAT, 0, m_prounded_left_top_vertList);
	glColorPointer(4, GL_FLOAT, 0, m_prounded_left_top_colorList);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+m_step));

	glVertexPointer(2, GL_FLOAT, 0, m_prounded_right_top_vertList);
	glColorPointer(4, GL_FLOAT, 0, m_prounded_right_top_colorList);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+m_step));

	if(m_type==0)
	{
		glVertexPointer(2, GL_FLOAT, 0, m_prounded_right_bottom_vertList);
		glColorPointer(4, GL_FLOAT, 0, m_prounded_right_bottom_colorList);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (2+m_step));

		glVertexPointer(2, GL_FLOAT, 0, m_prounded_left_bottom_vertList);
		glColorPointer(4, GL_FLOAT, 0, m_prounded_left_bottom_colorList);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (2+m_step));
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}