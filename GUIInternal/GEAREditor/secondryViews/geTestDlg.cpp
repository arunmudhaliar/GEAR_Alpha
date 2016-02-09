#include "geTestDlg.h"
#include "../EditorApp.h"

geTestDlg::geTestDlg(geFontManager* fontManager, rendererGL10* mainRenderer):
	geSecondryView("Test Dialog", fontManager, mainRenderer)
{
    geVector2f tmp(200.0f, 320.0f);
	setSize(tmp);
	m_pWindow=NULL;
}

geTestDlg::~geTestDlg()
{
	GE_DELETE(m_pWindow);
}

void geTestDlg::onCreate()
{
	m_pWindow = new geWindow("Color Dialog", m_pFontManager);
	m_pWindow->create(m_pSecondryRenderer, NULL, 0, 0, m_cSize.x, m_cSize.y, false);
	layoutManager->getRootLayout()->createAsParent(m_pWindow);
	//rootLayout->createAsParent(EditorGEARApp::getSceneWorldEditor());
}

void geTestDlg::onSize(float cx, float cy)
{

}

void geTestDlg::onDraw()
{
	geVector2f sz=m_pWindow->getSize();

	std::vector<geGUIBase*>* childList=m_pWindow->getChildControls();
	for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	char buffer[32];
	strcpy(buffer, "Test Dialog");
	geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 10, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight(), m_cSize.x);
}

void geTestDlg::onDestroy()
{
}