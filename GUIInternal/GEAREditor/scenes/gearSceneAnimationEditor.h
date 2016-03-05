#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "gearSceneGraphEditor.h"

class gearSceneAnimationEditor : public geWindow, public MTreeViewObserver
{
public:

	gearSceneAnimationEditor(geFontManager* fontmanager);
	~gearSceneAnimationEditor();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

    void populatePropertyOfObject3d(object3d* object);
    
    void setGraphControlView(geGraphControl* graphView) {   graphControlView=graphView; }
protected:
	virtual void onCreate();
    virtual void onSize(float cx, float cy, int flag);

	virtual void onDraw();

private:
    object3d* selectedObject;
	geTreeView* animationsTreeView;

    geGraphControl* graphControlView;
    Sprite2Dx spriteArray[1];
    
    geTreeNode* positionNode[3];
    geTreeNode* rotationNode[3];
    geTreeNode* scaleNode[3];
};