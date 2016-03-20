#include "geTreeView.h"
#include "geGUIManager.h"

geTreeNode::geTreeNode(geFontManager* fontmanager):
	geGUIBase(GEGUI_TREEVIEW_NODE, "TreeView Node", fontmanager)
{
	//no implementation
	parentTreeView=NULL;
	sprite=NULL;
}

geTreeNode::geTreeNode(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager, float xoffset):
	geGUIBase(GEGUI_TREEVIEW_NODE, name, fontmanager)
{
	createBase(renderer, parent, 0, 0, 1, 1);

	hasAtleastOneTreeNodeChild=false;
	offSetX=xoffset;
	if(parent==NULL)
	{
		setPos(0, 0);
		offSetX=0;
	}
	else
	{
		offSetX=(parent->getParent()==NULL)?0:offSetX;
		setPos(offSetX, GE_TREEVIEWNODE_CY);
	}

    geGUIBase* parentTV = nullptr;
    if(parent)
        parentTV = ((geTreeNode*)parent)->getParentTreeView();
    
    setSize((parentTV)?parentTV->getSize().x : (parent)?parent->getSize().x : 70, GE_TREEVIEWNODE_CY);

	setClientAreaPrimaryActiveForeColor(0.12f, 0.12f, 0.12f, 1.0f);
	applyPrimaryColorToVBClientArea();

	openNode();

	int h=GE_TREEVIEWNODE_CY-4;
	const float togglebutton_linevertLst[6*2] =
	{
		5,		(GE_TREEVIEWNODE_CY-4),
		5+8,	(float)((GE_TREEVIEWNODE_CY-4)-(h>>2)),
		5,		(float)((GE_TREEVIEWNODE_CY-4)-(h>>1)),

		5+4,	(GE_TREEVIEWNODE_CY-4),
		5+8,	(float)((GE_TREEVIEWNODE_CY-4)-(h>>1)),
		5,		(float)((GE_TREEVIEWNODE_CY-4)-(h>>1)),
	};
	memcpy(vertexBufferToggleButtonArray, togglebutton_linevertLst, sizeof(togglebutton_linevertLst));

	parentTreeView=NULL;
	this->sprite=NULL;
	setNodeSprite(sprite);

	isNodeSelected=false;

	setMouseBoundCheck(false);

	setNodeColor(0.12f, 0.12f, 0.12f);
	setNodeSelectionColor(0.24f, 0.38f, 0.57f);
}

geTreeNode::~geTreeNode()
{
	if(selectedControl==this)
		selectedControl=NULL;
	GE_DELETE(sprite);
	destroyAllTVChilds();
}

void geTreeNode::destroyAllTVChilds()
{
	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		GE_DELETE(tvnode);
	}
	childControlList.clear();
}

void geTreeNode::setNodeSprite(Sprite2Dx* sprite)
{
	if(sprite)
	{
		GE_DELETE(this->sprite);
		this->sprite = new Sprite2Dx();
		this->sprite->copySprite(*sprite);
		this->sprite->setPos(5+11, 3);
	}
}

