#include "gePropertyOctree.h"
#include "../../EditorApp.h"

gePropertyOctree::gePropertyOctree(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager):
	geTreeNode(renderer, parent, name, sprite, fontManager, 10)
{
	EditorGEARApp::setPropertyOctree(this);

	setSize(m_cSize.x, 80.0f);

	//package name
	m_pNoOfTransformObjectPerNode = new geHorizontalSlider(m_pFontManagerPtr);
	m_pNoOfTransformObjectPerNode->create(m_pRenderer, this, "slider", 10, 65, 130);
	m_pNoOfTransformObjectPerNode->setSliderValue(0.5f);
	m_pNoOfTransformObjectPerNode->setGUIObserver(this);

	m_pNoOfLevels = new geHorizontalSlider(m_pFontManagerPtr);
	m_pNoOfLevels->create(m_pRenderer, this, "slider", 10, 65, 130);
	m_pNoOfLevels->setSliderValue(0.2f);
	m_pNoOfLevels->setGUIObserver(this);


	m_pButtonApply = new geButton("", fontManager);
	m_pButtonApply->create(renderer, this, "Apply", 15, 10);
	m_pButtonApply->setGUIObserver(this);

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(m_pFontManagerPtr);
	pWindowColumn->create(m_pRenderer, this, 20, 300.0f, 10.0f, 0.44f);
	m_pRows[0] = pWindowColumn->addRow("No of objects per node: 6");
	pWindowColumn->addControl(m_pRows[0], m_pNoOfTransformObjectPerNode, 15.0f);
	m_pRows[1] = pWindowColumn->addRow("Octree max depth: 1");
	pWindowColumn->addControl(m_pRows[1], m_pNoOfLevels, 15.0f);
	m_pRows[2] = pWindowColumn->addRow("");
	pWindowColumn->addControl(m_pRows[2], m_pButtonApply);
	//

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyOctree::~gePropertyOctree()
{

}

//#if !defined(__APPLE__) //disable Drag-Drop
void gePropertyOctree::onDragDrop(int x, int y, MDropData* dropObject)
{

}
//#endif

void gePropertyOctree::drawNode()
{
	drawRect(&m_cVBClientArea);

	if(m_vControls.size() && m_bHaveAtleastOneTreeNodeChild)
	{
		if(m_bNodeOpen)
			drawTriangle(&m_cVBLayoutToggleButtonLine[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		else
			drawTriangle(&m_cVBLayoutToggleButtonLine[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
	}

	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	//m_pWindowColumn->draw();

	if(m_pSprite)
		m_pSprite->draw();
}

//void gePropertyOctree::populatePropertyOfCamera(object3d* obj)
//{
//}

void gePropertyOctree::onSliderChange(geGUIBase* slider)
{
	char buffer[256];
	if(m_pNoOfTransformObjectPerNode==slider)
	{
		int value=4+(int)(m_pNoOfTransformObjectPerNode->getSliderValue()*4);
		sprintf(buffer, "No of objects per node: %d", value);
		m_pRows[0]->setName(buffer);
	}
	else if(m_pNoOfLevels==slider)
	{
		int value=(int)(m_pNoOfLevels->getSliderValue()*6);
		sprintf(buffer, "Octree max depth: %d", value);
		m_pRows[1]->setName(buffer);
	}
}

void gePropertyOctree::onButtonClicked(geGUIBase* btn)
{
	if(btn==m_pButtonApply && m_pButtonApply->isButtonPressed())
	{
		EditorGEARApp::getSceneHierarchy()->recreateOctree();
	}
}

void gePropertyOctree::getOctreeVars(int& nTransformPerNodes, int& nLevels)
{
	nTransformPerNodes=4+(int)(m_pNoOfTransformObjectPerNode->getSliderValue()*4);
	nLevels=(int)(m_pNoOfLevels->getSliderValue()*6);
}
