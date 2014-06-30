#include "geTreeView.h"
#include "geGUIManager.h"

geTreeNode::geTreeNode():
	geGUIBase(GEGUI_TREEVIEW_NODE, "TreeView Node")
{
	//no implementation
	m_pParentTreeView=NULL;
	m_pSprite=NULL;
}

geTreeNode::geTreeNode(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, float xoffset):
	geGUIBase(GEGUI_TREEVIEW_NODE, name)
{
	createBase(renderer, parent);

	m_bHaveAtleastOneTreeNodeChild=false;
	m_fXOffSet=xoffset;
	if(parent==NULL)
	{
		setPos(0, 0);
		m_fXOffSet=0;
	}
	else
	{
		m_fXOffSet=(parent->getParent()==NULL)?0:m_fXOffSet;
		setPos(m_fXOffSet, GE_TREEVIEWNODE_CY);
	}
	setSize(70, GE_TREEVIEWNODE_CY);

	setClientAreaPrimaryActiveForeColor(0.12f, 0.12f, 0.12f, 1.0f);
	applyPrimaryColorToVBClientArea();

	openNode();

	int h=GE_TREEVIEWNODE_CY-4;
	const float togglebutton_linevertLst[6*2] =
	{
		5,		(GE_TREEVIEWNODE_CY-4),
		5+8,	(GE_TREEVIEWNODE_CY-4)-(h>>2),
		5,		(GE_TREEVIEWNODE_CY-4)-(h>>1),

		5+4,	(GE_TREEVIEWNODE_CY-4),
		5+8,	(GE_TREEVIEWNODE_CY-4)-(h>>1),
		5,		(GE_TREEVIEWNODE_CY-4)-(h>>1),
	};
	memcpy(m_cVBLayoutToggleButtonLine, togglebutton_linevertLst, sizeof(togglebutton_linevertLst));

	m_pParentTreeView=NULL;
	m_pSprite=NULL;
	setNodeSprite(sprite);

	m_bSelected=false;

	setMouseBoundCheck(false);

	setNodeColor(0.12f, 0.12f, 0.12f);
	setNodeSelectionColor(0.24f, 0.38f, 0.57f);
}

geTreeNode::~geTreeNode()
{
	if(m_pSelectedControlPtr==this)
		m_pSelectedControlPtr=NULL;
	GE_DELETE(m_pSprite);
	destroyAllTVChilds();
}

void geTreeNode::destroyAllTVChilds()
{
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		GE_DELETE(tvnode);
	}
	m_vControls.clear();
}

void geTreeNode::setNodeSprite(Sprite2Dx* sprite)
{
	if(sprite)
	{
		GE_DELETE(m_pSprite);
		m_pSprite = new Sprite2Dx();
		m_pSprite->copy(*sprite);
		m_pSprite->setPos(5+11, 3);
	}
}

void geTreeNode::drawNode()
{
	if(m_bSelected)
	{
		drawRect(&m_cVBClientArea);
	}

	geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

	if(m_vControls.size() && m_bHaveAtleastOneTreeNodeChild)
	{
		if(m_bNodeOpen)
			drawTriangle(&m_cVBLayoutToggleButtonLine[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		else
			drawTriangle(&m_cVBLayoutToggleButtonLine[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
	}

	if(m_pSprite)
		m_pSprite->draw();
}

void geTreeNode::draw()
{
	if(m_bNodeOpen)
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
				continue;

			geTreeNode* node=(geTreeNode*)tvnode;
			glTranslatef(node->getXOffset(), 0, 0.0f);
			node->drawNode();
			glTranslatef(0, node->getSize().y, 0.0f);
			node->draw();
			glTranslatef(-node->getXOffset(), 0.0f, 0.0f);
		}
	}
}

void geTreeNode::onPosition(float x, float y, int flag)
{
}

void geTreeNode::onSize(float cx, float cy, int flag)
{
	const float clientarea_vertLst[8] =
	{
		cx,	2,
		0,	2,
		cx,	cy-1,
		0,	cy-1,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, clientarea_vertLst, sizeof(clientarea_vertLst));
}

geTreeNode* geTreeNode::getSelectedNode(int x, int y, int& xoff, int& yoff, bool& bClickedOnToggleButton)
{
	if(y>yoff && y<yoff+m_cSize.y)
	{
		if(x>xoff+4+m_fXOffSet && x<xoff+(5+9)+m_fXOffSet)
			bClickedOnToggleButton=true;
		return this;
	}

	yoff+=m_cSize.y;

	if(m_bNodeOpen)
	{
		xoff+=m_fXOffSet;
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
				continue;
			geTreeNode* selectedNode=((geTreeNode*)tvnode)->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);

			if(selectedNode)
				return selectedNode;
		}
		xoff-=m_fXOffSet;
	}

	return NULL;
}

