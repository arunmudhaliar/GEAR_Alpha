#include "geColorDlg.h"
#include "../EditorApp.h"

geColorDlg::geColorDlg():
	geSecondryView("Color Dialog")
{
	setSize(geVector2f(200.0f, 320.0f));
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

	m_pWindow = new geWindow("Color Dialog");
	m_pWindow->create(m_pSecondryRenderer, NULL, 0, 0, m_cSize.x, m_cSize.y, false);
	m_cLayoutManager.getRootLayout()->createAsParent(m_pWindow);
	//m_pRootLayout->createAsParent(EditorApp::getSceneWorldEditor());

	for(int x=0;x<3;x++)
	{
		m_pHorizontalSlider_RGBA[x] = new geHorizontalSlider();
		m_pHorizontalSlider_RGBA[x]->create(m_pSecondryRenderer, m_pWindow, "slider", 10, (m_cSize.y-140)+x*15, 120.0f);
		m_pHorizontalSlider_RGBA[x]->setSliderValue(0.0f);
		m_pHorizontalSlider_RGBA[x]->setGUIObserver(this);
	}
	
	m_pHorizontalSlider_RGBA[3] = new geHorizontalSlider();
	m_pHorizontalSlider_RGBA[3]->create(m_pSecondryRenderer, m_pWindow, "slider", 10, (m_cSize.y-140)+3*15+10, 120.0f);
	m_pHorizontalSlider_RGBA[3]->setSliderValue(1.0f);
	m_pHorizontalSlider_RGBA[3]->setGUIObserver(this);

	m_pColorControl = new geColorControl();
	m_pColorControl->create(m_pSecondryRenderer, m_pWindow, m_cSize.x-50.0f, m_fCircleRadius*2.0f);
	m_pColorControl->setControlColor(0.0f, 0.0f, 0.0f, 1.0f);

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
	float cntr=0;
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

	//pointer
	glColor3f(0.0f, 0.0f, 0.0f);
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
	geGUIManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geGUIManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+0*15, m_cSize.x);
	sprintf(buffer, "G : %d", (int)(m_pHorizontalSlider_RGBA[1]->getSliderValue()*255));
	geGUIManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geGUIManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+1*15, m_cSize.x);
	sprintf(buffer, "B : %d", (int)(m_pHorizontalSlider_RGBA[2]->getSliderValue()*255));
	geGUIManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geGUIManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+2*15, m_cSize.x);
	sprintf(buffer, "A : %d", (int)(m_pHorizontalSlider_RGBA[3]->getSliderValue()*255));
	geGUIManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geGUIManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+3*15+10, m_cSize.x);
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

			m_pColorControl->setControlColor(res.x, res.y, res.z, 1.0f);

			m_pHorizontalSlider_RGBA[0]->setSliderValue(res.x, false);
			m_pHorizontalSlider_RGBA[1]->setSliderValue(res.y, false);
			m_pHorizontalSlider_RGBA[2]->setSliderValue(res.z, false);
		}
	}

	return geSecondryView::onMouseMove(x, y, flag);
}

void geColorDlg::onSliderChange(geGUIBase* slider)
{
	geVector3f rgb(m_pHorizontalSlider_RGBA[0]->getSliderValue(), m_pHorizontalSlider_RGBA[1]->getSliderValue(), m_pHorizontalSlider_RGBA[2]->getSliderValue());
	m_pColorControl->setControlColor(rgb.x, rgb.y, rgb.z, 1.0f);

	//rgb to xyz
	matrix4x4f xyzTM;
	vector3f xaxis(cos(DEG2RAD(60.0f)), sin(DEG2RAD(60.0f)), 0.0f);
	xyzTM.setXAxis(xaxis);
	vector3f yaxis(cos(DEG2RAD(180.0f)), sin(DEG2RAD(180.0f)), 0.0f);
	xyzTM.setYAxis(yaxis);
	vector3f zaxis(cos(DEG2RAD(300.0f)), sin(DEG2RAD(300.0f)), 0.0f);
	xyzTM.setZAxis(zaxis);

	vector3f res(xyzTM*vector3f(rgb.x, rgb.y, rgb.z));
	m_cPointerPos.set(m_fCircleRadius*res.x, m_fCircleRadius*res.y);
}