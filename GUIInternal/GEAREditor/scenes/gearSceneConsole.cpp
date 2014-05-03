#include "gearSceneConsole.h"
#include "../core/Timer.h"

gearSceneConsole::gearSceneConsole():
geWindow("Console View")
{
	m_pCurrentBuildRootNodePtr=NULL;
	m_pClearBtn=NULL;
	m_pClearAllBtn=NULL;
	m_pDontLogBtn=NULL;
	m_uCurrentRunElapsedTime=0;
}

gearSceneConsole::~gearSceneConsole()
{
}

void gearSceneConsole::onCreate()
{
	m_cConsoleTreeView.create(m_pRenderer, this, "AssetsFileTV", this);

	//clear btn
	m_pClearBtn=new geToolBarButton(m_pRenderer, "Clear", getToolBar());
	m_pClearBtn->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pClearBtn);

	//clear all btn
	m_pClearAllBtn=new geToolBarButton(m_pRenderer, "Clear All", getToolBar());
	m_pClearAllBtn->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pClearAllBtn);

	//don't log all btn
	m_pDontLogBtn=new geToolBarButton(m_pRenderer, "Don't Log", getToolBar());
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

	m_cConsoleTreeView.draw();
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
	sprintf(buffer, "#%d Run (0)", m_cConsoleTreeView.getRoot()->getTVNodeChildCount());

	m_cConsoleTreeView.getRoot()->openNode();
	for(std::vector<geGUIBase*>::iterator it = m_cConsoleTreeView.getRoot()->getChildControls()->begin(); it != m_cConsoleTreeView.getRoot()->getChildControls()->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			geTreeNode* child=(geTreeNode*)tvnode;
			child->closeNode();
		}
	}

	m_pCurrentBuildRootNodePtr = new geTreeNode(m_pRenderer, m_cConsoleTreeView.getRoot(), buffer, NULL);
	m_pCurrentBuildRootNodePtr->traverseSetWidth(m_cSize.x);
	m_cConsoleTreeView.refreshTreeView();

	//reset the timer
	m_uCurrentRunElapsedTime=Timer::getCurrentTimeInMilliSec();
}

void gearSceneConsole::appendConsoleMsg(const char* msg)
{
	if(m_pDontLogBtn->isButtonPressed())
		return;

	geTreeNode* newtvConsoleNode;

	if(m_pCurrentBuildRootNodePtr)
	{
		//
		unsigned long now=Timer::getCurrentTimeInMilliSec()-m_uCurrentRunElapsedTime;
		char msg_buffer[256];
		sprintf(msg_buffer, "(%d) %s", now, msg);
		//
		newtvConsoleNode = new geTreeNode(m_pRenderer, m_pCurrentBuildRootNodePtr, msg_buffer, NULL);
	}
	else
	{
		appendConsoleRunRootNode();
		//
		unsigned long now=Timer::getCurrentTimeInMilliSec()-m_uCurrentRunElapsedTime;
		char msg_buffer[256];
		sprintf(msg_buffer, "(%d) %s", now, msg);
		//
		newtvConsoleNode = new geTreeNode(m_pRenderer, m_pCurrentBuildRootNodePtr, msg_buffer, NULL);
	}

	char buffer[32];
	sprintf(buffer, "#%d Run (%d)", m_cConsoleTreeView.getRoot()->getTVNodeChildCount(), m_pCurrentBuildRootNodePtr->getTVNodeChildCount());
	m_pCurrentBuildRootNodePtr->setName(buffer);

	newtvConsoleNode->traverseSetWidth(m_cSize.x);
	//for performance reason we wont use refreshTreeView instead use quick_refreshTreeViewForOnlyVerticalScrollBar
	//m_cConsoleTreeView.refreshTreeView();
	if(m_pCurrentBuildRootNodePtr->isOpenNode())
	{
		m_cConsoleTreeView.quick_refreshTreeViewForOnlyVerticalScrollBar(newtvConsoleNode->getSize().y);
	}
}

void gearSceneConsole::clearConsoleCurrentRun()
{
	if(m_pCurrentBuildRootNodePtr)
	{
		m_pCurrentBuildRootNodePtr->destroyAllTVChilds();
		m_cConsoleTreeView.resetSelectedNodePtr();
		m_cConsoleTreeView.refreshTreeView();
		char buffer[32];
		sprintf(buffer, "#%d Run (%d)", m_cConsoleTreeView.getRoot()->getTVNodeChildCount(), m_pCurrentBuildRootNodePtr->getTVNodeChildCount());
		m_pCurrentBuildRootNodePtr->setName(buffer);
		//reset the timer
		m_uCurrentRunElapsedTime=Timer::getCurrentTimeInMilliSec();
	}
}

void gearSceneConsole::clearConsoleAllRun()
{
	m_cConsoleTreeView.getRoot()->destroyAllTVChilds();
	m_pCurrentBuildRootNodePtr=NULL;
	m_cConsoleTreeView.resetSelectedNodePtr();
	m_cConsoleTreeView.refreshTreeView();
}

void gearSceneConsole::onButtonClicked(geGUIBase* btn)
{
	if(m_pClearBtn==btn)
	{
		clearConsoleCurrentRun();
		m_pClearBtn->buttonNormal();
	}
	else if(m_pClearAllBtn==btn)
	{
		clearConsoleAllRun();
		m_pClearAllBtn->buttonNormal();
	}
}