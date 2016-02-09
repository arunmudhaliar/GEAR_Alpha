#include "geColorDlg.h"
#include "../EditorApp.h"

geColorDlg::geColorDlg(geColorControl* pObserverControlPtr, geFontManager* fontManager, rendererGL10* mainRenderer):
	geSecondryView("Color Dialog", fontManager, mainRenderer)
{
	m_pObserverControlPtr=pObserverControlPtr;
    geVector2f tmp(200.0f, 320.0f);
	setSize(tmp);
	m_pWindow=NULL;
	m_fCircleRadius=1.0f;
}

geColorDlg::~geColorDlg()
{
	GE_DELETE(m_pWindow);
	GE_DELETE(m_pHorizontalSlider_RGBA[0]);
	GE_DELETE(m_pHorizontalSlider_RGBA[1]);
	GE_DELETE(m_pHorizontalSlider_RGBA[2]);
	GE_DELETE(m_pHorizontalSlider_RGBA[3]);
	GE_DELETE(m_pColorControl);
}

void geColorDlg::onCreate()
{
	m_fCircleRadius=75.0f;

	m_pWindow = new geWindow("Color Dialog", m_pFontManager);
	m_pWindow->create(m_pSecondryRenderer, NULL, 0, 0, m_cSize.x, m_cSize.y, false);
	layoutManager->getRootLayout()->createAsParent(m_pWindow);
	//rootLayout->createAsParent(EditorGEARApp::getSceneWorldEditor());

	float rgba[4]={1.0f, 1.0f, 1.0f, 1.0f};
	if(m_pObserverControlPtr)
	{
		rgba[0]=m_pObserverControlPtr->getControlColor().x;
		rgba[1]=m_pObserverControlPtr->getControlColor().y;
		rgba[2]=m_pObserverControlPtr->getControlColor().z;
		rgba[3]=m_pObserverControlPtr->getControlColor().w;
	}

	for(int x=0;x<3;x++)
	{
		m_pHorizontalSlider_RGBA[x] = new geHorizontalSlider(m_pFontManager);
		m_pHorizontalSlider_RGBA[x]->create(m_pSecondryRenderer, m_pWindow, "slider", 10, (m_cSize.y-140)+x*15, 120.0f);
		m_pHorizontalSlider_RGBA[x]->setSliderValue(rgba[x]);
		m_pHorizontalSlider_RGBA[x]->setGUIObserver(this);
	}
	
    m_selectedColor.x = rgba[0];
    m_selectedColor.y = rgba[1];
    m_selectedColor.z = rgba[2];
    m_selectedColor.w = MAX(m_selectedColor.x, MAX(m_selectedColor.y, m_selectedColor.z));
    
	m_pHorizontalSlider_RGBA[3] = new geHorizontalSlider(m_pFontManager);
	m_pHorizontalSlider_RGBA[3]->create(m_pSecondryRenderer, m_pWindow, "slider", 10, (m_cSize.y-140)+3*15+10, 120.0f);
	m_pHorizontalSlider_RGBA[3]->setSliderValue(m_selectedColor.w);
	m_pHorizontalSlider_RGBA[3]->setGUIObserver(this);

	m_pColorControl = new geColorControl(m_pFontManager);
	m_pColorControl->create(m_pSecondryRenderer, m_pWindow, m_cSize.x-50.0f, m_fCircleRadius*2.0f);
	m_pColorControl->setControlColor(rgba[0], rgba[1], rgba[2], rgba[3]);

	//vertices
	for(int xx=0;xx<COLOR_DLG_MAX_RESOLUTION-1;xx++)
	{
		float csx=m_fCircleRadius*cos(DEG2RAD((float)xx*10));
		float snx=m_fCircleRadius*sin(DEG2RAD((float)xx*10));
		m_pszColorCircleVertices[COLOR_DLG_MAX_RESOLUTION-(xx+1)].set(csx, snx);
	}
	m_pszColorCircleVertices[0].set(0, 0);

	//color
	int d3=(COLOR_DLG_MAX_RESOLUTION-1)/3;
//	float cntr=0;
	for(int xx=0;xx<COLOR_DLG_MAX_RESOLUTION-1;xx++)
	{
		int reminder=xx%(d3);
		int retval=xx/d3;

		float color=(float)reminder/d3;
		if(retval==0)
		{
			if(color<0.5f)
				m_pszColorCircleColor[xx+1].set(1.0f-color*2.0f, 0.0f, 1.0f);
			else
				m_pszColorCircleColor[xx+1].set(0.0f, color*2.0f-1.0f, 1.0f);
		}
		else if(retval==1)
		{
			if(color<0.5f)
				m_pszColorCircleColor[xx+1].set(0.0f, 1.0f, 1.0f-color*2.0f);
			else
				m_pszColorCircleColor[xx+1].set(color*2.0f-1.0f, 1.0f, 0.0f);
		}
		else if(retval==2)
		{
			if(color<0.5f)
				m_pszColorCircleColor[xx+1].set(1.0f, 1.0f-color*2.0f, 0.0f);
			else
				m_pszColorCircleColor[xx+1].set(1.0f, 0.0f, color*2.0f-1.0f);
		}
		else if(retval==3)
			m_pszColorCircleColor[xx+1].set(m_pszColorCircleColor[1].x, m_pszColorCircleColor[1].y, m_pszColorCircleColor[1].z);
	}

	m_pszColorCircleColor[0].set(1.0f, 1.0f, 1.0f);
	//

	//pointer
	int multiplier=360/12;
	for(int xx=12-1;xx>=0;xx--)
	{
		float csx=4.0f*cos(DEG2RAD((float)xx*multiplier));
		float snx=4.0f*sin(DEG2RAD((float)xx*multiplier));
		m_pszColorPointerVertices[xx].set(csx, snx);
	}
	//
}