void geTreeNode::getTotalHeightofAllNodes(int& height)
{
	if(m_bNodeOpen)
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
				continue;
			height+=tvnode->getSize().y;
			((geTreeNode*)tvnode)->getTotalHeightofAllNodes(height);
		}
	}
}

void geTreeNode::selectNode()
{
	setColor(&m_cVBClientArea, m_cNodeSelectionColor.x, m_cNodeSelectionColor.y, m_cNodeSelectionColor.z, 1.0f);
	m_bSelected=true;
}

void geTreeNode::unselectNode()
{
	setColor(&m_cVBClientArea, m_cNodeColor.x, m_cNodeColor.y, m_cNodeColor.z, 1.0f);
	m_bSelected=false;
}

void geTreeNode::traverseSetWidth(float cx)
{
	setSize(cx, m_cSize.y);
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
			continue;
		((geTreeNode*)tvnode)->traverseSetWidth(cx);
	}
}

void geTreeNode::deFocus()
{
	setColor(&m_cVBClientArea, 0.25f, 0.25f, 0.25f, 1.0f);
}


void geTreeNode::onAppendChild(geGUIBase* child)
{
	if(!m_bHaveAtleastOneTreeNodeChild)
		m_bHaveAtleastOneTreeNodeChild=(child->getGUIID()==GEGUI_TREEVIEW_NODE);
}

void geTreeNode::appnendTVChild(geTreeNode* child)
{
	child->setPos(child->getXOffset(), GE_TREEVIEWNODE_CY);

	child->setParent(this);
	traverseSetWidth(m_cSize.x);
}

void geTreeNode::removeTVChild(geTreeNode* child)
{
	if(m_vControls.size()==0)
		return;

	m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), child), m_vControls.end());
	m_bHaveAtleastOneTreeNodeChild=false;
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			m_bHaveAtleastOneTreeNodeChild=true;
			break;
		}
	}
}

geTreeNode* geTreeNode::getTopNode()
{
	geTreeNode* parentNode=(geTreeNode*)getParent();
	if(parentNode->getParent()==NULL)
	{
		std::vector<geGUIBase*>* childList=parentNode->getChildControls();
		if(*childList->begin()==this)
			return this;
	}

	std::vector<geGUIBase*>* childList=parentNode->getChildControls();
	geGUIBase* tvnode_prevNode=NULL;
	for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			if(tvnode==this)
			{
				if(tvnode_prevNode==NULL)
				{
					if(getParent()->getParent()==NULL)	//root
						tvnode_prevNode=this;
					else
						tvnode_prevNode=getParent();
				}
				break;
			}
			tvnode_prevNode=tvnode;
		}
	}

	if(tvnode_prevNode)
	{
		geTreeNode* temp=(geTreeNode*)tvnode_prevNode;
		if(temp->isOpenNode())
		{
			if(temp==getParent())
				return temp;
			else
				return temp->getBottomMostNode();
		}
		else
			return (geTreeNode*)tvnode_prevNode;
	}

	return (geTreeNode*)getParent();
}

geTreeNode* geTreeNode::getBottomMostNode()
{
	if(isOpenNode())
	{
		if(m_vControls.size()==0)
			return this;

		geGUIBase* tvnode_bottomNode=NULL;
		for(std::vector<geGUIBase*>::reverse_iterator rit = m_vControls.rbegin(); rit != m_vControls.rend(); ++rit)
		{
			geGUIBase* tvnode = *rit;
			if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
			{
				//tvnode_bottomNode=tvnode;
				return ((geTreeNode*)tvnode)->getBottomMostNode();
			}
		}
	}
	else
		return this;
}

