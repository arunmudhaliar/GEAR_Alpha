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
	gearSceneFileView(geFontManager* fontManager);
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
//#if !defined(__APPLE__) //disable Drag-Drop
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
//#endif

private:
	void destroyTVUserData(geGUIBase* parent);
	void loadPreviewObjects();
	void read3dFile(gxFile& file, object3d* obj);
	void deleteAnmationFromObject3d(object3d* obj3d);
	static int find_files(rendererGL10* renderer, const char *dirname, const char* searchString, geTreeNode* parentNode, Sprite2Dx* spriteArray);

	void tryLoadTexturesFromObject3d(object3d* obj3d, const char* filepath);

	Sprite2Dx m_cszSprites[5];
	geTreeView* m_pFileTreeView;
	geTextBox* m_pSerachStringTextBoxPtr;
	char m_szDirectoryPath[512];

	object3d* m_pPreviewObj_Cube;
	gxMaterial m_cPreviewMaterial;
};

#endif