void geTreeNode::drawNode()
{
	if(isNodeSelected)
	{
		drawRect(&vertexBufferClientArea);
	}

	geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

	if(childControlList.size() && hasAtleastOneTreeNodeChild)
	{
		if(is_OpenNode)
			drawTriangle(&vertexBufferToggleButtonArray[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		else
			drawTriangle(&vertexBufferToggleButtonArray[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
	}

	if(sprite)
		sprite->draw();
}

void geTreeNode::draw()
{
	if(is_OpenNode)
	{
		for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
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
//	const float clientarea_vertLst[8] =
//	{
//		cx,	2,
//		0,	2,
//		cx,	cy-1,
//		0,	cy-1,
//	};
//	memcpy(vertexBufferClientArea.vertexArray, clientarea_vertLst, sizeof(clientarea_vertLst));
    vertexBufferClientArea.updateRect(0, 2, cx, cy-3);
}

geTreeNode* geTreeNode::getSelectedNode(int x, int y, int& xoff, int& yoff, bool& bClickedOnToggleButton)
{
	if(y>yoff && y<yoff+m_cSize.y)
	{
		if(x>xoff+4+offSetX && x<xoff+(5+9)+offSetX)
			bClickedOnToggleButton=true;
		return this;
	}

	yoff+=m_cSize.y;

	if(is_OpenNode)
	{
		xoff+=offSetX;
		for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
				continue;
			geTreeNode* selectedNode=((geTreeNode*)tvnode)->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);

			if(selectedNode)
				return selectedNode;
		}
		xoff-=offSetX;
	}

	return NULL;
}

void geTreeNode::getTotalHeightofAllNodes(int& height)
{
	if(is_OpenNode)
	{
		for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
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
	setColor(&vertexBufferClientArea, nodeSelectionColor.x, nodeSelectionColor.y, nodeSelectionColor.z, 1.0f);
	isNodeSelected=true;
}

void geTreeNode::unselectNode()
{
	setColor(&vertexBufferClientArea, nodeDiffuseColor.x, nodeDiffuseColor.y, nodeDiffuseColor.z, 1.0f);
	isNodeSelected=false;
}

void geTreeNode::traverseSetWidth(float cx)
{
	setSize(cx, m_cSize.y);
	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
			continue;
		((geTreeNode*)tvnode)->traverseSetWidth(cx);
	}
}

void geTreeNode::deFocus()
{
	setColor(&vertexBufferClientArea, 0.25f, 0.25f, 0.25f, 1.0f);
}


void geTreeNode::onAppendChild(geGUIBase* child)
{
	if(!hasAtleastOneTreeNodeChild)
		hasAtleastOneTreeNodeChild=(child->getGUIID()==GEGUI_TREEVIEW_NODE);
}

void geTreeNode::appnendTVChild(geTreeNode* child)
{
	child->setPos(child->getXOffset(), GE_TREEVIEWNODE_CY);

	child->setParent(this);
	traverseSetWidth(m_cSize.x);
}

void geTreeNode::removeTVChild(geTreeNode* child)
{
	if(childControlList.size()==0)
		return;

	childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), child), childControlList.end());
	hasAtleastOneTreeNodeChild=false;
	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			hasAtleastOneTreeNodeChild=true;
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
		if(childControlList.size()==0)
			return this;

		//geGUIBase* tvnode_bottomNode=NULL;
		for(std::vector<geGUIBase*>::reverse_iterator rit = childControlList.rbegin(); rit != childControlList.rend(); ++rit)
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
    
    return NULL;
}

geTreeNode* geTreeNode::getParentBottomNextNode(geTreeNode* childnodeToCheck)
{
	geGUIBase* tvnode_bottomNode=NULL;
	for(std::vector<geGUIBase*>::reverse_iterator rit = childControlList.rbegin(); rit != childControlList.rend(); ++rit)
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
    
    return NULL;
}

