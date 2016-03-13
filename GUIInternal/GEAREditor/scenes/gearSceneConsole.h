#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../gui/geTextBox.h"
#include <time.h>

class gearSceneConsole : public geWindow, public MTreeViewObserver, public MGUIObserver
{
public:
	gearSceneConsole(geFontManager* fontManager);
	~gearSceneConsole();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);
	void onButtonClicked(geGUIBase* btn);

	void appendConsoleRunRootNode();
	void appendConsoleMsg(const char* msg, int msgtype);
	void clearConsoleCurrentRun();
	void clearConsoleAllRun();

protected:
	virtual void onCreate(float cx, float cy);
	virtual void onDraw();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

private:
	Sprite2Dx spriteArray[10];
	unsigned long currentRunElapsedTime;
	geToolBarButton* toolBarClearButton;
	geToolBarButton* toolBarClearAllButton;
	geToolBarButton* toolBarDontLogButton;
	geTreeView* consoleTreeView;
	geTreeNode* currentBuildRootTreeNode;
};