#include "gearSceneSettings.h"

gearSceneSettings::gearSceneSettings(geFontManager* fontmanager):
geWindow("Settings View", fontmanager)
{
	settingsAndroidParentTreeNode=NULL;
	androidSettings=NULL;

	settingsGlobalParentTreeNode=NULL;
	globalSettings=NULL;
    
    settingsTreeView = new geTreeView(fontmanager);
}

gearSceneSettings::~gearSceneSettings()
{
    GE_DELETE(settingsTreeView);
}

void gearSceneSettings::onCreate(float cx, float cy)
{
	spriteArray[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[0].setClip(6, 256, 16, 16);
	spriteArray[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[1].setClip(173, 321, 16, 16);

	settingsTreeView->create(rendererGUI, this, "SettingsTV", this);

	geTreeNode* rootNode=settingsTreeView->getRoot();

	settingsGlobalParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Global", &spriteArray[0], 0);
	globalSettings = new geSettingsGlobal(rendererGUI, settingsGlobalParentTreeNode, "", NULL, fontManagerGUI);

	settingsAndroidParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Android", &spriteArray[1], 0);
	androidSettings = new geSettingsAndroid(rendererGUI, settingsAndroidParentTreeNode, "", NULL, fontManagerGUI);
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

	settingsTreeView->draw();
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