geTreeNode* geTreeNode::getBottomNode()
{
	if(isOpenNode() && childControlList.size())
	{
		geTreeNode* tvnode_nextNode=NULL;
		for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
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
	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
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
	if(!parentTreeView)
	{
		notifyParent(msg);
	}
	else
	{
		switch(msg)
		{
		case 700:	//invalidate view msg
			{
				geTreeView* tree = (geTreeView*)parentTreeView;
				tree->refreshTreeView();
				tree->resetSelectedNodePtr();
			}
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////
geTreeView::geTreeView(geFontManager* fontmanager):
	geGUIBase(GEGUI_TREEVIEW, "TreeView", fontmanager)
{
	rootTreeNode=NULL;
	virtualYPosition=0.0f;
	treeViewObserver=NULL;
	currentSelectedTreeNode=NULL;
	isSelectionChanged=false;
    verticalScrollBar = new geScrollBar(fontmanager);
}

geTreeView::geTreeView(const char* name, geFontManager* fontmanager):
	geGUIBase(GEGUI_TREEVIEW, name, fontmanager)
{
	rootTreeNode=NULL;
	virtualYPosition=0.0f;
	treeViewObserver=NULL;
	currentSelectedTreeNode=NULL;
	isSelectionChanged=false;
    verticalScrollBar = new geScrollBar(fontmanager);
}

geTreeView::~geTreeView()
{
	GE_DELETE(rootTreeNode);
	selectedTreeNodeList.clear();
    GE_DELETE(verticalScrollBar);
}

void geTreeView::create(rendererGL10* renderer, geGUIBase* parent, const char* name, MTreeViewObserver* pObserver)
{
	treeViewObserver=pObserver;
	createBase(renderer, parent, 0, 0, parent->getSize().x, parent->getSize().y-parent->getTopMarginOffsetHeight());
    setSizable(true);
    
	verticalScrollBar->create(renderer, this, this);

	virtualYPosition=0.0f;
	rootTreeNode = new geTreeNode(renderer, NULL, "root", NULL, fontManagerGUI);
	rootTreeNode->setParentTreeView(this);
	currentSelectedTreeNode=NULL;
}

void geTreeView::setRenderer(rendererGL10* renderer, bool recursive)
{
    if(childControlList.size()!=0)
    {
        printf("ERROR : geTreeView childControlList.size() must be zero !!!\n");
    }
    
    rootTreeNode->setRenderer(renderer, recursive);
}

void geTreeView::draw()
{
    //applyClipIfIamOnMainWindow();
	glPushMatrix();
	glTranslatef(m_cPos.x/*-rootTreeNode->getXOffset()*/, (int)(m_cPos.y+virtualYPosition), 0.0f);
	rootTreeNode->draw();
	glPopMatrix();

	verticalScrollBar->draw();
}

//#if !defined(GEAR_APPLE) //disable Drag-Drop
void geTreeView::onDragDrop(int x, int y, MDropData* dropObject)
{
	int xoff=-rootTreeNode->getXOffset();
	int yoff=(int)virtualYPosition-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = rootTreeNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);
	if(selectedNode)
	{
		//std::vector<geGUIBase*>* list=selectedNode->getChildControls();
		//for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
		//{
		//	geGUIBase* obj = *it;
		//	float xxx=x-(/*getPos().x+*/selectedNode->getPos().x+xoff)+rootTreeNode->getXOffset();
		//	float yyy=y-(/*getPos().y+*/selectedNode->getPos().y/*+getTopMarginOffsetHeight()*/+yoff+GE_TREEVIEWNODE_CY);

		//	obj->DragDrop(xxx, yyy, dropObject);
		//}
		selectedNode->DragDrop(x-xoff+rootTreeNode->getXOffset(), y-yoff+GE_TREEVIEWNODE_CY, dropObject);
		return;
	}

	geGUIBase::onDragDrop(x, y, dropObject);
}
//#endif

void geTreeView::onCommand(int cmd)
{
	geGUIBase* selectedNode = getSelectedNode();
	if(selectedNode)
		selectedNode->DoCommand(cmd);
	//std::vector<geGUIBase*>* list=selectedNode->getChildControls();
	//for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	//{
	//	geGUIBase* obj = *it;
	//	float xxx=x-(/*getPos().x+*/selectedNode->getPos().x+xoff)+rootTreeNode->getXOffset();
	//	float yyy=y-(/*getPos().y+*/selectedNode->getPos().y/*+getTopMarginOffsetHeight()*/+yoff+GE_TREEVIEWNODE_CY);

	//	obj->DragDrop(xxx, yyy, dropObject);
	//}
}

bool geTreeView::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(verticalScrollBar->MouseLButtonDown(x, y, nFlag))
		return false;

	int xoff=-rootTreeNode->getXOffset();
	int yoff=(int)virtualYPosition-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = rootTreeNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);
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
			offsetCacheForMouseMove.set(xoff+rootTreeNode->getXOffset(), yoff+GE_TREEVIEWNODE_CY);
			selectedNode->MouseLButtonDown(x-xoff+rootTreeNode->getXOffset(), y-yoff+GE_TREEVIEWNODE_CY, nFlag);

			bool bAlreadySelected=false;
			for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
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
					if(selectedTreeNodeList.size()==0)
					{
						if(currentSelectedTreeNode)
							currentSelectedTreeNode->unselectNode();
					}
					selectedTreeNodeList.push_back(selectedNode);
				}
				else
				{
					selectedTreeNodeList.erase(std::remove(selectedTreeNodeList.begin(), selectedTreeNodeList.end(), selectedNode), selectedTreeNodeList.end());
					selectedNode->unselectNode();
					if(currentSelectedTreeNode==selectedNode)
						currentSelectedTreeNode=NULL;
				}
			}
			else
			{
				if(!bAlreadySelected)
				{
					for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					selectedTreeNodeList.clear();

					selectedNode->selectNode();
					if(currentSelectedTreeNode && currentSelectedTreeNode!=selectedNode)
						currentSelectedTreeNode->unselectNode();
					currentSelectedTreeNode=selectedNode;
					selectedTreeNodeList.push_back(selectedNode);
				}
				else
				{
					currentSelectedTreeNode=selectedNode;
					selectedNode->selectNode();
					//treeViewObserver->onTVSelectionChange(selectedNode, this);	//moved to onMouseLButtonUp
					isSelectionChanged=true;
				}
			}

			if(!bAlreadySelected)
			{
				if(!(nFlag&MK_CONTROL) && currentSelectedTreeNode && currentSelectedTreeNode!=selectedNode)
					currentSelectedTreeNode->unselectNode();
				currentSelectedTreeNode=selectedNode;
				//treeViewObserver->onTVSelectionChange(selectedNode, this);	//moved to onMouseLButtonUp
				isSelectionChanged=true;
			}
		}

		
		totalHeightOfAllNodes=0;
		rootTreeNode->getTotalHeightofAllNodes(totalHeightOfAllNodes);
		verticalScrollBar->setConetentSize(totalHeightOfAllNodes);
		return true;
	}

	return false;
}

