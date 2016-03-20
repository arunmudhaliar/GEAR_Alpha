#include "geColorDlg.h"
#include "../EditorApp.h"

geColorDlg::geColorDlg(geColorControl* pObserverControlPtr, geFontManager* fontManager, rendererGL10* mainRenderer):
	geSecondryView("Color Dialog", fontManager, mainRenderer)
{
	observerControlPtr=pObserverControlPtr;
    geVector2f tmp(200.0f, 320.0f);
	setSize(tmp);
	m_pWindow=NULL;
	colorDlgCircleRadius=1.0f;
}

geColorDlg::~geColorDlg()
{
	GE_DELETE(m_pWindow);
	GE_DELETE(rgbaHorizontalSliderArray[0]);
	GE_DELETE(rgbaHorizontalSliderArray[1]);
	GE_DELETE(rgbaHorizontalSliderArray[2]);
	GE_DELETE(rgbaHorizontalSliderArray[3]);
	GE_DELETE(colorControl);
}

void geColorDlg::onCreate(float cx, float cy)
{
	colorDlgCircleRadius=75.0f;

	m_pWindow = new geWindow("Color Dialog", fontManager);
	m_pWindow->create(secondryRenderer, NULL, 0, 0, m_cSize.x, m_cSize.y, false);
	auto layout = layoutManager->getRootLayout()->createLeft(m_pWindow, 1.0f);
    layout->unPinLayout();
	//rootLayout->createAsParent(EditorGEARApp::getSceneWorldEditor());

	float rgba[4]={1.0f, 1.0f, 1.0f, 1.0f};
	if(observerControlPtr)
	{
		rgba[0]=observerControlPtr->getControlColor().x;
		rgba[1]=observerControlPtr->getControlColor().y;
		rgba[2]=observerControlPtr->getControlColor().z;
		rgba[3]=observerControlPtr->getControlColor().w;
	}

	for(int x=0;x<3;x++)
	{
		rgbaHorizontalSliderArray[x] = new geHorizontalSlider(fontManager);
		rgbaHorizontalSliderArray[x]->create(secondryRenderer, m_pWindow, 10, (m_cSize.y-140)+x*15, 120.0f);
		rgbaHorizontalSliderArray[x]->setSliderValue(rgba[x]);
		rgbaHorizontalSliderArray[x]->setGUIObserver(this);
	}
	
    selectedColor.x = rgba[0];
    selectedColor.y = rgba[1];
    selectedColor.z = rgba[2];
    selectedColor.w = MAX(selectedColor.x, MAX(selectedColor.y, selectedColor.z));
    
	rgbaHorizontalSliderArray[3] = new geHorizontalSlider(fontManager);
	rgbaHorizontalSliderArray[3]->create(secondryRenderer, m_pWindow, 10, (m_cSize.y-140)+3*15+10, 120.0f);
	rgbaHorizontalSliderArray[3]->setSliderValue(selectedColor.w);
	rgbaHorizontalSliderArray[3]->setGUIObserver(this);

	colorControl = new geColorControl(fontManager);
	colorControl->create(secondryRenderer, m_pWindow, m_cSize.x-50.0f, colorDlgCircleRadius*2.0f);
	colorControl->setControlColor(rgba[0], rgba[1], rgba[2], rgba[3]);

	//vertices
	for(int xx=0;xx<COLOR_DLG_MAX_RESOLUTION-1;xx++)
	{
		float csx=colorDlgCircleRadius*cos(DEG2RAD((float)xx*10));
		float snx=colorDlgCircleRadius*sin(DEG2RAD((float)xx*10));
		colorCircleVertexArray[COLOR_DLG_MAX_RESOLUTION-(xx+1)].set(csx, snx);
	}
	colorCircleVertexArray[0].set(0, 0);

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
				colorCircleColorArray[xx+1].set(1.0f-color*2.0f, 0.0f, 1.0f);
			else
				colorCircleColorArray[xx+1].set(0.0f, color*2.0f-1.0f, 1.0f);
		}
		else if(retval==1)
		{
			if(color<0.5f)
				colorCircleColorArray[xx+1].set(0.0f, 1.0f, 1.0f-color*2.0f);
			else
				colorCircleColorArray[xx+1].set(color*2.0f-1.0f, 1.0f, 0.0f);
		}
		else if(retval==2)
		{
			if(color<0.5f)
				colorCircleColorArray[xx+1].set(1.0f, 1.0f-color*2.0f, 0.0f);
			else
				colorCircleColorArray[xx+1].set(1.0f, 0.0f, color*2.0f-1.0f);
		}
		else if(retval==3)
			colorCircleColorArray[xx+1].set(colorCircleColorArray[1].x, colorCircleColorArray[1].y, colorCircleColorArray[1].z);
	}

	colorCircleColorArray[0].set(1.0f, 1.0f, 1.0f);
	//

	//pointer
	int multiplier=360/12;
	for(int xx=12-1;xx>=0;xx--)
	{
		float csx=4.0f*cos(DEG2RAD((float)xx*multiplier));
		float snx=4.0f*sin(DEG2RAD((float)xx*multiplier));
		colorPointerVertexArray[xx].set(csx, snx);
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
	glTranslatef(sz.x*0.5f, colorDlgCircleRadius+15.0f, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &colorCircleVertexArray[0].x);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, &colorCircleColorArray[0].x);
	glDrawArrays(GL_TRIANGLE_FAN, 0, COLOR_DLG_MAX_RESOLUTION);
	glDisableClientState(GL_COLOR_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_COLOR_MATERIAL);
	float luminance=rgbaHorizontalSliderArray[3]->getSliderValue();
	glColor4f(0, 0, 0, 1.0f-luminance);
	glDrawArrays(GL_TRIANGLE_FAN, 0, COLOR_DLG_MAX_RESOLUTION);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);

	//pointer
    float pointer_clr = 1.0f-luminance;
	glColor3f(pointer_clr, pointer_clr, pointer_clr);
	glVertexPointer(2, GL_FLOAT, 0, &colorPointerVertexArray[0].x);

	glPushMatrix();
	glTranslatef(colorDlgPointerPosition.x, colorDlgPointerPosition.y, 0.0f);
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
	sprintf(buffer, "R : %d", (int)(rgbaHorizontalSliderArray[0]->getSliderValue()*255));
	geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+0*15, m_cSize.x);
	sprintf(buffer, "G : %d", (int)(rgbaHorizontalSliderArray[1]->getSliderValue()*255));
	geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+1*15, m_cSize.x);
	sprintf(buffer, "B : %d", (int)(rgbaHorizontalSliderArray[2]->getSliderValue()*255));
	geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 135, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight()+(m_cSize.y-140)+2*15, m_cSize.x);
	sprintf(buffer, "A : %d", (int)(rgbaHorizontalSliderArray[3]->getSliderValue()*255));
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
		vector3f mousePos(x-sz.x*0.5f, y-(colorDlgCircleRadius+15.0f+m_pWindow->getTopMarginOffsetHeight()), 0.0f);
		if(mousePos.length()<colorDlgCircleRadius)
		{
			colorDlgPointerPosition.set(mousePos.x, mousePos.y);
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

			colorControl->setControlColor(res.x, res.y, res.z, 1.0f);

			//rgbaHorizontalSliderArray[0]->setSliderValue((res.x+1.0f)*0.5f, false);
			//rgbaHorizontalSliderArray[1]->setSliderValue((res.y+1.0f)*0.5f, false);
			//rgbaHorizontalSliderArray[2]->setSliderValue((res.z+1.0f)*0.5f, false);


			rgbaHorizontalSliderArray[0]->setSliderValue(res.x, false);
			rgbaHorizontalSliderArray[1]->setSliderValue(res.y, false);
			rgbaHorizontalSliderArray[2]->setSliderValue(res.z, false);

			//float avg_rgb=(ABS(res.x)+ABS(res.y)+ABS(res.z))/3.0f;
			//if(avg_rgb>=1.0f)
			//	rgbaHorizontalSliderArray[3]->setSliderValue(1.0f, false);
			//else
			//	rgbaHorizontalSliderArray[3]->setSliderValue(avg_rgb, false);

			if(observerControlPtr)
				observerControlPtr->setControlColor(res.x, res.y, res.z, 1.0f);
		}
	}

	return geSecondryView::onMouseMove(x, y, flag);
}

