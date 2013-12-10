#include "geWindow.h"
#include "geGUIManager.h"

void drawRoundedRectangle(float x, float y, float cx, float cy, float deltaHeight);

geWindow::geWindow(const char* name):
	geGUIBase(GEGUI_WINDOW, name)
{
	m_bCanMove=false;
	m_pToolBar=NULL;
}

geWindow::~geWindow()
{
	GE_DELETE(m_pToolBar);
}

void geWindow::create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx, float cy, bool bCreateToolBar)
{
	createBase(renderer, parent);

	m_fTitleWidth=0;
	for(int index=0;index<strlen(m_szName);index++)
	{
		m_fTitleWidth+=geGUIManager::g_pFontArial10_84Ptr->getCharWidth(m_szName[index]);
	}

	m_bCanMove=false;
	m_pIamOnLayout=NULL;
	setPos(x, y);
	setSize(cx, cy);
	setTitleColor(0.12f, 0.12f, 0.12f, 1.0f);
	setClientColor(0.2f, 0.2f, 0.2f, 1.0f);
	setSizable(true);
	m_pToolBar=NULL;
	if(bCreateToolBar)
	{
		m_pToolBar = new geToolBar();
		m_pToolBar->create(renderer, this, 0, -GE_TOOLBAR_HEIGHT/*GE_WND_TITLE_HEIGHT*/, cx, GE_TOOLBAR_HEIGHT);
	}

	onCreate();
}

void geWindow::show()
{
}

void geWindow::hide()
{
}

void geWindow::enable()
{
}

void geWindow::disable()
{
}

void geWindow::update(float dt)
{
}

void geWindow::setTitleColor(float r, float g, float b, float a)
{
	setColor(&m_cVBTitle, r, g, b, a);
}

void geWindow::setClientColor(float r, float g, float b, float a)
{
	setColor(&m_cVBClientArea, r, g, b, a);
}

void geWindow::drawTitleAndToolBar(float xoff, float yoff, bool bActiveWindow, bool bFirstWindow)
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(m_cPos.x+xoff, m_cPos.y+yoff, 0);
	if(bFirstWindow)
	{
		drawRect(&m_cVBTitle);
	}

	if(bActiveWindow)
		drawRoundedRectangle(1, 3, m_fTitleWidth+30, GE_WND_TITLE_HEIGHT-3, 5);
	geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 15, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	//drawTriangle(&m_cVBLayoutToggleButtonLine[3*0], 0.05f, 0.05f, 0.05f, 1.0f, 3);
	if(m_pToolBar && bActiveWindow)
	{
		glTranslatef(-xoff, -yoff, 0);
		m_pToolBar->draw();
	}
	glPopMatrix();
}

void geWindow::draw()
{
	glViewport(m_cPos.x+m_pIamOnLayout->getPos().x, (m_pRenderer->getViewPortSz().y)-(m_cPos.y+m_pIamOnLayout->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/);	
	glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
		glLoadIdentity();
		gluOrtho2D((int)0, (int)(m_cSize.x/*+2.0f*/), (int)(m_cSize.y-getTopMarginOffsetHeight()/*+2.0f*/), (int)0);
		glMatrixMode(GL_MODELVIEW);

		drawRect(&m_cVBClientArea);
		onDraw();
	//glPopMatrix();
}



void geWindow::clearVarsAfterWindowMoved()
{
	m_bCanMove = false;
}

//bool geWindow::selectWindow(int x, int y)
//{
//	bool bSelected = isPointInsideWindow(x, y);
//	m_bCanMove = isPointInsideWindowTitle(x, y);
//
//	return bSelected;
//}

bool geWindow::isPointInsideWindowTitle(int x, int y)
{
	return (x>m_cPos.x && x<m_cPos.x+m_cSize.x && y>m_cPos.y && y<m_cPos.y+GE_WND_TITLE_HEIGHT);
}

void geWindow::onCreate()
{
}

void geWindow::onPosition(float x, float y, int flag)
{
}

void geWindow::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx,	0,
		0,	0,
		cx,	GE_WND_TITLE_HEIGHT,
		0,	GE_WND_TITLE_HEIGHT,
	};
	memcpy(m_cVBTitle.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	float cy_val=cy-getTopMarginOffsetHeight();
	if(cy_val<=0.0f)cy_val=0.0f;
	const float clientarea_vertLst[8] =
	{
		cx,	0,
		0,	0,
		cx,	cy_val,
		0,	cy_val,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, clientarea_vertLst, sizeof(clientarea_vertLst));


	const float clientarea_linevertLst[8] =
	{
		cx-1,	1/*GE_WND_TITLE_HEIGHT*/,
		1,		1/*GE_WND_TITLE_HEIGHT*/,
		1,		cy_val-1,
		cx-1,	cy_val-1,
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));

	int h=GE_WND_TITLE_HEIGHT-4;
	const float togglebutton_linevertLst[6*2] =
	{
		5,		(GE_WND_TITLE_HEIGHT-4),
		5+8,	(GE_WND_TITLE_HEIGHT-4)-(h>>2),
		5,		(GE_WND_TITLE_HEIGHT-4)-(h>>1),

		5+4,	(GE_WND_TITLE_HEIGHT-4),
		5+8,	(GE_WND_TITLE_HEIGHT-4)-(h>>1),
		5,		(GE_WND_TITLE_HEIGHT-4)-(h>>1),
	};
	memcpy(m_cVBLayoutToggleButtonLine, togglebutton_linevertLst, sizeof(togglebutton_linevertLst));

	if(m_pToolBar)
		m_pToolBar->setSize(cx, GE_TOOLBAR_HEIGHT);

	geGUIBase::onSize(cx, cy, flag);
}