bool geTreeView::onMouseLButtonUp(float x, float y, int nFlag)
{
	verticalScrollBar->MouseLButtonUp(x, y, nFlag);

	if(isSelectionChanged)
	{
		if(currentSelectedTreeNode)
			treeViewObserver->onTVSelectionChange(currentSelectedTreeNode, this);
		isSelectionChanged=false;
	}

	int xoff=-rootTreeNode->getXOffset();
	int yoff=(int)virtualYPosition-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = rootTreeNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);
	if(selectedNode)
	{
		selectedNode->MouseLButtonUp(x-xoff+rootTreeNode->getXOffset(), y-yoff+GE_TREEVIEWNODE_CY, nFlag);
	}

	return true;//arun-check geGUIBase::onMouseLButtonUp(x, y, nFlag);
}

bool geTreeView::onMouseMove(float x, float y, int flag)
{
	verticalScrollBar->MouseMove(x, y, flag);

	//arun-check: need to optimize
	int xoff=-rootTreeNode->getXOffset();
	int yoff=(int)virtualYPosition-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = rootTreeNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);
	if(selectedNode)
	{
		selectedNode->MouseMove(x-xoff+rootTreeNode->getXOffset(), y-yoff+GE_TREEVIEWNODE_CY, flag);
	}

	//if(m_pSelectedNodePtr)
	//{
	//	//int xoff=-rootTreeNode->getXOffset();
	//	//int yoff=(int)virtualYPosition-GE_TREEVIEWNODE_CY;
	//	m_pSelectedNodePtr->MouseMove(x-offsetCacheForMouseMove.x, y-offsetCacheForMouseMove.y, flag);
	//}
	return geGUIBase::onMouseMove(x, y, flag);
}

void geTreeView::onMouseWheel(int zDelta, int x, int y, int flag)
{
	verticalScrollBar->scrollMouseWheel(zDelta, x, y, flag);
	geGUIBase::onMouseWheel(zDelta, x, y, flag);
}

