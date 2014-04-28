#include "gearSceneSettings.h"

gearSceneSettings::gearSceneSettings():
geWindow("Settings View")
{
	m_pSettingsAndroidParentNode=NULL;
	m_pSettingsAndroid=NULL;
}

gearSceneSettings::~gearSceneSettings()
{
}

void gearSceneSettings::onCreate()
{
	m_cSettingsTreeView.create(m_pRenderer, this, "SettingsTV", this);

	geTreeNode* rootNode=m_cSettingsTreeView.getRoot();

	m_pSettingsAndroidParentNode = new geTreeNode(m_pRenderer, rootNode, "Android", NULL, 0);
	m_pSettingsAndroid = new geSettingsAndroid(m_pRenderer, m_pSettingsAndroidParentNode, "", NULL);

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

	m_cSettingsTreeView.draw();
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