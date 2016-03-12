#include "geAssetImportDlg.h"
#include "../EditorApp.h"

geAssetImportDlg::geAssetImportDlg(geFontManager* fontManager, rendererGL10* mainRenderer):
	geSecondryView("Asset Importer.", fontManager, mainRenderer)
{
    geVector2f tmp(600.0f, 140.0f);
	setSize(tmp);
	m_pWindow=NULL;
    setBuffer("");
    geVector2i pos(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    setPos(pos);
}

geAssetImportDlg::~geAssetImportDlg()
{
	GE_DELETE(m_pWindow);
}

void geAssetImportDlg::onCreate()
{
	m_pWindow = new geWindow("Asset Importer.", fontManager);
	m_pWindow->create(secondryRenderer, NULL, 0, 0, m_cSize.x, m_cSize.y, false);
	auto layout = layoutManager->getRootLayout()->createAsParent(m_pWindow);
    layout->unPinLayout();
	//rootLayout->createAsParent(EditorGEARApp::getSceneWorldEditor());
}

void geAssetImportDlg::onSize(float cx, float cy)
{

}

void geAssetImportDlg::onDraw()
{
	geVector2f sz=m_pWindow->getSize();

	std::vector<geGUIBase*>* childList=m_pWindow->getChildControls();
	for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	geFontManager::g_pFontArial10_80Ptr->drawString(m_cszBuffer, 10, m_pWindow->getTopMarginOffsetHeight()+geFontManager::g_pFontArial10_80Ptr->getLineHeight(), m_cSize.x);
}

void geAssetImportDlg::onDestroy()
{
}

void geAssetImportDlg::setBuffer(const char* assetPath)
{
    STRCPY(m_cszBuffer, assetPath);
}
