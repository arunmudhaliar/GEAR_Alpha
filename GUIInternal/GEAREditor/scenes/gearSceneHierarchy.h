#ifndef GEARSCENEHIERARCHY_H
#define GEARSCENEHIERARCHY_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../mono/src/monoWrapper.h"
#include "../gui/geToolBarDropMenu.h"

class gearSceneHierarchy : public geWindow, public MEngineObserver, public MObject3dObserver, public MTreeViewObserver, public MGUIObserver
{
public:
	gearSceneHierarchy();
	~gearSceneHierarchy();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);
	//void setSceneWorldEditor(gearSceneWorldEditor* pSceneWorldEditorPtr	)	{	m_pSceneWorldEditorPtr=pSceneWorldEditorPtr;	}

	//engine observer
	virtual void onAppendToWorld(gxWorld* world, object3d* obj);
	virtual void onRemoveFromWorld(gxWorld* world, object3d* obj);

	//object3d observer
	virtual void onObject3dChildAppend(object3d* child);
	virtual void onObject3dChildRemove(object3d* child);

	//gui observer
	void onButtonClicked(geGUIBase* btn);
	void onSliderChange(geGUIBase* slider);

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual bool onMouseMove(float x, float y, int flag);

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
	geToolBarDropMenu* m_pCreateToolBarDropMenuBtnPtr;
//	gearSceneWorldEditor* m_pSceneWorldEditorPtr;	//must not delete this pointer
	//geButton m_cButton;
	//gePushButton m_cPushButton;
};

#endif