geTreeNode* geTreeView::getTVNode(float x, float y)
{
	if(verticalScrollBar->isPointInsideClientArea(x, y))
		return NULL;

	int xoff=-rootTreeNode->getXOffset();
	int yoff=(int)virtualYPosition-GE_TREEVIEWNODE_CY;
	bool bClickedOnToggleButton=false;
	geTreeNode* selectedNode = rootTreeNode->getSelectedNode(x, y, xoff, yoff, bClickedOnToggleButton);

	return selectedNode;
}

void geTreeView::onSize(float cx, float cy, int flag)
{
	verticalScrollBar->setPos(cx-SCROLLBAR_SIZE, 0);
	verticalScrollBar->setSize(SCROLLBAR_SIZE, cy);

	if(rootTreeNode)
	{
		//totalHeightOfAllNodes=0;
		//rootTreeNode->getTotalHeightofAllNodes(totalHeightOfAllNodes);
		verticalScrollBar->setConetentSize(totalHeightOfAllNodes);
	}

	geGUIBase::onSize(cx, cy, flag);
}

void geTreeView::onResizeComplete()
{
	verticalScrollBar->setPos(m_cSize.x-SCROLLBAR_SIZE, 0);
	verticalScrollBar->setSize(SCROLLBAR_SIZE, m_cSize.y);
	if(rootTreeNode)
	{
		if(rootTreeNode->getSize().x!=m_cSize.x)
			rootTreeNode->traverseSetWidth(m_cSize.x);

		totalHeightOfAllNodes=0;
		rootTreeNode->getTotalHeightofAllNodes(totalHeightOfAllNodes);
		verticalScrollBar->setConetentSize(totalHeightOfAllNodes);
	}

	geGUIBase::onResizeComplete();
}

void geTreeView::onCancelEngagedControls()
{
	verticalScrollBar->CancelEngagedControls();
	rootTreeNode->CancelEngagedControls();
	isSelectionChanged=false;
	geGUIBase::onCancelEngagedControls();
}

void geTreeView::onScrollBarChange(geScrollBar* scrollbar)
{
	virtualYPosition=-(scrollbar->getScrollGrabberPos()/(scrollbar->getSize().y-scrollbar->getScrollGrabberSize())) *(1.0f-scrollbar->getActualRatio())*scrollbar->getContentSize();//+getParent()->getTopMarginOffsetHeight();
}

void geTreeView::onFocusLost()
{
	for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
	{
		geTreeNode* node = *it;
		node->deFocus();
	}

	geGUIBase::onFocusLost();
}

void geTreeView::clearAndDestroyAll()
{
	isSelectionChanged=false;
	currentSelectedTreeNode=NULL;
	selectedTreeNodeList.clear();
	GE_DELETE(rootTreeNode);
	virtualYPosition=0.0f;
	verticalScrollBar->setConetentSize(0);

	rootTreeNode = new geTreeNode(rendererGUI, NULL, "root", NULL, fontManagerGUI);
	rootTreeNode->setParentTreeView(this);
}

void geTreeView::refreshTreeView(bool bDoNotResetScrollBarPosition)
{
	isSelectionChanged=false;
	float old_ypos=verticalScrollBar->getScrollGrabberPos();
	verticalScrollBar->setPos(m_cSize.x-SCROLLBAR_SIZE, 0);
	verticalScrollBar->setSize(SCROLLBAR_SIZE, m_cSize.y);
	verticalScrollBar->resetScrollBar();

	if(rootTreeNode)
	{
		totalHeightOfAllNodes=0;
		rootTreeNode->getTotalHeightofAllNodes(totalHeightOfAllNodes);
		if(bDoNotResetScrollBarPosition)
			verticalScrollBar->setScrollGrabberPos(old_ypos);
		verticalScrollBar->setConetentSize(totalHeightOfAllNodes);
	}

}

void geTreeView::quick_refreshTreeViewForOnlyVerticalScrollBar(float deltaheight)
{
	verticalScrollBar->setConetentSize(verticalScrollBar->getContentSize()+deltaheight);
}