geTreeNode* geTreeNode::getParentBottomNextNode(geTreeNode* childnodeToCheck)
{
	geGUIBase* tvnode_bottomNode=NULL;
	for(std::vector<geGUIBase*>::reverse_iterator rit = m_vControls.rbegin(); rit != m_vControls.rend(); ++rit)
	{
		geGUIBase* tvnode = *rit;
		if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			if(childnodeToCheck==tvnode)
			{
				if(getParent()==NULL)
				{
					if(tvnode_bottomNode==NULL)
						return childnodeToCheck;
					return (geTreeNode*)tvnode_bottomNode;
				}

				//return ((geTreeNode*)getParent())->getParentBottomNextNode((geTreeNode*)tvnode_bottomNode);
				if(tvnode_bottomNode==NULL)
					return ((geTreeNode*)getParent())->getParentBottomNextNode((geTreeNode*)this);
				return (geTreeNode*)tvnode_bottomNode;//tvnode;//((geTreeNode*)tvnode->getParent())->getParentBottomNextNode((geTreeNode*)tvnode);
			}
			tvnode_bottomNode=tvnode;
		}
	}
}

geTreeNode* geTreeNode::getBottomNode()
{
	if(isOpenNode() && m_vControls.size())
	{
		geTreeNode* tvnode_nextNode=NULL;
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
			{
				tvnode_nextNode=(geTreeNode*)tvnode;
				break;
			}
		}
		
		if(tvnode_nextNode)
			return tvnode_nextNode;
	}
	else
	{
		geTreeNode* parentNode=(geTreeNode*)getParent();
		geTreeNode* ret_node=parentNode->getParentBottomNextNode(this);

		return ret_node;
		/*
		geTreeNode* parentNode=(geTreeNode*)getParent();
		std::vector<geGUIBase*>* childList=parentNode->getChildControls();
		geGUIBase* tvnode_prevNode=NULL;
		for(std::vector<geGUIBase*>::reverse_iterator rit = childList->rbegin(); rit != childList->rend(); ++rit)
		{
			geGUIBase* tvnode = *rit;
			if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
			{
				if(tvnode==this)
				{
					break;
				}
				tvnode_prevNode=tvnode;

			}
		}

		if(tvnode_prevNode)
			return (geTreeNode*)tvnode_prevNode;
		{
			geTreeNode* parentOfParentNode=(geTreeNode*)parentNode->getParent();
			if(parentOfParentNode)
			{
						std::vector<geGUIBase*>* childList=parentOfParentNode->getChildControls();
						geGUIBase* tvnode_parentprevNode=NULL;
						for(std::vector<geGUIBase*>::reverse_iterator rit = childList->rbegin(); rit != childList->rend(); ++rit)
						{
							geGUIBase* tvnode = *rit;
							if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
							{
								if(tvnode==parentNode)
								{
									break;
								}
								tvnode_parentprevNode=tvnode;
							}
						}

						if(tvnode_parentprevNode)
							return (geTreeNode*)tvnode_parentprevNode;
			}
		}
		*/
	}

	return NULL;
}

int geTreeNode::getTVNodeChildCount()
{
	int cnt=0;
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			cnt++;
		}
	}

	return cnt;
}

