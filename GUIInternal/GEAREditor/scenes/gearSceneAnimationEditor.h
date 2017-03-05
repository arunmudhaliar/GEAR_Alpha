#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "gearSceneGraphEditor.h"

class gearSceneAnimationEditor : public geWindow, public MTreeViewObserver, public MGUIObserver
{
public:

	gearSceneAnimationEditor(geFontManager* fontmanager);
	~gearSceneAnimationEditor();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

    void populatePropertyOfObject3d(object3d* object);
    
    void setGraphControlView(geGraphControl* graphView) {   graphControlView=graphView; }
protected:
	virtual void onCreate(float cx, float cy);
    virtual void onSize(float cx, float cy, int flag);

	virtual void onDraw();
    virtual void onCommand(int cmd);
    virtual void onButtonClicked(geGUIBase* btn);
    
private:
    object3d* selectedObject;
	geTreeView* animationsTreeView;

    geGraphControl* graphControlView;
    Sprite2Dx spriteArray[1];
    
    geTreeNode* positionNode[3];
    geTreeNode* rotationNode[3];
    geTreeNode* scaleNode[3];
    std::map<geTreeNode*, std::string> treeNode_PropertyTypeMap;
    
    //gui
    geToolBarButton* addKeyFrameBtn;
};
