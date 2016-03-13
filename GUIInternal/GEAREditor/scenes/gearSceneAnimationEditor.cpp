#include "gearSceneAnimationEditor.h"
#include "../EditorApp.h"

gearSceneAnimationEditor::gearSceneAnimationEditor(geFontManager* fontmanager):
geWindow("Animation Editor", fontmanager)
{
    selectedObject = nullptr;
    animationsTreeView = new geTreeView(fontmanager);
    
    memset(positionNode, 0, sizeof(positionNode));
    memset(rotationNode, 0, sizeof(rotationNode));
    memset(scaleNode, 0, sizeof(scaleNode));
}

gearSceneAnimationEditor::~gearSceneAnimationEditor()
{   
    GE_DELETE(animationsTreeView);
}

void gearSceneAnimationEditor::onCreate(float cx, float cy)
{
	animationsTreeView->create(rendererGUI, this, "animationeditorTV", this);
    animationsTreeView->setMouseBoundCheck(true);
    
    spriteArray[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
    spriteArray[0].setClip(26, 298, 16, 16);

}

void gearSceneAnimationEditor::onSize(float cx, float cy, int flag)
{
    geWindow::onSize(cx, cy, flag);
}

void gearSceneAnimationEditor::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	animationsTreeView->draw();
}

void gearSceneAnimationEditor::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
    int index=0;
    for (auto node : positionNode)
    {
        if (node==tvnode)
        {
            graphControlView->showPositionValues(index);
            return;
        }
        index++;
    }
    
    index=0;
    for (auto node : rotationNode)
    {
        if (node==tvnode)
        {
            graphControlView->showRotationValues(index);
            return;
        }
        index++;
    }
    
    index=0;
    for (auto node : scaleNode)
    {
        if (node==tvnode)
        {
            graphControlView->showScaleValues(index);
            return;
        }
        index++;
    }
}

void gearSceneAnimationEditor::populatePropertyOfObject3d(object3d* object)
{
    if(selectedObject==object)
    {
        return;
    }
    
    selectedObject = object;
    animationsTreeView->clearAndDestroyAll();

    if(selectedObject==nullptr || !selectedObject->getAnimationTrack())
        return;
    
    auto track = selectedObject->getAnimationTrack();
    graphControlView->setTrack(track);
    auto animationTrackNode = new geTreeNode(rendererGUI, animationsTreeView->getRoot(), track->getName(), &spriteArray[0], fontManagerGUI);
    auto positionRootNode = new geTreeNode(rendererGUI, animationTrackNode, "Position", &spriteArray[0], fontManagerGUI);
    auto rotationRootNode = new geTreeNode(rendererGUI, animationTrackNode, "Rotation", &spriteArray[0], fontManagerGUI);
    auto scaleRootNode = new geTreeNode(rendererGUI, animationTrackNode, "Scale", &spriteArray[0], fontManagerGUI);
    
    positionNode[0] = new geTreeNode(rendererGUI, positionRootNode, "X", &spriteArray[0], fontManagerGUI);
    positionNode[1] = new geTreeNode(rendererGUI, positionRootNode, "Y", &spriteArray[0], fontManagerGUI);
    positionNode[2] = new geTreeNode(rendererGUI, positionRootNode, "Z", &spriteArray[0], fontManagerGUI);
    
    rotationNode[0] = new geTreeNode(rendererGUI, rotationRootNode, "X", &spriteArray[0], fontManagerGUI);
    rotationNode[1] = new geTreeNode(rendererGUI, rotationRootNode, "Y", &spriteArray[0], fontManagerGUI);
    rotationNode[2] = new geTreeNode(rendererGUI, rotationRootNode, "Z", &spriteArray[0], fontManagerGUI);

    scaleNode[0] = new geTreeNode(rendererGUI, scaleRootNode, "X", &spriteArray[0], fontManagerGUI);
    scaleNode[1] = new geTreeNode(rendererGUI, scaleRootNode, "Y", &spriteArray[0], fontManagerGUI);
    scaleNode[2] = new geTreeNode(rendererGUI, scaleRootNode, "Z", &spriteArray[0], fontManagerGUI);
}