void geTreeNode::onNotify(int msg)
{
	if(!m_pParentTreeView)
	{
		notifyParent(msg);
	}
	else
	{
		switch(msg)
		{
		case 700:	//invalidate view msg
			{
				geTreeView* tree = (geTreeView*)m_pParentTreeView;
				tree->refreshTreeView();
				tree->resetSelectedNodePtr();
			}
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////
geTreeView::geTreeView():
	geGUIBase(GEGUI_TREEVIEW, "TreeView")
{
	m_pRootNode=NULL;
	m_fVirtualYPos=0.0f;
	m_pTVObserver=NULL;
	m_pCurrentSelectedNodePtr=NULL;
}

geTreeView::geTreeView(const char* name):
	geGUIBase(GEGUI_TREEVIEW, name)
{
	m_pRootNode=NULL;
	m_fVirtualYPos=0.0f;
	m_pTVObserver=NULL;
	m_pCurrentSelectedNodePtr=NULL;
}

geTreeView::~geTreeView()
{
	GE_DELETE(m_pRootNode);
	m_cSelectedNodes.clear();
}

void geTreeView::create(rendererGL10* renderer, geGUIBase* parent, const char* name, MTreeViewObserver* pObserver)
{
	m_pTVObserver=pObserver;
	createBase(renderer, parent);
	setSizable(true);
	setPos(0, 0);
	setSize(parent->getSize().x, parent->getSize().y-parent->getTopMarginOffsetHeight());

	m_cVerticalScrollBar.create(renderer, this, this);

	m_fVirtualYPos=0.0f;

	m_pRootNode = new geTreeNode(renderer, NULL, "root", NULL);
	m_pRootNode->setParentTreeView(this);
	m_pCurrentSelectedNodePtr=NULL;
}

void geTreeView::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x/*-m_pRootNode->getXOffset()*/, (int)(m_cPos.y+m_fVirtualYPos), 0.0f);
	m_pRootNode->draw();
	glPopMatrix();

	m_cVerticalScrollBar.draw();
}

void geTreeView::onDragDrop(int x, int y, MDataObject* dropObject)
{
	int xoff=-m_pRootNode->getXOffset();
	int yoff=(int)m_fVirtualYPos-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = m_pRootNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);
	if(selectedNode)
	{
		//std::vector<geGUIBase*>* list=selectedNode->getChildControls();
		//for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
		//{
		//	geGUIBase* obj = *it;
		//	float xxx=x-(/*getPos().x+*/selectedNode->getPos().x+xoff)+m_pRootNode->getXOffset();
		//	float yyy=y-(/*getPos().y+*/selectedNode->getPos().y/*+getTopMarginOffsetHeight()*/+yoff+GE_TREEVIEWNODE_CY);

		//	obj->DragDrop(xxx, yyy, dropObject);
		//}
		selectedNode->DragDrop(x-xoff+m_pRootNode->getXOffset(), y-yoff+GE_TREEVIEWNODE_CY, dropObject);
		return;
	}

	geGUIBase::onDragDrop(x, y, dropObject);
}

void geTreeView::onCommand(int cmd)
{
	geGUIBase* selectedNode = getSelectedNode();
	if(selectedNode)
		selectedNode->DoCommand(cmd);
	//std::vector<geGUIBase*>* list=selectedNode->getChildControls();
	//for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	//{
	//	geGUIBase* obj = *it;
	//	float xxx=x-(/*getPos().x+*/selectedNode->getPos().x+xoff)+m_pRootNode->getXOffset();
	//	float yyy=y-(/*getPos().y+*/selectedNode->getPos().y/*+getTopMarginOffsetHeight()*/+yoff+GE_TREEVIEWNODE_CY);

	//	obj->DragDrop(xxx, yyy, dropObject);
	//}
}

