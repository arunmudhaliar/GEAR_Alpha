#ifndef GEARSCENEHIERARCHY_H
#define GEARSCENEHIERARCHY_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../mono/src/monoWrapper.h"
//#include "gearSceneWorldEditor.h"

class gearSceneHierarchy : public geWindow, public MTreeViewObserver
{
public:
	gearSceneHierarchy();
	~gearSceneHierarchy();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);
	//void setSceneWorldEditor(gearSceneWorldEditor* pSceneWorldEditorPtr	)	{	m_pSceneWorldEditorPtr=pSceneWorldEditorPtr;	}

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual void onMouseMove(float x, float y, int flag);

	virtual void onDragEnter(int x, int y);
	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
	virtual void onDragLeave();

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

private:
	void destroyTVUserData(geGUIBase* parent);
	void createTVNode(geTreeNode* parentNode, object3d* obj, const char* filename);

	geTreeView m_cGameObjectsTreeView;
	Sprite2Dx m_cszSprites[5];
//	gearSceneWorldEditor* m_pSceneWorldEditorPtr;	//must not delete this pointer
	//geButton m_cButton;
	//gePushButton m_cPushButton;
};

#endif