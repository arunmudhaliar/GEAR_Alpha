#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../../GEAREngine/src/mono/src/monoWrapper.h"
#include "../gui/geToolBarDropMenu.h"

class gearSceneHierarchy : public geWindow, public MEngineObserver, public MObject3dObserver, public MTreeViewObserver, public MGUIObserver
{
public:
	gearSceneHierarchy(geFontManager* fontManager);
	~gearSceneHierarchy();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);
	//void setSceneWorldEditor(gearSceneWorldEditor* pSceneWorldEditorPtr	)	{	m_pSceneWorldEditorPtr=pSceneWorldEditorPtr;	}

	//engine observer
	virtual void onAppendToWorld(object3d* world, object3d* obj);
	virtual void onRemoveFromWorld(object3d* world, object3d* obj);
	virtual void onConsoleLogFromMono(const char* msg, int msgtype);

	//object3d observer
	virtual void onObject3dChildAppend(object3d* child);
	virtual void onObject3dChildRemove(object3d* child);
	virtual void onObject3dDestroy(object3d* obj);

	//gui observer
	void onButtonClicked(geGUIBase* btn);
	void onSliderChange(geGUIBase* slider);

	geGUIBase* getSelectedTreeNode();

	void recreateOctree();

	void selectObject3dInTreeView(object3d* objtoselect);

	void clearHierarchy();
	bool saveCurrentScene(const std::string& sceneFilePath);
    bool loadScene(const std::string& sceneFilePath);

protected:
	virtual void onCreate(float cx, float cy);
	virtual void onDraw();

	virtual bool onMouseMove(float x, float y, int flag);

//#if !defined(GEAR_APPLE) //disable Drag-Drop
	virtual void onDragEnter(int x, int y);
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
	virtual void onDragLeave();
//#endif
    
	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

	virtual void onCommand(int cmd);

private:
	bool selectObject3dInTreeView(geTreeNode* node, object3d* objtoselect);

	void destroyTVUserData(geGUIBase* parent);
	void createTVNode(geTreeNode* parentNode, object3d* obj, const char* filename);

	geTreeView* gameObjectTreeView;
	Sprite2Dx spriteArray[5];
	geToolBarDropMenu* createToolBarDropMenuButton;
	geToolBarButton* toolBarClearButton;
};