#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../gui/geTextBox.h"
#include "settingsViews/geSettingsAndroid.h"
#include "settingsViews/geSettingsGlobal.h"

class gearSceneSettings : public geWindow, public MTreeViewObserver, public MGUIObserver
{
public:
	gearSceneSettings(geFontManager* fontmanager);
	~gearSceneSettings();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

private:
	Sprite2Dx spriteArray[10];

	geTreeView* settingsTreeView;

	geTreeNode* settingsAndroidParentTreeNode;
	geSettingsAndroid* androidSettings;

	geTreeNode* settingsGlobalParentTreeNode;
	geSettingsGlobal* globalSettings;
};