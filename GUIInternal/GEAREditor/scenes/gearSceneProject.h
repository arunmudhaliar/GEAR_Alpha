#ifndef GEARSCENEPROJECT_H
#define GEARSCENEPROJECT_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../SpriteManager/Sprite2Dx.h"
//#include "gearSceneFileView.h"

class gearSceneProject : public geWindow, public MTreeViewObserver
{
public:
	gearSceneProject();
	~gearSceneProject();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

	//void setFileViewScenePtr(gearSceneFileView* fileView)	{	m_pFileViewScenePtr=fileView;	
	void populateProjectView();
	geTreeNode* getSelectedNode()	{	return m_cAssetTreeView.getSelectedNode();	}

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

	virtual void onDragEnter(int x, int y);
	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
	virtual void onDragLeave();

private:

	void destroyTVUserData(geGUIBase* parent);

	geTreeView m_cAssetTreeView;

	Sprite2Dx m_cszSprites[5];
	//gearSceneFileView* m_pFileViewScenePtr;	//must not delete this pointer
};

#endif