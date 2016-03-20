#pragma once

#include "geGUIBase.h"
#include <vector>
#include "../SpriteManager/Sprite2Dx.h"
#include "geScrollBar.h"
#include "../util/geVector3.h"

#define GE_TREEVIEWNODE_CY	20

class geTreeNode : public geGUIBase
{
private:
	geTreeNode(geFontManager* fontmanager);
public:
	geTreeNode(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager, float xoffset=20.0f);
	virtual ~geTreeNode();

	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);


	virtual void draw();
	virtual void drawNode();

	int getTVNodeChildCount();


	void openNode()		{	is_OpenNode=true;	}
	void closeNode()	{	is_OpenNode=false;	}
	bool isOpenNode()	{	return is_OpenNode;	}
	void deFocus();

	void selectNode();
	void unselectNode();
	void traverseSetWidth(float cx);

	geTreeNode* getSelectedNode(int x, int y, int& xoff, int& yoff, bool& bClickedOnToggleButton);

	void getTotalHeightofAllNodes(int& height);

	void setNodeColor(float r, float g, float b)			{	nodeDiffuseColor.x=r; nodeDiffuseColor.y=g; nodeDiffuseColor.z=b;	}
	void setNodeSelectionColor(float r, float g, float b)	{	nodeSelectionColor.x=r; nodeSelectionColor.y=g; nodeSelectionColor.z=b;	}

	float getXOffset()			{	return offSetX;	}
	void setXOffset(float offX)	{	offSetX=offX;	}

	void onAppendChild(geGUIBase* child);

	void appnendTVChild(geTreeNode* child);
	void removeTVChild(geTreeNode* child);

	void destroyAllTVChilds();

	geTreeNode* getTopNode();
	geTreeNode* getBottomMostNode();
	geTreeNode* getBottomNode();
	geTreeNode* getParentBottomNextNode(geTreeNode* childnodeToCheck);

	void setNodeSprite(Sprite2Dx* sprite);

	//hack
	void setParentTreeView(geGUIBase* parentTreeView)	{	this->parentTreeView=parentTreeView;	}
	geGUIBase* getParentTreeView()						{	return parentTreeView;                  }
	//

	virtual void onNotify(int msg);

protected:
	Sprite2Dx* sprite;

	//std::vector<geTreeNode*> m_vChilds;

	bool is_OpenNode;
	float vertexBufferToggleButtonArray[6*2];

	bool isNodeSelected;
	float offSetX;
	geVector3f nodeDiffuseColor;
	geVector3f nodeSelectionColor;
	bool hasAtleastOneTreeNodeChild;

	//hack
	geGUIBase* parentTreeView;
};

class MTreeViewObserver;
class geTreeView : public geGUIBase, public MScrollBarObserver
{
public:
	geTreeView(geFontManager* fontmanager);
	geTreeView(const char* name, geFontManager* fontmanager);
	virtual ~geTreeView();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, MTreeViewObserver* pObserver);

	virtual void draw();

	geTreeNode* getRoot()	{	return rootTreeNode;	}

	virtual void onScrollBarChange(geScrollBar* scrollbar);

	void clearAndDestroyAll();
	void refreshTreeView(bool bDoNotResetScrollBarPosition=false);
	void quick_refreshTreeViewForOnlyVerticalScrollBar(float deltaheight);	//wont be accurate

	geTreeNode* getSelectedNode();
	void resetSelectedNodePtr();

	std::vector<geTreeNode*>* getSelectedNodeList()		{	return &selectedTreeNodeList;	}

	geScrollBar* getScrollBar()		{	return verticalScrollBar;	}

	geTreeNode* getTVNode(float x, float y);

	float getVirtualYPos()	{	return virtualYPosition;	}

	void selectNode(geTreeNode* nodetoselect);

protected:
	virtual void onSize(float cx, float cy, int flag);
	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

//#if !defined(GEAR_APPLE) //disable Drag-Drops
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
//#endif
    
	virtual void onCommand(int cmd);

	virtual void onCancelEngagedControls();
	virtual void onFocusLost();
	virtual void onResizeComplete();

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

    virtual void setRenderer(rendererGL10* renderer, bool recursive=false);

	int totalHeightOfAllNodes;

private:
	geTreeNode* rootTreeNode;
	geScrollBar* verticalScrollBar;
	float virtualYPosition;
	geTreeNode* currentSelectedTreeNode;
	MTreeViewObserver* treeViewObserver;
	geVector2f offsetCacheForMouseMove;
	std::vector<geTreeNode*> selectedTreeNodeList;
	bool isSelectionChanged;
};

class MTreeViewObserver
{
public:
	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)=0;
};