void geColorDlg::onSize(float cx, float cy)
{

}

void geColorDlg::onDraw()
{
	geVector2f sz=m_pWindow->getSize();

	glPushMatrix();
	glTranslatef(sz.x*0.5f, m_fCircleRadius+15.0f, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &m_pszColorCircleVertices[0].x);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, &m_pszColorCircleColor[0].x);
	glDrawArrays(GL_TRIANGLE_FAN, 0, COLOR_DLG_MAX_RESOLUTION);
	glDisableClientState(GL_COLOR_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_COLOR_MATERIAL);
	float luminance=m_pHorizontalSlider_RGBA[3]->getSliderValue();
	glColor4f(0, 0, 0, 1.0f-luminance);
	glDrawArrays(GL_TRIANGLE_FAN, 0, COLOR_DLG_MAX_RESOLUTION);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);

	//pointer
    float pointer_clr = 1.0f-luminance;
	glColor3f(pointer_clr, pointer_clr, pointer_clr);
	glVertexPointer(2, GL_FLOAT, 0, &m_pszColorPointerVertices[0].x);

	glPushMatrix();
	glTranslatef(m_cPointerPos.x, m_cPointerPos.y, 0.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 12);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	std::vector<geGUIBase*>* childList=m_pWindow->getChildControls();
	for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	char buffer[32];
	sprintf(buffer, "R : %d", (int)(m_pHorizontalSlider_RGBA[0]->getSliderValue()*255));
	geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+0*15, m_cSize.x);
	sprintf(buffer, "G : %d", (int)(m_pHorizontalSlider_RGBA[1]->getSliderValue()*255));
	geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+1*15, m_cSize.x);
	sprintf(buffer, "B : %d", (int)(m_pHorizontalSlider_RGBA[2]->getSliderValue()*255));
	geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+2*15, m_cSize.x);
	sprintf(buffer, "A : %d", (int)(m_pHorizontalSlider_RGBA[3]->getSliderValue()*255));
	geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+3*15+10, m_cSize.x);
}

void geColorDlg::onDestroy()
{
}

