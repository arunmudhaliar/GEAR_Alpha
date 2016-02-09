#include "gearSceneConsole.h"
//#include "../../../GEAREngine/src/core/Timer.h"
#include "../EditorApp.h"
#include "../gui/geGUIManager.h"

gearSceneConsole::gearSceneConsole(geFontManager* fontManager):
geWindow("Console View", fontManager)
{
	m_pCurrentBuildRootNodePtr=NULL;
	m_pClearBtn=NULL;
	m_pClearAllBtn=NULL;
	m_pDontLogBtn=NULL;
	m_uCurrentRunElapsedTime=0;
    m_pConsoleTreeView = new geTreeView(fontManager);
}

gearSceneConsole::~gearSceneConsole()
{
    GE_DELETE(m_pConsoleTreeView);
}

void gearSceneConsole::onCreate()
{
	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(110, 258, 16, 16);

	m_pConsoleTreeView->create(rendererGUI, this, "AssetsFileTV", this);

	//clear btn
	m_pClearBtn=new geToolBarButton(rendererGUI, "Clear", getToolBar(), fontManagerGUI);
	m_pClearBtn->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pClearBtn);

	//clear all btn
	m_pClearAllBtn=new geToolBarButton(rendererGUI, "Clear All", getToolBar(), fontManagerGUI);
	m_pClearAllBtn->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pClearAllBtn);

	//don't log all btn
	m_pDontLogBtn=new geToolBarButton(rendererGUI, "Don't Log", getToolBar(), fontManagerGUI);
	m_pDontLogBtn->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pDontLogBtn);

	m_uCurrentRunElapsedTime=Timer::getCurrentTimeInMilliSec();
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

	m_pConsoleTreeView->draw();
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
	//if(m_pDontLogBtn->isButtonPressed())
	//	return;

	char buffer[32];
	sprintf(buffer, "#%d Run (0)", m_pConsoleTreeView->getRoot()->getTVNodeChildCount());

	m_pConsoleTreeView->getRoot()->openNode();
	for(std::vector<geGUIBase*>::iterator it = m_pConsoleTreeView->getRoot()->getChildControls()->begin(); it != m_pConsoleTreeView->getRoot()->getChildControls()->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			geTreeNode* child=(geTreeNode*)tvnode;
			child->closeNode();
		}
	}

	m_pCurrentBuildRootNodePtr = new geTreeNode(rendererGUI, m_pConsoleTreeView->getRoot(), buffer, &m_cszSprites[0], fontManagerGUI);
	m_pCurrentBuildRootNodePtr->traverseSetWidth(m_cSize.x);
	m_pConsoleTreeView->refreshTreeView();

	//reset the timer
	m_uCurrentRunElapsedTime=Timer::getCurrentTimeInMilliSec();
}

void gearSceneConsole::appendConsoleMsg(const char* msg, int msgtype)
{
	if(m_pDontLogBtn->isButtonPressed())
		return;

	geTreeNode* newtvConsoleNode;

	//
	unsigned long now=Timer::getCurrentTimeInMilliSec()-m_uCurrentRunElapsedTime;
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

	if(m_pCurrentBuildRootNodePtr)
	{
		newtvConsoleNode = new geTreeNode(rendererGUI, m_pCurrentBuildRootNodePtr, msg_buffer, NULL, fontManagerGUI);
	}
	else
	{
		appendConsoleRunRootNode();
		newtvConsoleNode = new geTreeNode(rendererGUI, m_pCurrentBuildRootNodePtr, msg_buffer, NULL, fontManagerGUI);
	}

	char buffer[32];
	sprintf(buffer, "#%d Run (%d)", m_pConsoleTreeView->getRoot()->getTVNodeChildCount(), m_pCurrentBuildRootNodePtr->getTVNodeChildCount());
	m_pCurrentBuildRootNodePtr->setName(buffer);

	newtvConsoleNode->traverseSetWidth(m_cSize.x);
	//for performance reason we wont use refreshTreeView instead use quick_refreshTreeViewForOnlyVerticalScrollBar
	//m_pConsoleTreeView->refreshTreeView();
	if(m_pCurrentBuildRootNodePtr->isOpenNode())
	{
		m_pConsoleTreeView->quick_refreshTreeViewForOnlyVerticalScrollBar(newtvConsoleNode->getSize().y);
	}
}

void gearSceneConsole::clearConsoleCurrentRun()
{
	if(m_pCurrentBuildRootNodePtr)
	{
		m_pCurrentBuildRootNodePtr->destroyAllTVChilds();
		m_pConsoleTreeView->resetSelectedNodePtr();
		m_pConsoleTreeView->refreshTreeView();
		char buffer[32];
		sprintf(buffer, "#%d Run (%d)", m_pConsoleTreeView->getRoot()->getTVNodeChildCount(), m_pCurrentBuildRootNodePtr->getTVNodeChildCount());
		m_pCurrentBuildRootNodePtr->setName(buffer);
		//reset the timer
		m_uCurrentRunElapsedTime=Timer::getCurrentTimeInMilliSec();
	}
}

void gearSceneConsole::clearConsoleAllRun()
{
	m_pConsoleTreeView->getRoot()->destroyAllTVChilds();
	m_pCurrentBuildRootNodePtr=NULL;
	m_pConsoleTreeView->resetSelectedNodePtr();
	m_pConsoleTreeView->refreshTreeView();
}

void gearSceneConsole::onButtonClicked(geGUIBase* btn)
{
	if(m_pClearBtn==btn)
	{
		clearConsoleCurrentRun();
		m_pClearBtn->buttonNormal(true);
	}
	else if(m_pClearAllBtn==btn)
	{
		clearConsoleAllRun();
		m_pClearAllBtn->buttonNormal(true);
	}
}