bool geTreeView::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(m_cVerticalScrollBar.MouseLButtonDown(x, y, nFlag))
		return false;

	int xoff=-m_pRootNode->getXOffset();
	int yoff=(int)m_fVirtualYPos-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = m_pRootNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);
	if(selectedNode)
	{
		if(bClickedOnToggleButton)
		{
			if(selectedNode->isOpenNode())
			{
				selectedNode->closeNode();
			}
			else
			{
				selectedNode->openNode();
			}
		}
		else
		{
			m_fOffsetCacheForMouseMove.set(xoff+m_pRootNode->getXOffset(), yoff+GE_TREEVIEWNODE_CY);
			selectedNode->MouseLButtonDown(x-xoff+m_pRootNode->getXOffset(), y-yoff+GE_TREEVIEWNODE_CY, nFlag);

			bool bAlreadySelected=false;
			for(std::vector<geTreeNode*>::iterator it = m_cSelectedNodes.begin(); it != m_cSelectedNodes.end(); ++it)
			{
				geTreeNode* node = *it;
				if(node==selectedNode)
				{
					bAlreadySelected=true;
					break;
				}
			}

			if(nFlag&MK_CONTROL)
			{
				selectedNode->selectNode();
				if(!bAlreadySelected)
				{
					if(m_cSelectedNodes.size()==0)
					{
						if(m_pCurrentSelectedNodePtr)
							m_pCurrentSelectedNodePtr->unselectNode();
					}
					m_cSelectedNodes.push_back(selectedNode);
				}
				else
				{
					m_cSelectedNodes.erase(std::remove(m_cSelectedNodes.begin(), m_cSelectedNodes.end(), selectedNode), m_cSelectedNodes.end());
					selectedNode->unselectNode();
					if(m_pCurrentSelectedNodePtr==selectedNode)
						m_pCurrentSelectedNodePtr=NULL;
				}
			}
			else
			{
				if(!bAlreadySelected)
				{
					for(std::vector<geTreeNode*>::iterator it = m_cSelectedNodes.begin(); it != m_cSelectedNodes.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					m_cSelectedNodes.clear();

					selectedNode->selectNode();
					if(m_pCurrentSelectedNodePtr && m_pCurrentSelectedNodePtr!=selectedNode)
						m_pCurrentSelectedNodePtr->unselectNode();
					m_pCurrentSelectedNodePtr=selectedNode;
					m_cSelectedNodes.push_back(selectedNode);
				}
				else
				{
					m_pCurrentSelectedNodePtr=selectedNode;
					selectedNode->selectNode();
					m_pTVObserver->onTVSelectionChange(selectedNode, this);
				}
			}

			if(!bAlreadySelected)
			{
				if(!(nFlag&MK_CONTROL) && m_pCurrentSelectedNodePtr && m_pCurrentSelectedNodePtr!=selectedNode)
					m_pCurrentSelectedNodePtr->unselectNode();
				m_pCurrentSelectedNodePtr=selectedNode;
				m_pTVObserver->onTVSelectionChange(selectedNode, this);
			}
		}

		
		m_iTotalHeightOfAllNodes=0;
		m_pRootNode->getTotalHeightofAllNodes(m_iTotalHeightOfAllNodes);
		m_cVerticalScrollBar.setConetentHeight(m_iTotalHeightOfAllNodes);
		return true;
	}

	return false;
}

bool geTreeView::onMouseLButtonUp(float x, float y, int nFlag)
{
	m_cVerticalScrollBar.MouseLButtonUp(x, y, nFlag);

	int xoff=-m_pRootNode->getXOffset();
	int yoff=(int)m_fVirtualYPos-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = m_pRootNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);
	if(selectedNode)
	{
		selectedNode->MouseLButtonUp(x-xoff+m_pRootNode->getXOffset(), y-yoff+GE_TREEVIEWNODE_CY, nFlag);
	}

	return true;//arun-check geGUIBase::onMouseLButtonUp(x, y, nFlag);
}

bool geTreeView::onMouseMove(float x, float y, int flag)
{
	m_cVerticalScrollBar.MouseMove(x, y, flag);

	//arun-check: need to optimize
	int xoff=-m_pRootNode->getXOffset();
	int yoff=(int)m_fVirtualYPos-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = m_pRootNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);
	if(selectedNode)
	{
		selectedNode->MouseMove(x-xoff+m_pRootNode->getXOffset(), y-yoff+GE_TREEVIEWNODE_CY, flag);
	}

	//if(m_pSelectedNodePtr)
	//{
	//	//int xoff=-m_pRootNode->getXOffset();
	//	//int yoff=(int)m_fVirtualYPos-GE_TREEVIEWNODE_CY;
	//	m_pSelectedNodePtr->MouseMove(x-m_fOffsetCacheForMouseMove.x, y-m_fOffsetCacheForMouseMove.y, flag);
	//}
	return geGUIBase::onMouseMove(x, y, flag);
}

void geTreeView::onMouseWheel(int zDelta, int x, int y, int flag)
{
	m_cVerticalScrollBar.scrollMouseWheel(zDelta, x, y, flag);
	geGUIBase::onMouseWheel(zDelta, x, y, flag);
}

geTreeNode* geTreeView::getTVNode(float x, float y)
{
	if(m_cVerticalScrollBar.isPointInsideClientArea(x, y))
		return NULL;

	int xoff=-m_pRootNode->getXOffset();
	int yoff=(int)m_fVirtualYPos-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = m_pRootNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);

	return selectedNode;
}

