#include "gearSceneConsole.h"

gearSceneConsole::gearSceneConsole():
geWindow("Console View")
{
}

gearSceneConsole::~gearSceneConsole()
{
}

void gearSceneConsole::onCreate()
{
	m_cFileTreeView.create(m_pRenderer, this, "AssetsFileTV", this);

}

void gearSceneConsole::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	m_cFileTreeView.draw();
}

void gearSceneConsole::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
}

bool gearSceneConsole::onMouseLButtonDown(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonDown(x, y, nFlag);
}

bool gearSceneConsole::onMouseLButtonUp(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonUp(x, y, nFlag);
}

bool gearSceneConsole::onMouseMove(float x, float y, int flag)
{
	return geWindow::onMouseMove(x, y, flag);
}

void gearSceneConsole::onMouseWheel(int zDelta, int x, int y, int flag)
{
	geWindow::onMouseWheel(zDelta, x, y, flag);
}