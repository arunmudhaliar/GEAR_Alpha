#ifndef GEARSCENECONSOLE_H
#define GEARSCENECONSOLE_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../gui/geTextBox.h"
#include "../../../GEAREngine/src/core/gxMaterial.h"
#include "../../../GEAREngine/src/core/object3d.h"
#include <time.h>

class gearSceneConsole : public geWindow, public MTreeViewObserver, public MGUIObserver
{
public:
	gearSceneConsole();
	~gearSceneConsole();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);
	void onButtonClicked(geGUIBase* btn);

	void appendConsoleRunRootNode();
	void appendConsoleMsg(const char* msg);
	void clearConsoleCurrentRun();
	void clearConsoleAllRun();

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

private:

	unsigned long m_uCurrentRunElapsedTime;
	geToolBarButton* m_pClearBtn;
	geToolBarButton* m_pClearAllBtn;
	geTreeView m_cConsoleTreeView;
	geTreeNode* m_pCurrentBuildRootNodePtr;
};

#endif