void geTreeView::onSize(float cx, float cy, int flag)
{
	m_cVerticalScrollBar.setPos(cx-SCROLLBAR_SIZE, 0);
	m_cVerticalScrollBar.setSize(SCROLLBAR_SIZE, cy);

	if(m_pRootNode)
	{
		//m_iTotalHeightOfAllNodes=0;
		//m_pRootNode->getTotalHeightofAllNodes(m_iTotalHeightOfAllNodes);
		m_cVerticalScrollBar.setConetentHeight(m_iTotalHeightOfAllNodes);
	}

	geGUIBase::onSize(cx, cy, flag);
}

void geTreeView::onResizeComplete()
{
	m_cVerticalScrollBar.setPos(m_cSize.x-SCROLLBAR_SIZE, 0);
	m_cVerticalScrollBar.setSize(SCROLLBAR_SIZE, m_cSize.y);
	if(m_pRootNode)
	{
		if(m_pRootNode->getSize().x!=m_cSize.x)
			m_pRootNode->traverseSetWidth(m_cSize.x);

		m_iTotalHeightOfAllNodes=0;
		m_pRootNode->getTotalHeightofAllNodes(m_iTotalHeightOfAllNodes);
		m_cVerticalScrollBar.setConetentHeight(m_iTotalHeightOfAllNodes);
	}

	geGUIBase::onResizeComplete();
}

void geTreeView::onCancelEngagedControls()
{
	m_cVerticalScrollBar.CancelEngagedControls();
	m_pRootNode->CancelEngagedControls();

	geGUIBase::onCancelEngagedControls();
}

void geTreeView::onScrollBarChange(geScrollBar* scrollbar)
{
	m_fVirtualYPos=-(scrollbar->getScrollGrabberYPos()/(scrollbar->getSize().y-scrollbar->getScrollGrabberHeight())) *(1.0f-scrollbar->getActualRatio())*scrollbar->getContentHeight();//+getParent()->getTopMarginOffsetHeight();
}

void geTreeView::onFocusLost()
{
	for(std::vector<geTreeNode*>::iterator it = m_cSelectedNodes.begin(); it != m_cSelectedNodes.end(); ++it)
	{
		geTreeNode* node = *it;
		node->deFocus();
	}

	geGUIBase::onFocusLost();
}

void geTreeView::clearAndDestroyAll()
{
	m_pCurrentSelectedNodePtr=NULL;
	m_cSelectedNodes.clear();
	GE_DELETE(m_pRootNode);
	m_fVirtualYPos=0.0f;
	m_cVerticalScrollBar.setConetentHeight(0);

	m_pRootNode = new geTreeNode(m_pRenderer, NULL, "root", NULL);
	m_pRootNode->setParentTreeView(this);
}

void geTreeView::refreshTreeView(bool bDoNotResetScrollBarPosition)
{
	float old_ypos=m_cVerticalScrollBar.getScrollGrabberYPos();
	m_cVerticalScrollBar.setPos(m_cSize.x-SCROLLBAR_SIZE, 0);
	m_cVerticalScrollBar.setSize(SCROLLBAR_SIZE, m_cSize.y);
	m_cVerticalScrollBar.resetScrollBar();

	if(m_pRootNode)
	{
		m_iTotalHeightOfAllNodes=0;
		m_pRootNode->getTotalHeightofAllNodes(m_iTotalHeightOfAllNodes);
		if(bDoNotResetScrollBarPosition)
			m_cVerticalScrollBar.setScrollGrabberYPos(old_ypos);
		m_cVerticalScrollBar.setConetentHeight(m_iTotalHeightOfAllNodes);
	}

}

void geTreeView::quick_refreshTreeViewForOnlyVerticalScrollBar(float deltaheight)
{
	m_cVerticalScrollBar.setConetentHeight(m_cVerticalScrollBar.getContentHeight()+deltaheight);
}

