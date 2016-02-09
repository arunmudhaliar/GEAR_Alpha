#include "gearSceneSettings.h"

gearSceneSettings::gearSceneSettings(geFontManager* fontmanager):
geWindow("Settings View", fontmanager)
{
	m_pSettingsAndroidParentNode=NULL;
	m_pSettingsAndroid=NULL;

	m_pSettingsGlobalParentNode=NULL;
	m_pSettingsGlobal=NULL;
    
    m_pSettingsTreeView = new geTreeView(fontmanager);
}

gearSceneSettings::~gearSceneSettings()
{
    GE_DELETE(m_pSettingsTreeView);
}

void gearSceneSettings::onCreate()
{
	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(6, 256, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(173, 321, 16, 16);

	m_pSettingsTreeView->create(rendererGUI, this, "SettingsTV", this);

	geTreeNode* rootNode=m_pSettingsTreeView->getRoot();

	m_pSettingsGlobalParentNode = new geTreeNode(rendererGUI, rootNode, "Global", &m_cszSprites[0], 0);
	m_pSettingsGlobal = new geSettingsGlobal(rendererGUI, m_pSettingsGlobalParentNode, "", NULL, fontManagerGUI);

	m_pSettingsAndroidParentNode = new geTreeNode(rendererGUI, rootNode, "Android", &m_cszSprites[1], 0);
	m_pSettingsAndroid = new geSettingsAndroid(rendererGUI, m_pSettingsAndroidParentNode, "", NULL, fontManagerGUI);
}

void gearSceneSettings::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	m_pSettingsTreeView->draw();
}

void gearSceneSettings::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
}

bool gearSceneSettings::onMouseLButtonDown(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonDown(x, y, nFlag);
}

bool gearSceneSettings::onMouseLButtonUp(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonUp(x, y, nFlag);
}

bool gearSceneSettings::onMouseMove(float x, float y, int flag)
{
	return geWindow::onMouseMove(x, y, flag);
}

void gearSceneSettings::onMouseWheel(int zDelta, int x, int y, int flag)
{
	geWindow::onMouseWheel(zDelta, x, y, flag);
}