void geColorDlg::onSliderChange(geGUIBase* slider)
{
    auto currentColor =  geVector4f(rgbaHorizontalSliderArray[0]->getSliderValue(), rgbaHorizontalSliderArray[1]->getSliderValue(), rgbaHorizontalSliderArray[2]->getSliderValue(), rgbaHorizontalSliderArray[3]->getSliderValue());
    
    auto colorDiff = currentColor - selectedColor;
    
    //Luminance has changed
    if(rgbaHorizontalSliderArray[3]->isGrabbed())
    {
        currentColor.x = selectedColor.x * currentColor.w;
        currentColor.y = selectedColor.y * currentColor.w;
        currentColor.z = selectedColor.z * currentColor.w;
        
        selectedColor.w = currentColor.w;
    }
    else if(    colorDiff.x != 0.0f
            ||  colorDiff.y != 0.0f
            ||  colorDiff.z != 0.0f)//Color has changed
    {
        currentColor.w = MAX(currentColor.x, MAX(currentColor.y, currentColor.z));
        
        selectedColor.x = currentColor.x / MAX(currentColor.w, 0.001f);
        selectedColor.y = currentColor.y / MAX(currentColor.w, 0.001f);
        selectedColor.z = currentColor.z / MAX(currentColor.w, 0.001f);
    }

	rgbaHorizontalSliderArray[0]->setSliderValue(currentColor.x, false);
	rgbaHorizontalSliderArray[1]->setSliderValue(currentColor.y, false);
	rgbaHorizontalSliderArray[2]->setSliderValue(currentColor.z, false);
	rgbaHorizontalSliderArray[3]->setSliderValue(currentColor.w, false);

	colorControl->setControlColor(currentColor.x, currentColor.y, currentColor.z, 1.0f);
	if(observerControlPtr)
		observerControlPtr->setControlColor(currentColor.x, currentColor.y, currentColor.z, 1.0f);

	//rgb to xyz
	matrix4x4f xyzTM;

	vector3f xaxis(cos(DEG2RAD(60.0f)), sin(DEG2RAD(60.0f)), 0.0f);
	xyzTM.setXAxis(xaxis);
	vector3f yaxis(cos(DEG2RAD(180.0f)), sin(DEG2RAD(180.0f)), 0.0f);
	xyzTM.setYAxis(yaxis);
	vector3f zaxis(cos(DEG2RAD(300.0f)), sin(DEG2RAD(300.0f)), 0.0f);
	xyzTM.setZAxis(zaxis);

	vector3f res(xyzTM*vector3f(currentColor.x, currentColor.y, currentColor.z));
	colorDlgPointerPosition.set(colorDlgCircleRadius*res.x, colorDlgCircleRadius*res.y);

}