bool geColorDlg::onMouseMove(float x, float y, int flag)
{
	if(flag&MK_LBUTTON)
	{
		geVector2f sz=m_pWindow->getSize();
		vector3f mousePos(x-sz.x*0.5f, y-(m_fCircleRadius+15.0f+m_pWindow->getTopMarginOffsetHeight()), 0.0f);
		if(mousePos.length()<m_fCircleRadius)
		{
			m_cPointerPos.set(mousePos.x, mousePos.y);
			mousePos.normalize();

			matrix4x4f colorTM;
			vector3f xaxis(cos(DEG2RAD(60.0f)), sin(DEG2RAD(60.0f)), 1.0f);
			colorTM.setXAxis(xaxis);
			vector3f yaxis(cos(DEG2RAD(180.0f)), sin(DEG2RAD(180.0f)), 1.0f);
			colorTM.setYAxis(yaxis);
			vector3f zaxis(cos(DEG2RAD(300.0f)), sin(DEG2RAD(300.0f)), 1.0f);
			colorTM.setZAxis(zaxis);
			vector3f res(colorTM.getInverse() *mousePos);

			//res.x=0.3359375+res.x;
			//res.y=0.3359375+res.y;
			//res.z=0.3359375+res.z;

			m_pColorControl->setControlColor(res.x, res.y, res.z, 1.0f);

			//m_pHorizontalSlider_RGBA[0]->setSliderValue((res.x+1.0f)*0.5f, false);
			//m_pHorizontalSlider_RGBA[1]->setSliderValue((res.y+1.0f)*0.5f, false);
			//m_pHorizontalSlider_RGBA[2]->setSliderValue((res.z+1.0f)*0.5f, false);


			m_pHorizontalSlider_RGBA[0]->setSliderValue(res.x, false);
			m_pHorizontalSlider_RGBA[1]->setSliderValue(res.y, false);
			m_pHorizontalSlider_RGBA[2]->setSliderValue(res.z, false);

			//float avg_rgb=(ABS(res.x)+ABS(res.y)+ABS(res.z))/3.0f;
			//if(avg_rgb>=1.0f)
			//	m_pHorizontalSlider_RGBA[3]->setSliderValue(1.0f, false);
			//else
			//	m_pHorizontalSlider_RGBA[3]->setSliderValue(avg_rgb, false);

			if(m_pObserverControlPtr)
				m_pObserverControlPtr->setControlColor(res.x, res.y, res.z, 1.0f);
		}
	}

	return geSecondryView::onMouseMove(x, y, flag);
}

void geColorDlg::onSliderChange(geGUIBase* slider)
{
    auto currentColor =  geVector4f(m_pHorizontalSlider_RGBA[0]->getSliderValue(), m_pHorizontalSlider_RGBA[1]->getSliderValue(), m_pHorizontalSlider_RGBA[2]->getSliderValue(), m_pHorizontalSlider_RGBA[3]->getSliderValue());
    
    auto colorDiff = currentColor - m_selectedColor;
    
    //Luminance has changed
    if(m_pHorizontalSlider_RGBA[3]->isGrabbed())
    {
        currentColor.x = m_selectedColor.x * currentColor.w;
        currentColor.y = m_selectedColor.y * currentColor.w;
        currentColor.z = m_selectedColor.z * currentColor.w;
        
        m_selectedColor.w = currentColor.w;
    }
    else if(    colorDiff.x != 0.0f
            ||  colorDiff.y != 0.0f
            ||  colorDiff.z != 0.0f)//Color has changed
    {
        currentColor.w = MAX(currentColor.x, MAX(currentColor.y, currentColor.z));
        
        m_selectedColor.x = currentColor.x / MAX(currentColor.w, 0.001f);
        m_selectedColor.y = currentColor.y / MAX(currentColor.w, 0.001f);
        m_selectedColor.z = currentColor.z / MAX(currentColor.w, 0.001f);
    }

	m_pHorizontalSlider_RGBA[0]->setSliderValue(currentColor.x, false);
	m_pHorizontalSlider_RGBA[1]->setSliderValue(currentColor.y, false);
	m_pHorizontalSlider_RGBA[2]->setSliderValue(currentColor.z, false);
	m_pHorizontalSlider_RGBA[3]->setSliderValue(currentColor.w, false);

	m_pColorControl->setControlColor(currentColor.x, currentColor.y, currentColor.z, 1.0f);
	if(m_pObserverControlPtr)
		m_pObserverControlPtr->setControlColor(currentColor.x, currentColor.y, currentColor.z, 1.0f);

	//rgb to xyz
	matrix4x4f xyzTM;

	vector3f xaxis(cos(DEG2RAD(60.0f)), sin(DEG2RAD(60.0f)), 0.0f);
	xyzTM.setXAxis(xaxis);
	vector3f yaxis(cos(DEG2RAD(180.0f)), sin(DEG2RAD(180.0f)), 0.0f);
	xyzTM.setYAxis(yaxis);
	vector3f zaxis(cos(DEG2RAD(300.0f)), sin(DEG2RAD(300.0f)), 0.0f);
	xyzTM.setZAxis(zaxis);

	vector3f res(xyzTM*vector3f(currentColor.x, currentColor.y, currentColor.z));
	m_cPointerPos.set(m_fCircleRadius*res.x, m_fCircleRadius*res.y);

}