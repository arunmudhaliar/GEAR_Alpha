#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../SpriteManager/Sprite2Dx.h"
//#include "gearSceneFileView.h"

class gearSceneProject : public geWindow, public MTreeViewObserver
{
public:
	gearSceneProject(geFontManager* fontmanager);
	~gearSceneProject();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

	//void setFileViewScenePtr(gearSceneFileView* fileView)	{	m_pFileViewScenePtr=fileView;	
	void populateProjectView();
	geTreeNode* getSelectedNode()	{	return assetTreeView->getSelectedNode();	}

protected:
	virtual void onCreate(float cx, float cy);
	virtual void onDraw();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

//#if !defined(__APPLE__) //disable Drag-Drop
	virtual void onDragEnter(int x, int y);
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
	virtual void onDragLeave();
//#endif
    
private:
    static int traverse_project_directory(rendererGL10* renderer, const char *dirname, geTreeNode* parentNode, Sprite2Dx* spriteArray, geFontManager* fontmanager);
	void destroyTVUserData(geGUIBase* parent);

	geTreeView* assetTreeView;
	Sprite2Dx spriteArray[5];
	//gearSceneFileView* m_pFileViewScenePtr;	//must not delete this pointer
};