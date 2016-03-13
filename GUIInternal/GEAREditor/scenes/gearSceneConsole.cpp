#include "gearSceneConsole.h"
//#include "../../../GEAREngine/src/core/Timer.h"
#include "../EditorApp.h"
#include "../gui/geGUIManager.h"

gearSceneConsole::gearSceneConsole(geFontManager* fontManager):
geWindow("Console View", fontManager)
{
	currentBuildRootTreeNode=NULL;
	toolBarClearButton=NULL;
	toolBarClearAllButton=NULL;
	toolBarDontLogButton=NULL;
	currentRunElapsedTime=0;
    consoleTreeView = new geTreeView(fontManager);
}

gearSceneConsole::~gearSceneConsole()
{
    GE_DELETE(consoleTreeView);
}

void gearSceneConsole::onCreate(float cx, float cy)
{
	spriteArray[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[0].setClip(110, 258, 16, 16);

	consoleTreeView->create(rendererGUI, this, "AssetsFileTV", this);

	//clear btn
	toolBarClearButton=new geToolBarButton(rendererGUI, "Clear", getToolBar(), fontManagerGUI);
	toolBarClearButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(toolBarClearButton);

	//clear all btn
	toolBarClearAllButton=new geToolBarButton(rendererGUI, "Clear All", getToolBar(), fontManagerGUI);
	toolBarClearAllButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(toolBarClearAllButton);

	//don't log all btn
	toolBarDontLogButton=new geToolBarButton(rendererGUI, "Don't Log", getToolBar(), fontManagerGUI);
	toolBarDontLogButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(toolBarDontLogButton);

	currentRunElapsedTime=Timer::getCurrentTimeInMilliSec();
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

	consoleTreeView->draw();
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

//==============================CONSOLE FUNC=========================
void gearSceneConsole::appendConsoleRunRootNode()
{
	//if(toolBarDontLogButton->isButtonPressed())
	//	return;

	char buffer[32];
	sprintf(buffer, "#%d Run (0)", consoleTreeView->getRoot()->getTVNodeChildCount());

	consoleTreeView->getRoot()->openNode();
	for(std::vector<geGUIBase*>::iterator it = consoleTreeView->getRoot()->getChildControls()->begin(); it != consoleTreeView->getRoot()->getChildControls()->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			geTreeNode* child=(geTreeNode*)tvnode;
			child->closeNode();
		}
	}

	currentBuildRootTreeNode = new geTreeNode(rendererGUI, consoleTreeView->getRoot(), buffer, &spriteArray[0], fontManagerGUI);
	currentBuildRootTreeNode->traverseSetWidth(m_cSize.x);
	consoleTreeView->refreshTreeView();

	//reset the timer
	currentRunElapsedTime=Timer::getCurrentTimeInMilliSec();
}

void gearSceneConsole::appendConsoleMsg(const char* msg, int msgtype)
{
	if(toolBarDontLogButton->isButtonPressed())
		return;

	geTreeNode* newtvConsoleNode;

	//
	unsigned long now=Timer::getCurrentTimeInMilliSec()-currentRunElapsedTime;
	char msg_buffer[256];
	switch (msgtype)
	{
	case 0:	//info
		sprintf(msg_buffer, "(%ld) info: %s", now, msg);
		break;
	case 1:	//warning
		sprintf(msg_buffer, "(%ld) warning: %s", now, msg);
		break;
	case 2:	//error
		sprintf(msg_buffer, "(%ld) ERROR: %s", now, msg);
		break;
	default:
		break;
	}
	//

	if(currentBuildRootTreeNode)
	{
		newtvConsoleNode = new geTreeNode(rendererGUI, currentBuildRootTreeNode, msg_buffer, NULL, fontManagerGUI);
	}
	else
	{
		appendConsoleRunRootNode();
		newtvConsoleNode = new geTreeNode(rendererGUI, currentBuildRootTreeNode, msg_buffer, NULL, fontManagerGUI);
	}

	char buffer[32];
	sprintf(buffer, "#%d Run (%d)", consoleTreeView->getRoot()->getTVNodeChildCount(), currentBuildRootTreeNode->getTVNodeChildCount());
	currentBuildRootTreeNode->setName(buffer);

	newtvConsoleNode->traverseSetWidth(m_cSize.x);
	//for performance reason we wont use refreshTreeView instead use quick_refreshTreeViewForOnlyVerticalScrollBar
	//consoleTreeView->refreshTreeView();
	if(currentBuildRootTreeNode->isOpenNode())
	{
		consoleTreeView->quick_refreshTreeViewForOnlyVerticalScrollBar(newtvConsoleNode->getSize().y);
	}
}

void gearSceneConsole::clearConsoleCurrentRun()
{
	if(currentBuildRootTreeNode)
	{
		currentBuildRootTreeNode->destroyAllTVChilds();
		consoleTreeView->resetSelectedNodePtr();
		consoleTreeView->refreshTreeView();
		char buffer[32];
		sprintf(buffer, "#%d Run (%d)", consoleTreeView->getRoot()->getTVNodeChildCount(), currentBuildRootTreeNode->getTVNodeChildCount());
		currentBuildRootTreeNode->setName(buffer);
		//reset the timer
		currentRunElapsedTime=Timer::getCurrentTimeInMilliSec();
	}
}

void gearSceneConsole::clearConsoleAllRun()
{
	consoleTreeView->getRoot()->destroyAllTVChilds();
	currentBuildRootTreeNode=NULL;
	consoleTreeView->resetSelectedNodePtr();
	consoleTreeView->refreshTreeView();
}

void gearSceneConsole::onButtonClicked(geGUIBase* btn)
{
	if(toolBarClearButton==btn)
	{
		clearConsoleCurrentRun();
		toolBarClearButton->buttonNormal(true);
	}
	else if(toolBarClearAllButton==btn)
	{
		clearConsoleAllRun();
		toolBarClearAllButton->buttonNormal(true);
	}
}