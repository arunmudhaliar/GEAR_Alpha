#ifndef GETREEVIEW_H
#define GETREEVIEW_H

#include "geGUIBase.h"
#include <vector>
#include "../SpriteManager/Sprite2Dx.h"
#include "geScrollBar.h"
#include "../util/geVector3.h"

#define GE_TREEVIEWNODE_CY	20

class geTreeNode : public geGUIBase
{
private:
	geTreeNode();
public:
	geTreeNode(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, float xoffset=20.0f);
	virtual ~geTreeNode();

	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);


	virtual void draw();
	virtual void drawNode();

	int getTVNodeChildCount();


	void openNode()		{	m_bNodeOpen=true;	}
	void closeNode()	{	m_bNodeOpen=false;	}
	bool isOpenNode()	{	return m_bNodeOpen;	}
	void deFocus();

	void selectNode();
	void unselectNode();
	void traverseSetWidth(float cx);

	geTreeNode* getSelectedNode(int x, int y, int& xoff, int& yoff, bool& bClickedOnToggleButton);

	void getTotalHeightofAllNodes(int& height);

	void setNodeColor(float r, float g, float b)			{	m_cNodeColor.x=r; m_cNodeColor.y=g; m_cNodeColor.z=b;	}
	void setNodeSelectionColor(float r, float g, float b)	{	m_cNodeSelectionColor.x=r; m_cNodeSelectionColor.y=g; m_cNodeSelectionColor.z=b;	}

	float getXOffset()			{	return m_fXOffSet;	}
	void setXOffset(float offX)	{	m_fXOffSet=offX;	}

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
	void setParentTreeView(geGUIBase* parentTreeView)	{	m_pParentTreeView=parentTreeView;	}
	geGUIBase* getParentTreeView()						{	return m_pParentTreeView;			}
	//

	virtual void onNotify(int msg);

protected:
	Sprite2Dx* m_pSprite;

	//std::vector<geTreeNode*> m_vChilds;

	bool m_bNodeOpen;
	float m_cVBLayoutToggleButtonLine[6*2];

	bool m_bSelected;
	float m_fXOffSet;
	geVector3f m_cNodeColor;
	geVector3f m_cNodeSelectionColor;
	bool m_bHaveAtleastOneTreeNodeChild;

	//hack
	geGUIBase* m_pParentTreeView;
};

class MTreeViewObserver;
class geTreeView : public geGUIBase, public MScrollBarObserver
{
public:
	geTreeView();
	geTreeView(const char* name);
	virtual ~geTreeView();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, MTreeViewObserver* pObserver);

	virtual void draw();

	geTreeNode* getRoot()	{	return m_pRootNode;	}

	virtual void onScrollBarChange(geScrollBar* scrollbar);

	void clearAndDestroyAll();
	void refreshTreeView(bool bDoNotResetScrollBarPosition=false);
	void quick_refreshTreeViewForOnlyVerticalScrollBar(float deltaheight);	//wont be accurate

	geTreeNode* getSelectedNode();
	void resetSelectedNodePtr();

	std::vector<geTreeNode*>* getSelectedNodeList()		{	return &m_cSelectedNodes;	}

	geScrollBar* getScrollBar()		{	return &m_cVerticalScrollBar;	}

	geTreeNode* getTVNode(float x, float y);

	float getVirtualYPos()	{	return m_fVirtualYPos;	}

protected:
	virtual void onSize(float cx, float cy, int flag);
	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

	virtual void onDragDrop(int x, int y, MDataObject* dropObject);

	virtual void onCommand(int cmd);

	virtual void onCancelEngagedControls();
	virtual void onFocusLost();
	virtual void onResizeComplete();

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

	int m_iTotalHeightOfAllNodes;

private:
	geTreeNode* m_pRootNode;
	geScrollBar m_cVerticalScrollBar;
	float m_fVirtualYPos;
	geTreeNode* m_pCurrentSelectedNodePtr;	//
	MTreeViewObserver* m_pTVObserver;
	geVector2f m_fOffsetCacheForMouseMove;
	std::vector<geTreeNode*> m_cSelectedNodes;
};

class MTreeViewObserver
{
public:
	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)=0;
};

#endif