bool geTreeView::onKeyDown(int charValue, int flag)
{
	//37-left
	//38-up
	//39-right
	//40-down

	geTreeNode* pSelectedNodePtr = m_pCurrentSelectedNodePtr;
	if(charValue==38)
	{
		geTreeNode* topNode=NULL;
		if(pSelectedNodePtr)
		{
			topNode=pSelectedNodePtr->getTopNode();
			if(topNode==NULL)
			{
				return geGUIBase::onKeyDown(charValue, flag);
			}
			else
			{
				if(topNode!=pSelectedNodePtr)
				{
					for(std::vector<geTreeNode*>::iterator it = m_cSelectedNodes.begin(); it != m_cSelectedNodes.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					m_cSelectedNodes.clear();

					pSelectedNodePtr->unselectNode();
					pSelectedNodePtr=topNode;
					pSelectedNodePtr->selectNode();
					m_pCurrentSelectedNodePtr=pSelectedNodePtr;
					m_pTVObserver->onTVSelectionChange(topNode, this);
					refreshTreeView(true);
					return geGUIBase::onKeyDown(charValue, flag);
				}
			}
		}
	}
	else if(charValue==40)
	{
		geTreeNode* bottomNode=NULL;
		if(pSelectedNodePtr)
		{
			bottomNode=pSelectedNodePtr->getBottomNode();
			if(bottomNode==NULL)
			{
				return geGUIBase::onKeyDown(charValue, flag);
			}
			else
			{
				if(bottomNode!=pSelectedNodePtr)
				{
					for(std::vector<geTreeNode*>::iterator it = m_cSelectedNodes.begin(); it != m_cSelectedNodes.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					m_cSelectedNodes.clear();

					pSelectedNodePtr->unselectNode();
					pSelectedNodePtr=bottomNode;
					pSelectedNodePtr->selectNode();
					m_pCurrentSelectedNodePtr=pSelectedNodePtr;
					m_pTVObserver->onTVSelectionChange(bottomNode, this);
					refreshTreeView(true);
					return geGUIBase::onKeyDown(charValue, flag);
				}
			}
		}
	}
	else if(charValue==37)
	{
		if(pSelectedNodePtr)
		{
			if(pSelectedNodePtr->isOpenNode()/* && pSelectedNodePtr->getChildControls()->size()*/)
				pSelectedNodePtr->closeNode();
			else
			{
				geTreeNode* parentNode = (geTreeNode*)pSelectedNodePtr->getParent();
				if(parentNode && parentNode!=getRoot())
				{
					for(std::vector<geTreeNode*>::iterator it = m_cSelectedNodes.begin(); it != m_cSelectedNodes.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					m_cSelectedNodes.clear();

					pSelectedNodePtr->unselectNode();
					pSelectedNodePtr=parentNode;
					pSelectedNodePtr->selectNode();
					pSelectedNodePtr->closeNode();
					m_pCurrentSelectedNodePtr=pSelectedNodePtr;
					refreshTreeView(true);
					m_pTVObserver->onTVSelectionChange(parentNode, this);
				}
			}
		}
	}
	else if(charValue==39)
	{
		if(pSelectedNodePtr)
		{
			if(!pSelectedNodePtr->isOpenNode())
			{
				pSelectedNodePtr->openNode();
				if(pSelectedNodePtr->getTVNodeChildCount())
				{
					pSelectedNodePtr->unselectNode();
					//find the first treenode child
					for(std::vector<geGUIBase*>::iterator it = pSelectedNodePtr->getChildControls()->begin(); it != pSelectedNodePtr->getChildControls()->end(); ++it)
					{
						geGUIBase* tvnode = *it;
						if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
						{
							pSelectedNodePtr=(geTreeNode*)tvnode;
							break;
						}
					}
					//
					
					for(std::vector<geTreeNode*>::iterator it = m_cSelectedNodes.begin(); it != m_cSelectedNodes.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					m_cSelectedNodes.clear();

					pSelectedNodePtr->selectNode();
					//pSelectedNodePtr->closeNode();
					m_pCurrentSelectedNodePtr=pSelectedNodePtr;
					refreshTreeView(true);
					m_pTVObserver->onTVSelectionChange(pSelectedNodePtr, this);
				}
			}
		}
	}

	return geGUIBase::onKeyDown(charValue, flag);
}

bool geTreeView::onKeyUp(int charValue, int flag)
{
	return geGUIBase::onKeyUp(charValue, flag);
}

geTreeNode* geTreeView::getSelectedNode()
{
	if(m_cSelectedNodes.size())
		return m_cSelectedNodes.at(m_cSelectedNodes.size()-1);
	else
		return NULL;
}

void geTreeView::resetSelectedNodePtr()
{
	m_pCurrentSelectedNodePtr=NULL;
	m_cSelectedNodes.clear();
}