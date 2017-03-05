#include "gearSceneAnimationEditor.h"
#include "../EditorApp.h"
#include "../gui/geToolBarSeperator.h"
#include "../gui/geToolBarDropMenu.h"
#include "../../../GEAREngine/src/animation/gxColorAnimationTrack.h"

gearSceneAnimationEditor::gearSceneAnimationEditor(geFontManager* fontmanager):
geWindow("Animation Editor", fontmanager)
{
    selectedObject = nullptr;
    animationsTreeView = new geTreeView(fontmanager);
    
    memset(positionNode, 0, sizeof(positionNode));
    memset(rotationNode, 0, sizeof(rotationNode));
    memset(scaleNode, 0, sizeof(scaleNode));
    
    addKeyFrameBtn = nullptr;
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

    //Toolbar buttons
    auto actionMenu=new geToolBarDropMenu(rendererGUI, "Action", getToolBar(), fontManagerGUI);
    actionMenu->setGUIObserver(this);
    actionMenu->appendMenuItem("Create Property", 0x00002000);
    getToolBar()->appendToolBarControl(actionMenu);

    auto seperator = new geToolBarSeperator(rendererGUI, getToolBar(), 40, fontManagerGUI);
    getToolBar()->appendToolBarControl(seperator);

    auto recordBtn=new geToolBarButton(rendererGUI, "R", getToolBar(), fontManagerGUI);
    recordBtn->loadImage("res//icons16x16.png", 6, 216);
    recordBtn->setGUIObserver(this);
    getToolBar()->appendToolBarControl(recordBtn);
    
    auto playBtn=new geToolBarButton(rendererGUI, "P", getToolBar(), fontManagerGUI);
    playBtn->loadImage("res//icons16x16.png", 27, 216);
    playBtn->setGUIObserver(this);
    getToolBar()->appendToolBarControl(playBtn);
    
    auto previousFrameBtn=new geToolBarButton(rendererGUI, "PF", getToolBar(), fontManagerGUI);
    previousFrameBtn->loadImage("res//icons16x16.png", 66, 216);
    previousFrameBtn->setGUIObserver(this);
    getToolBar()->appendToolBarControl(previousFrameBtn);
    
    auto nextFrameBtn=new geToolBarButton(rendererGUI, "NF", getToolBar(), fontManagerGUI);
    nextFrameBtn->loadImage("res//icons16x16.png", 47, 216);
    nextFrameBtn->setGUIObserver(this);
    getToolBar()->appendToolBarControl(nextFrameBtn);
    
    seperator = new geToolBarSeperator(rendererGUI, getToolBar(), 5, fontManagerGUI);
    getToolBar()->appendToolBarControl(seperator);

    auto currentFrameTextBox=new geTextBox(fontManagerGUI);
    currentFrameTextBox->create(rendererGUI, getToolBar(), "0", 0, 1, 30, 13);
    currentFrameTextBox->setGUIObserver(this);
    currentFrameTextBox->setAcceptOnlyNumbers(true);
    getToolBar()->appendToolBarControl(currentFrameTextBox);

    seperator = new geToolBarSeperator(rendererGUI, getToolBar(), 50, fontManagerGUI);
    getToolBar()->appendToolBarControl(seperator);

    addKeyFrameBtn=new geToolBarButton(rendererGUI, "K", getToolBar(), fontManagerGUI);
    addKeyFrameBtn->loadImage("res//icons16x16.png", 362, 216);
    addKeyFrameBtn->setGUIObserver(this);
    getToolBar()->appendToolBarControl(addKeyFrameBtn);
    
    auto addEventBtn=new geToolBarButton(rendererGUI, "E", getToolBar(), fontManagerGUI);
    addEventBtn->loadImage("res//icons16x16.png", 280, 216);
    addEventBtn->setGUIObserver(this);
    getToolBar()->appendToolBarControl(addEventBtn);

    //
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

void gearSceneAnimationEditor::onButtonClicked(geGUIBase* btn)
{
    if(btn==addKeyFrameBtn)
    {
        if(addKeyFrameBtn->isButtonPressed())
        {
            addKeyFrameBtn->buttonNormal(true);
            
            if(!selectedObject)
                return;
            auto selectedTreeNode = animationsTreeView->getSelectedNode();
            if(!selectedTreeNode || treeNode_PropertyTypeMap.find(selectedTreeNode)==treeNode_PropertyTypeMap.end())
                return;
            auto animationController = selectedObject->getAnimationController();
            auto keyFrameAnimationSet = animationController->getAnimationSet("gxKeyFrameAnimationSet");
            if(!keyFrameAnimationSet)
                return;
            
            auto animationTrack = keyFrameAnimationSet->getAnimationTrack(selectedTreeNode->getName());
            if(!animationTrack)
                return;

            switch (animationTrack->getAnimationTrackType())
            {
                case 2:
                {
                    break;
                }
                case 3:
                {
                    
                    break;
                }
                default:
                    DEBUG_PRINT("Unknown key frame animation type");
                    break;
            }
        }
    }
}

void gearSceneAnimationEditor::onCommand(int cmd)
{
    switch(cmd)
    {
        case 0x00002000:
        {
            if(!selectedObject)
                return;
            
            auto selectedTreeNode = animationsTreeView->getSelectedNode();
            if(!selectedTreeNode || treeNode_PropertyTypeMap.find(selectedTreeNode)==treeNode_PropertyTypeMap.end())
                return;
            
            auto animationController = selectedObject->getAnimationController();
            std::string typeName = treeNode_PropertyTypeMap[selectedTreeNode];
            if(typeName.compare("MonoGEAR.gxColor")==0)
            {
                auto keyFrameAnimationSet = animationController->getAnimationSet("gxKeyFrameAnimationSet");
                if(keyFrameAnimationSet)
                {
                    //check if the animation track is there or not
                    auto colorAnimationTrack = keyFrameAnimationSet->getAnimationTrack(selectedTreeNode->getName());
                    if(!colorAnimationTrack)
                    {
                        colorAnimationTrack = gxColorAnimationTrack::create(selectedTreeNode->getName());
                        keyFrameAnimationSet->appendTrack(colorAnimationTrack);
                        REF_RELEASE(colorAnimationTrack);
                        graphControlView->setTrack(colorAnimationTrack);
                    }
                }
                else
                {
                    //create
                    keyFrameAnimationSet = gxAnimationSet::create("gxKeyFrameAnimationSet");
                    auto colorAnimationTrack = gxColorAnimationTrack::create(selectedTreeNode->getName());
                    keyFrameAnimationSet->appendTrack(colorAnimationTrack);
                    REF_RELEASE(colorAnimationTrack);
                    animationController->appendAnimationSet(keyFrameAnimationSet);
                    REF_RELEASE(keyFrameAnimationSet);
                    graphControlView->setTrack(colorAnimationTrack);
                    
                    EditorGEARApp::getScenePropertyEditor()->refreshProperties();
                }
            }
        }
            break;
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
    treeNode_PropertyTypeMap.clear();

    if(selectedObject==nullptr)
    {
        return;
    }
    
//    if(selectedObject==nullptr || !selectedObject->getAnimationTrack())
//    {
//        //graphControlView->setTrack(nullptr);
//        //return;
//    }
    
    auto track = selectedObject->getAnimationTrack();
    graphControlView->setTrack(track);
//    auto animationTrackNode = new geTreeNode(rendererGUI, animationsTreeView->getRoot(), track->getName(), &spriteArray[0], fontManagerGUI);
//    auto positionRootNode = new geTreeNode(rendererGUI, animationTrackNode, "Position", &spriteArray[0], fontManagerGUI);
//    auto rotationRootNode = new geTreeNode(rendererGUI, animationTrackNode, "Rotation", &spriteArray[0], fontManagerGUI);
//    auto scaleRootNode = new geTreeNode(rendererGUI, animationTrackNode, "Scale", &spriteArray[0], fontManagerGUI);
//    
//    positionNode[0] = new geTreeNode(rendererGUI, positionRootNode, "X", &spriteArray[0], fontManagerGUI);
//    positionNode[1] = new geTreeNode(rendererGUI, positionRootNode, "Y", &spriteArray[0], fontManagerGUI);
//    positionNode[2] = new geTreeNode(rendererGUI, positionRootNode, "Z", &spriteArray[0], fontManagerGUI);
//    
//    rotationNode[0] = new geTreeNode(rendererGUI, rotationRootNode, "X", &spriteArray[0], fontManagerGUI);
//    rotationNode[1] = new geTreeNode(rendererGUI, rotationRootNode, "Y", &spriteArray[0], fontManagerGUI);
//    rotationNode[2] = new geTreeNode(rendererGUI, rotationRootNode, "Z", &spriteArray[0], fontManagerGUI);
//
//    scaleNode[0] = new geTreeNode(rendererGUI, scaleRootNode, "X", &spriteArray[0], fontManagerGUI);
//    scaleNode[1] = new geTreeNode(rendererGUI, scaleRootNode, "Y", &spriteArray[0], fontManagerGUI);
//    scaleNode[2] = new geTreeNode(rendererGUI, scaleRootNode, "Z", &spriteArray[0], fontManagerGUI);
    
    auto propertiesNode = new geTreeNode(rendererGUI, animationsTreeView->getRoot(), object->getName().c_str(), &spriteArray[0], fontManagerGUI);
    
//    //mono fields
//    for(int x=0;x<monoScriptobject->getScriptPtr()->getMonoFieldCount();x++)
//    {
//        auto mvartypename = monoScriptobject->getScriptPtr()->getMonoFieldTypeName(x);
//        if(strncmp(mvartypename, "MonoGEAR.gxColor", strlen("MonoGEAR.gxColor"))==0)
//        {
//            stWindowColumnRow* row = windowColumnControl->addRow(monoScriptobject->getScriptPtr()->getMonoFieldName(x));
//            geColorControl* colorControl = new geColorControl(fontManagerGUI);
//            colorControl->create(rendererGUI, this, 100, 10);
//            colorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
//            colorControl->setGUIObserver(this);
//            windowColumnControl->addControl(row, colorControl, 16.0f);
//            lastControl=colorControl;
//        }
//        else
//        {
//            stWindowColumnRow* row = windowColumnControl->addRow(monoScriptobject->getScriptPtr()->getMonoFieldName(x));
//            geTextBox* variableeditbox = new geTextBox("", fontManagerGUI);
//            variableeditbox->create(rendererGUI, this, mvartypename, 0, 0);
//            windowColumnControl->addControl(row, variableeditbox, 16.0f);
//            lastControl=variableeditbox;
//        }
//    }

    
    for(auto monoInstance : object->getAttachedMonoScripts())
    {
        auto script = monoInstance->getScriptPtr();
        
        //mono properties
        auto properties = script->getMonoProperties();
        for(auto property : properties)
        {
            auto propertyName = script->getPropertyName(property);
            auto getType = script->getPropertyGetMethodType(property);
            auto setType = script->getPropertySetMethodType(property);
            const char* typeName = nullptr;
            if(getType && setType)
            {
                typeName = script->getMonoPropertyTypeName(getType);
            }
            
            if(getType && setType && strncmp(typeName, "MonoGEAR.gxColor", strlen("MonoGEAR.gxColor"))==0)
            {
                auto treeNode = new geTreeNode(rendererGUI, propertiesNode, propertyName, &spriteArray[0], fontManagerGUI);
                treeNode_PropertyTypeMap[treeNode] = typeName;
            }
        }
        
        //mono fields
        auto fields = script->getMonoFields();
        for(auto field : fields)
        {
            auto fieldTypeName = monoClassDef::getMonoFieldTypeName(field);
            auto fieldName = monoClassDef::getMonoFieldName(field);
            if(strncmp(fieldTypeName, "MonoGEAR.gxColor", strlen("MonoGEAR.gxColor"))==0)
            {
                auto fieldNode = new geTreeNode(rendererGUI, propertiesNode, fieldName, &spriteArray[0], fontManagerGUI);
                treeNode_PropertyTypeMap[fieldNode] = fieldTypeName;
            }
            else
            {
                auto fieldNode = new geTreeNode(rendererGUI, propertiesNode, fieldName, &spriteArray[0], fontManagerGUI);
                UNUSED(fieldNode);
            }
        }
    }
}
