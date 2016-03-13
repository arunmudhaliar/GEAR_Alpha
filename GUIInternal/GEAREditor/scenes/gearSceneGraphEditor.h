#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../gui/geGraphControl.h"

class gearSceneGraphEditor : public geWindow
{
public:
	gearSceneGraphEditor(geFontManager* fontmanager);
	~gearSceneGraphEditor();


    geGraphControl* getGraphControl()   {   return graphControlView;    }
    
protected:
	virtual void onCreate(float cx, float cy);
    virtual void onSize(float cx, float cy, int flag);

	virtual void onDraw();

private:
    geGraphControl* graphControlView;
};