void geWindow::onDraw()
{
}

void geWindow::onDestroy()
{
}

bool geWindow::onMouseLButtonDown(float x, float y, int nFlag)
{
	return geGUIBase::onMouseLButtonDown(x, y, nFlag);
}

bool geWindow::onMouseLButtonUp(float x, float y, int nFlag)
{
	return geGUIBase::onMouseLButtonUp(x, y, nFlag);
}

bool geWindow::onMouseMove(float x, float y, int flag)
{
	return geGUIBase::onMouseMove(x, y, flag);
}

void geWindow::onMouseEnterClientArea()
{
}

void geWindow::onMouseExitClientArea()
{
	//CancelEngagedControls();
}

geVector2f geWindow::getAbsolutePositionOnScreen()
{
	return geVector2f(m_cPos.x+m_pIamOnLayout->getPos().x, m_cPos.y+m_pIamOnLayout->getPos().y);
}

void drawRoundedRectangle(float x, float y, float cx, float cy, float deltaHeight)
{
	int type=1;

	float rgb_top[4] ={0.21f, 0.21f, 0.21f, 1.0f};
	float rgb_bottom[4] ={0.21f*0.75f, 0.21f*0.75f, 0.21f*0.75f, 1.0f};
	float rgb_delta[4]={(rgb_bottom[0]-rgb_top[0]), (rgb_bottom[1]-rgb_top[1]), (rgb_bottom[2]-rgb_top[2]), (rgb_bottom[3]-rgb_top[3])};

	const float horizontal_vertLst[8] =
	{
		cx,	deltaHeight,
		0,	deltaHeight,
		cx,	(type==0)?cy-deltaHeight:cy,
		0,	(type==0)?cy-deltaHeight:cy,
	};

	const float horizontal_colorLst[16] =
	{
		rgb_top[0]+rgb_delta[0]*(deltaHeight/cy), rgb_top[1]+rgb_delta[1]*(deltaHeight/cy), rgb_top[2]+rgb_delta[2]*(deltaHeight/cy), rgb_top[3]+rgb_delta[3]*(deltaHeight/cy),
		rgb_top[0]+rgb_delta[0]*(deltaHeight/cy), rgb_top[1]+rgb_delta[1]*(deltaHeight/cy), rgb_top[2]+rgb_delta[2]*(deltaHeight/cy), rgb_top[3]+rgb_delta[3]*(deltaHeight/cy),
		(type==0)?rgb_bottom[0]-rgb_delta[0]*(deltaHeight/cy):rgb_bottom[0], (type==0)?rgb_bottom[1]-rgb_delta[1]*(deltaHeight/cy):rgb_bottom[1], (type==0)?rgb_bottom[2]-rgb_delta[2]*(deltaHeight/cy):rgb_bottom[2], (type==0)?rgb_bottom[3]-rgb_delta[3]*(deltaHeight/cy):rgb_bottom[3],
		(type==0)?rgb_bottom[0]-rgb_delta[0]*(deltaHeight/cy):rgb_bottom[0], (type==0)?rgb_bottom[1]-rgb_delta[1]*(deltaHeight/cy):rgb_bottom[1], (type==0)?rgb_bottom[2]-rgb_delta[2]*(deltaHeight/cy):rgb_bottom[2], (type==0)?rgb_bottom[3]-rgb_delta[3]*(deltaHeight/cy):rgb_bottom[3]
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
		rgb_top[0], rgb_top[1], rgb_top[2], rgb_top[3],
		rgb_top[0], rgb_top[1], rgb_top[2], rgb_top[3],
		rgb_bottom[0], rgb_bottom[1], rgb_bottom[2], rgb_bottom[3],
		rgb_bottom[0], rgb_bottom[1], rgb_bottom[2], rgb_bottom[3]
	};

	const int step=10;
	float delta_angle=90/step;
	const int szz=(2+step)*2;
	const int cszz=(2+step)*4;
	float rounded_left_top_vertList[szz];
	float rounded_right_top_vertList[szz];
	float rounded_right_bottom_vertList[szz];
	float rounded_left_bottom_vertList[szz];

	float rounded_left_top_colorList[cszz];
	float rounded_right_top_colorList[cszz];
	float rounded_right_bottom_colorList[cszz];
	float rounded_left_bottom_colorList[cszz];

	//left top
	rounded_left_top_vertList[0]=deltaHeight;
	rounded_left_top_vertList[1]=deltaHeight;
	rounded_left_top_colorList[0]=rgb_top[0]+rgb_delta[0]*(rounded_left_top_vertList[1]/cy);
	rounded_left_top_colorList[1]=rgb_top[1]+rgb_delta[1]*(rounded_left_top_vertList[1]/cy);
	rounded_left_top_colorList[2]=rgb_top[2]+rgb_delta[2]*(rounded_left_top_vertList[1]/cy);
	rounded_left_top_colorList[3]=rgb_top[3]+rgb_delta[3]*(rounded_left_top_vertList[1]/cy);

	float angle=180;
	for(int xx=step;xx>=0;xx--)
	{
		rounded_left_top_vertList[(xx+1)*2+0]=rounded_left_top_vertList[0]+deltaHeight*gxMath::COSF(angle);
		rounded_left_top_vertList[(xx+1)*2+1]=rounded_left_top_vertList[1]+deltaHeight*gxMath::SINF(angle);

		float color_height=rounded_left_top_vertList[(xx+1)*2+1];
		rounded_left_top_colorList[(xx+1)*4+0]=rgb_top[0]+rgb_delta[0]*(color_height/cy);
		rounded_left_top_colorList[(xx+1)*4+1]=rgb_top[1]+rgb_delta[1]*(color_height/cy);
		rounded_left_top_colorList[(xx+1)*4+2]=rgb_top[2]+rgb_delta[2]*(color_height/cy);
		rounded_left_top_colorList[(xx+1)*4+3]=rgb_top[3]+rgb_delta[3]*(color_height/cy);
		angle+=delta_angle;
	}

	//right top
	rounded_right_top_vertList[0]=cx-deltaHeight;
	rounded_right_top_vertList[1]=deltaHeight;
	rounded_right_top_colorList[0]=rgb_top[0]+rgb_delta[0]*(rounded_left_top_vertList[1]/cy);
	rounded_right_top_colorList[1]=rgb_top[1]+rgb_delta[1]*(rounded_left_top_vertList[1]/cy);
	rounded_right_top_colorList[2]=rgb_top[2]+rgb_delta[2]*(rounded_left_top_vertList[1]/cy);
	rounded_right_top_colorList[3]=rgb_top[3]+rgb_delta[3]*(rounded_left_top_vertList[1]/cy);

	angle=270;
	for(int xx=step;xx>=0;xx--)
	{
		rounded_right_top_vertList[(xx+1)*2+0]=rounded_right_top_vertList[0]+deltaHeight*gxMath::COSF(angle);
		rounded_right_top_vertList[(xx+1)*2+1]=rounded_right_top_vertList[1]+deltaHeight*gxMath::SINF(angle);

		float color_height=rounded_right_top_vertList[(xx+1)*2+1];
		rounded_right_top_colorList[(xx+1)*4+0]=rgb_top[0]+rgb_delta[0]*(color_height/cy);
		rounded_right_top_colorList[(xx+1)*4+1]=rgb_top[1]+rgb_delta[1]*(color_height/cy);
		rounded_right_top_colorList[(xx+1)*4+2]=rgb_top[2]+rgb_delta[2]*(color_height/cy);
		rounded_right_top_colorList[(xx+1)*4+3]=rgb_top[3]+rgb_delta[3]*(color_height/cy);
		angle+=delta_angle;
	}

	if(type==0)
	{
		//right bottom
		rounded_right_bottom_vertList[0]=cx-deltaHeight;
		rounded_right_bottom_vertList[1]=cy-deltaHeight;
		rounded_right_bottom_colorList[0]=rgb_top[0]+rgb_delta[0]*(rounded_right_bottom_vertList[1]/cy);
		rounded_right_bottom_colorList[1]=rgb_top[1]+rgb_delta[1]*(rounded_right_bottom_vertList[1]/cy);
		rounded_right_bottom_colorList[2]=rgb_top[2]+rgb_delta[2]*(rounded_right_bottom_vertList[1]/cy);
		rounded_right_bottom_colorList[3]=rgb_top[3]+rgb_delta[3]*(rounded_right_bottom_vertList[1]/cy);

		angle=0;
		for(int xx=step;xx>=0;xx--)
		{
			rounded_right_bottom_vertList[(xx+1)*2+0]=rounded_right_bottom_vertList[0]+deltaHeight*gxMath::COSF(angle);
			rounded_right_bottom_vertList[(xx+1)*2+1]=rounded_right_bottom_vertList[1]+deltaHeight*gxMath::SINF(angle);

			float color_height=rounded_right_bottom_vertList[(xx+1)*2+1];
			rounded_right_bottom_colorList[(xx+1)*4+0]=rgb_top[0]+rgb_delta[0]*(color_height/cy);
			rounded_right_bottom_colorList[(xx+1)*4+1]=rgb_top[1]+rgb_delta[1]*(color_height/cy);
			rounded_right_bottom_colorList[(xx+1)*4+2]=rgb_top[2]+rgb_delta[2]*(color_height/cy);
			rounded_right_bottom_colorList[(xx+1)*4+3]=rgb_top[3]+rgb_delta[3]*(color_height/cy);
			angle+=delta_angle;
		}

		//left bottom
		rounded_left_bottom_vertList[0]=deltaHeight;
		rounded_left_bottom_vertList[1]=cy-deltaHeight;
		rounded_left_bottom_colorList[0]=rgb_top[0]+rgb_delta[0]*(rounded_left_bottom_vertList[1]/cy);
		rounded_left_bottom_colorList[1]=rgb_top[1]+rgb_delta[1]*(rounded_left_bottom_vertList[1]/cy);
		rounded_left_bottom_colorList[2]=rgb_top[2]+rgb_delta[2]*(rounded_left_bottom_vertList[1]/cy);
		rounded_left_bottom_colorList[3]=rgb_top[3]+rgb_delta[3]*(rounded_left_bottom_vertList[1]/cy);

		angle=90;
		for(int xx=step;xx>=0;xx--)
		{
			rounded_left_bottom_vertList[(xx+1)*2+0]=rounded_left_bottom_vertList[0]+deltaHeight*gxMath::COSF(angle);
			rounded_left_bottom_vertList[(xx+1)*2+1]=rounded_left_bottom_vertList[1]+deltaHeight*gxMath::SINF(angle);

			float color_height=rounded_left_bottom_vertList[(xx+1)*2+1];
			rounded_left_bottom_colorList[(xx+1)*4+0]=rgb_top[0]+rgb_delta[0]*(color_height/cy);
			rounded_left_bottom_colorList[(xx+1)*4+1]=rgb_top[1]+rgb_delta[1]*(color_height/cy);
			rounded_left_bottom_colorList[(xx+1)*4+2]=rgb_top[2]+rgb_delta[2]*(color_height/cy);
			rounded_left_bottom_colorList[(xx+1)*4+3]=rgb_top[3]+rgb_delta[3]*(color_height/cy);
			angle+=delta_angle;
		}
	}

	glPushMatrix();
	glTranslatef(x, y, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	//draw the horizontal rect

	glVertexPointer(2, GL_FLOAT, 0, vertical_vertLst);
	glColorPointer(4, GL_FLOAT, 0, vertical_colorLst);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glVertexPointer(2, GL_FLOAT, 0, horizontal_vertLst);
	glColorPointer(4, GL_FLOAT, 0, horizontal_colorLst);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glVertexPointer(2, GL_FLOAT, 0, rounded_left_top_vertList);
	glColorPointer(4, GL_FLOAT, 0, rounded_left_top_colorList);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+step));

	glVertexPointer(2, GL_FLOAT, 0, rounded_right_top_vertList);
	glColorPointer(4, GL_FLOAT, 0, rounded_right_top_colorList);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (2+step));

	if(type==0)
	{
		glVertexPointer(2, GL_FLOAT, 0, rounded_right_bottom_vertList);
		glColorPointer(4, GL_FLOAT, 0, rounded_right_bottom_colorList);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (2+step));

		glVertexPointer(2, GL_FLOAT, 0, rounded_left_bottom_vertList);
		glColorPointer(4, GL_FLOAT, 0, rounded_left_bottom_colorList);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (2+step));
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}