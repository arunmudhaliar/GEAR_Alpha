#ifndef GEARSCENEFILEVIEW_H
#define GEARSCENEFILEVIEW_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../gui/geTextBox.h"
#include "../../../GEAREngine/src/core/gxMaterial.h"
#include "../../../GEAREngine/src/core/object3d.h"

class gearSceneFileView : public geWindow, public MTreeViewObserver, public MGUIObserver
{
public:
	gearSceneFileView();
	~gearSceneFileView();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

	void populateFiles(const char* dirPath);
	void populateFileView();

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

	virtual void onTextChange(geGUIBase* btn);
	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
private:
	void destroyTVUserData(geGUIBase* parent);
	void loadPreviewObjects();

	Sprite2Dx m_cszSprites[5];
	geTreeView m_cFileTreeView;
	geTextBox* m_pSerachStringTextBoxPtr;
	char m_szDirectoryPath[512];

	object3d* m_pPreviewObj_Cube;
	gxMaterial m_cPreviewMaterial;
};

#endif