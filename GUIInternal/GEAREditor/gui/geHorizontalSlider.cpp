#include "geHorizontalSlider.h"
#include "geGUIManager.h"

#define SLIDER_GRABBER_SZ	10.0f

geHorizontalSlider::geHorizontalSlider():
	geGUIBase(GEGUI_HORIZONTAL_SLIDER, "Horizontal Slider")
{
}

geHorizontalSlider::~geHorizontalSlider()
{
}

void geHorizontalSlider::create(geGUIBase* parent, const char* name, float x, float y, float cx)
{
	createBase(parent);

	setSize(cx, 5);
	setPos(x, y);

	STRCPY(m_szName, name);
	setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f, EGRADIENT_VERTICAL_DOWN, 0.4f);
	setColor(&m_cVBGrabberArea, 0.6f, 0.6f, 0.6f, 1.0f, EGRADIENT_VERTICAL_UP, 0.5f);

	m_fSliderPos=0.0f;
	m_bGrabbed=false;
	m_fMousePrevXPos=0.0f;
	setMouseBoundCheck(false);
}

void geHorizontalSlider::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&m_cVBClientArea);	
	drawLine(m_cVBClientAreaLine, 0.13f, 0.13f, 0.13f, 1.0f, 3, false);
	drawLine(&m_cVBClientAreaLine[4], 0.3f, 0.3f, 0.3f, 1.0f, 3, false);
	glTranslatef(m_fSliderPos*(m_cSize.x-SLIDER_GRABBER_SZ), 0, 0);
	drawRect(&m_cVBGrabberArea);

	glPopMatrix();
}
	
void geHorizontalSlider::onPosition(float x, float y, int flag)
{
}

void geHorizontalSlider::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx-SLIDER_GRABBER_SZ*0.5f,	0,
		SLIDER_GRABBER_SZ*0.5f,		0,
		cx-SLIDER_GRABBER_SZ*0.5f,	cy,
		SLIDER_GRABBER_SZ*0.5f,		cy,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	const float grabber_vertLst[8] =
	{
		SLIDER_GRABBER_SZ,	-2,
		0,					-2,
		SLIDER_GRABBER_SZ,	7,
		0,					7,
	};
	memcpy(m_cVBGrabberArea.m_cszVertexList, grabber_vertLst, sizeof(grabber_vertLst));

	const float clientarea_linevertLst[10] =
	{
		cx-SLIDER_GRABBER_SZ*0.5f,	0,
		SLIDER_GRABBER_SZ*0.5f,		0,
		SLIDER_GRABBER_SZ*0.5f,		cy-0.5f,
		cx-SLIDER_GRABBER_SZ*0.5f,	cy-0.5f,
		cx-SLIDER_GRABBER_SZ*0.5f,	0,
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

bool geHorizontalSlider::onMouseLButtonDown(float x, float y, int nFlag)
{
	float actualPos=m_cPos.x+m_fSliderPos*(m_cSize.x-SLIDER_GRABBER_SZ);

	if(x>actualPos && x<actualPos+SLIDER_GRABBER_SZ && y>m_cPos.y-2 && y<m_cPos.y+7)
	{
		m_bGrabbed=true;
		m_fMousePrevXPos=x;
		return true;
	}
	return false;
}

bool geHorizontalSlider::onMouseLButtonUp(float x, float y, int nFlag)
{
	m_bGrabbed=false;
	m_fMousePrevXPos=x;
	return true;
}

void geHorizontalSlider::onMouseMove(float x, float y, int flag)
{
	float diffX=x-m_fMousePrevXPos;
	if(m_bGrabbed && (flag&MK_LBUTTON))
	{
		float actualPos=m_cPos.x+m_fSliderPos*(m_cSize.x-SLIDER_GRABBER_SZ);
		actualPos+=diffX;
		m_fSliderPos=(actualPos-m_cPos.x)/(m_cSize.x-SLIDER_GRABBER_SZ);

		if(m_fSliderPos<=0.0f)
			m_fSliderPos=0.0f;
		if(m_fSliderPos>=1.0f)
			m_fSliderPos=1.0f;

		onSliderChange(m_fSliderPos);
	}
	m_fMousePrevXPos=x;
}

void geHorizontalSlider::setSliderValue(float value)
{
	if(value<0.0f || value>1.0f)
		return;

	m_fSliderPos=value;
	onSliderChange(m_fSliderPos);	
}

void geHorizontalSlider::onSliderChange(float sliderValue)
{
	setColor(&m_cVBClientArea, sliderValue, sliderValue, sliderValue, 1.0f, EGRADIENT_VERTICAL_DOWN, 0.4f);
}