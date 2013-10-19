#include "geColorDlg.h"
#include "../EditorApp.h"

geColorDlg::geColorDlg():
	geSecondryView("Color Dialog")
{
	m_pWindow=NULL;
}

geColorDlg::~geColorDlg()
{
	GE_DELETE(m_pWindow);
}

void geColorDlg::onCreate()
{
	m_pWindow = new geWindow("Color Dialog");
	m_pWindow->create(m_pSecondryRenderer, NULL, 0, 0, 100, 100, true);
	m_pRootLayout->createAsParent(m_pWindow);
	//m_pRootLayout->createAsParent(EditorApp::getSceneWorldEditor());
}

void geColorDlg::onSize(float cx, float cy)
{

}

void geColorDlg::onDraw()
{
}

void geColorDlg::onDestroy()
{
}