bool geTreeView::onKeyDown(int charValue, int flag)
{
	//37-left
	//38-up
	//39-right
	//40-down

	geTreeNode* pSelectedNodePtr = currentSelectedTreeNode;
	if (charValue == SDL_SCANCODE_UP)//38
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
					for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					selectedTreeNodeList.clear();

					pSelectedNodePtr->unselectNode();
					pSelectedNodePtr=topNode;
					pSelectedNodePtr->selectNode();
					currentSelectedTreeNode=pSelectedNodePtr;
					treeViewObserver->onTVSelectionChange(topNode, this);
					refreshTreeView(true);
					return geGUIBase::onKeyDown(charValue, flag);
				}
			}
		}
	}
	else if (charValue == SDL_SCANCODE_DOWN)//40
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
					for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					selectedTreeNodeList.clear();

					pSelectedNodePtr->unselectNode();
					pSelectedNodePtr=bottomNode;
					pSelectedNodePtr->selectNode();
					currentSelectedTreeNode=pSelectedNodePtr;
					treeViewObserver->onTVSelectionChange(bottomNode, this);
					refreshTreeView(true);
					return geGUIBase::onKeyDown(charValue, flag);
				}
			}
		}
	}
	else if (charValue == SDL_SCANCODE_LEFT)//37
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
					for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					selectedTreeNodeList.clear();

					pSelectedNodePtr->unselectNode();
					pSelectedNodePtr=parentNode;
					pSelectedNodePtr->selectNode();
					pSelectedNodePtr->closeNode();
					currentSelectedTreeNode=pSelectedNodePtr;
					refreshTreeView(true);
					treeViewObserver->onTVSelectionChange(parentNode, this);
				}
			}
		}
	}
	else if (charValue == SDL_SCANCODE_RIGHT)//39
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
					
					for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
					{
						geTreeNode* node = *it;
						node->unselectNode();
					}
					selectedTreeNodeList.clear();

					pSelectedNodePtr->selectNode();
					//pSelectedNodePtr->closeNode();
					currentSelectedTreeNode=pSelectedNodePtr;
					refreshTreeView(true);
					treeViewObserver->onTVSelectionChange(pSelectedNodePtr, this);
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
	if(selectedTreeNodeList.size())
		return selectedTreeNodeList.at(selectedTreeNodeList.size()-1);
	else
		return NULL;
}

void geTreeView::resetSelectedNodePtr()
{
	currentSelectedTreeNode=NULL;
	selectedTreeNodeList.clear();
}

void geTreeView::selectNode(geTreeNode* nodetoselect)
{
	bool bAlreadySelected=false;
	for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
	{
		geTreeNode* node = *it;
		if(node==nodetoselect)
		{
			bAlreadySelected=true;
			break;
		}
	}

	if(!bAlreadySelected)
	{
		for(std::vector<geTreeNode*>::iterator it = selectedTreeNodeList.begin(); it != selectedTreeNodeList.end(); ++it)
		{
			geTreeNode* node = *it;
			node->unselectNode();
		}
		selectedTreeNodeList.clear();

		nodetoselect->selectNode();
		if(currentSelectedTreeNode && currentSelectedTreeNode!=nodetoselect)
			currentSelectedTreeNode->unselectNode();
		currentSelectedTreeNode=nodetoselect;
		selectedTreeNodeList.push_back(nodetoselect);
	}
	else
	{
		currentSelectedTreeNode=nodetoselect;
		nodetoselect->selectNode();
	}
	
	//open parent node
	geTreeNode* pnode=nodetoselect;
	while(pnode)
	{
		if(pnode!=nodetoselect)
		{
			pnode->openNode();
		}
		pnode=(geTreeNode*)pnode->getParent();
	}

	treeViewObserver->onTVSelectionChange(nodetoselect, this);
	totalHeightOfAllNodes=0;
	rootTreeNode->getTotalHeightofAllNodes(totalHeightOfAllNodes);
	verticalScrollBar->setConetentSize(totalHeightOfAllNodes);
}