#include "gePropertyOctree.h"
#include "../../EditorApp.h"

gePropertyOctree::gePropertyOctree(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager):
	geTreeNode(renderer, parent, name, sprite, fontManager, 10)
{
	EditorGEARApp::setPropertyOctree(this);

	setSize(m_cSize.x, 80.0f);

	//package name
	noOfTransformObjectPerNodeHorizontalSlider = new geHorizontalSlider(fontManagerGUI);
	noOfTransformObjectPerNodeHorizontalSlider->create(rendererGUI, this, "slider", 10, 65, 130);
	noOfTransformObjectPerNodeHorizontalSlider->setSliderValue(0.5f);
	noOfTransformObjectPerNodeHorizontalSlider->setGUIObserver(this);

	noOfLevelsHorizontalSlider = new geHorizontalSlider(fontManagerGUI);
	noOfLevelsHorizontalSlider->create(rendererGUI, this, "slider", 10, 65, 130);
	noOfLevelsHorizontalSlider->setSliderValue(0.2f);
	noOfLevelsHorizontalSlider->setGUIObserver(this);


	applyButton = new geButton("", fontManager);
	applyButton->create(renderer, this, "Apply", 15, 10);
	applyButton->setGUIObserver(this);

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(fontManagerGUI);
	pWindowColumn->create(rendererGUI, this, 20, 300.0f, 10.0f, 0.44f);
	windowColumnRowArray[0] = pWindowColumn->addRow("No of objects per node: 6");
	pWindowColumn->addControl(windowColumnRowArray[0], noOfTransformObjectPerNodeHorizontalSlider, 15.0f);
	windowColumnRowArray[1] = pWindowColumn->addRow("Octree max depth: 1");
	pWindowColumn->addControl(windowColumnRowArray[1], noOfLevelsHorizontalSlider, 15.0f);
	windowColumnRowArray[2] = pWindowColumn->addRow("");
	pWindowColumn->addControl(windowColumnRowArray[2], applyButton);
	//

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyOctree::~gePropertyOctree()
{

}

//#if !defined(GEAR_APPLE) //disable Drag-Drop
void gePropertyOctree::onDragDrop(int x, int y, MDropData* dropObject)
{

}
//#endif

void gePropertyOctree::drawNode()
{
	drawRect(&vertexBufferClientArea);

	if(childControlList.size() && hasAtleastOneTreeNodeChild)
	{
		if(is_OpenNode)
			drawTriangle(&vertexBufferToggleButtonArray[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		else
			drawTriangle(&vertexBufferToggleButtonArray[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
	}

	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	//windowColumnControl->draw();

	if(sprite)
		sprite->draw();
}

//void gePropertyOctree::populatePropertyOfCamera(object3d* obj)
//{
//}

void gePropertyOctree::onSliderChange(geGUIBase* slider)
{
	char buffer[256];
	if(noOfTransformObjectPerNodeHorizontalSlider==slider)
	{
		int value=4+(int)(noOfTransformObjectPerNodeHorizontalSlider->getSliderValue()*4);
		sprintf(buffer, "No of objects per node: %d", value);
		windowColumnRowArray[0]->setName(buffer);
	}
	else if(noOfLevelsHorizontalSlider==slider)
	{
		int value=(int)(noOfLevelsHorizontalSlider->getSliderValue()*6);
		sprintf(buffer, "Octree max depth: %d", value);
		windowColumnRowArray[1]->setName(buffer);
	}
}

void gePropertyOctree::onButtonClicked(geGUIBase* btn)
{
	if(btn==applyButton && applyButton->isButtonPressed())
	{
		EditorGEARApp::getSceneHierarchy()->recreateOctree();
	}
}

void gePropertyOctree::getOctreeVars(int& nTransformPerNodes, int& nLevels)
{
	nTransformPerNodes=4+(int)(noOfTransformObjectPerNodeHorizontalSlider->getSliderValue()*4);
	nLevels=(int)(noOfLevelsHorizontalSlider->getSliderValue()*6);
}
