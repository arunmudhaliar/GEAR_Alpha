#include "geHorizontalSlider.h"
#include "geGUIManager.h"

#define SLIDER_GRABBER_SZ	10.0f

geHorizontalSlider::geHorizontalSlider(geFontManager* fontmanager):
	geGUIBase(GEGUI_HORIZONTAL_SLIDER, "Horizontal Slider", fontmanager)
{
}

geHorizontalSlider::~geHorizontalSlider()
{
}

void geHorizontalSlider::create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float cx)
{
	createBase(renderer, parent);

	setSize(cx, 5);
	setPos(x, y);

	STRCPY(m_szName, name);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.4f);
	setColor(&vertexBufferGrabberArea, 0.6f, 0.6f, 0.6f, 1.0f, EGRADIENT_VERTICAL_UP, 0.5f);

	sliderValue=0.0f;
	is_Grabbed=false;
	mousePrevXPos=0.0f;
	setMouseBoundCheck(false);
	setRange(0.0f, 1.0f);
}

void geHorizontalSlider::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&vertexBufferClientArea);	
	drawLine(vertexBufferClientAreaArray, 0.13f, 0.13f, 0.13f, 1.0f, 3, false);
	drawLine(&vertexBufferClientAreaArray[4], 0.3f, 0.3f, 0.3f, 1.0f, 3, false);
	glTranslatef(sliderValue*(m_cSize.x-SLIDER_GRABBER_SZ), 0, 0);
	drawRect(&vertexBufferGrabberArea);

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
	memcpy(vertexBufferClientArea.vertexArray, title_vertLst, sizeof(title_vertLst));

	const float grabber_vertLst[8] =
	{
		SLIDER_GRABBER_SZ,	-2,
		0,					-2,
		SLIDER_GRABBER_SZ,	7,
		0,					7,
	};
	memcpy(vertexBufferGrabberArea.vertexArray, grabber_vertLst, sizeof(grabber_vertLst));

	const float clientarea_linevertLst[10] =
	{
		cx-SLIDER_GRABBER_SZ*0.5f,	0,
		SLIDER_GRABBER_SZ*0.5f,		0,
		SLIDER_GRABBER_SZ*0.5f,		cy-0.5f,
		cx-SLIDER_GRABBER_SZ*0.5f,	cy-0.5f,
		cx-SLIDER_GRABBER_SZ*0.5f,	0,
	};
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

bool geHorizontalSlider::onMouseLButtonDown(float x, float y, int nFlag)
{
	float actualPos=/*m_cPos.x+*/sliderValue*(m_cSize.x-SLIDER_GRABBER_SZ);

	if(x>actualPos && x<actualPos+SLIDER_GRABBER_SZ && y>-2 && y<7)
	{
		is_Grabbed=true;
		mousePrevXPos=x;
		return true;
	}
	return false;
}

bool geHorizontalSlider::onMouseLButtonUp(float x, float y, int nFlag)
{
	is_Grabbed=false;
	mousePrevXPos=x;
	return true;
}

bool geHorizontalSlider::onMouseMove(float x, float y, int flag)
{
	float diffX=x-mousePrevXPos;
	if(is_Grabbed && (flag&MK_LBUTTON))
	{
		float actualPos=m_cPos.x+sliderValue*(m_cSize.x-SLIDER_GRABBER_SZ);
		actualPos+=diffX;
		sliderValue=(actualPos-m_cPos.x)/(m_cSize.x-SLIDER_GRABBER_SZ);

		if(sliderValue<=0.0f)
			sliderValue=0.0f;
		if(sliderValue>=1.0f)
			sliderValue=1.0f;

		setSliderValue(sliderValue, true);
	}
	mousePrevXPos=x;

	return true;
}

void geHorizontalSlider::setSliderValue(float value, bool bCallObserver)
{
	if(value<0.0f || value>1.0f)
		return;

	sliderValue=value;
	setClientAreaPrimaryActiveForeColor(sliderValue, sliderValue, sliderValue, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.4f);
	if(bCallObserver)
		onSliderChange(sliderValue);
}

void geHorizontalSlider::setRange(float min, float max)
{
	minRange=min;
	maxRange=max;
}

float geHorizontalSlider::getSliderValueWithInRange()
{
	float diff=maxRange-minRange;
	return minRange+diff*sliderValue;
}

void geHorizontalSlider::setSliderValueWithInRange(float value)
{
	float diff=maxRange-minRange;
	sliderValue=(value-minRange)/diff;
}

void geHorizontalSlider::onSliderChange(float sliderValue)
{
	if(guiObserver)
		guiObserver->onSliderChange(this);
}