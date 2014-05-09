#include "gePropertyCamera.h"
#include "../../EditorApp.h"

gePropertyCamera::gePropertyCamera(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	geTreeNode(renderer, parent, name, sprite, 10)
{
	setSize(m_cSize.x, 130.0f);

	//diffuse
	m_pCameraTypeToolBarDropMenuBtnPtr=new geToolBarDropMenu(m_pRenderer, "CameraType", this);
	m_pCameraTypeToolBarDropMenuBtnPtr->setGUIObserver(this);
	m_pCameraTypeToolBarDropMenuBtnPtr->setPos(10, 35);

	m_pCameraTypeToolBarDropMenuBtnPtr->appendMenuItem("Perspective", 0x00007000);
	m_pCameraTypeToolBarDropMenuBtnPtr->appendMenuItem("Orthographic", 0x00007001);


	//attenuations
	m_pHorizontalSlider_FOV = new geHorizontalSlider();
	m_pHorizontalSlider_FOV->create(m_pRenderer, this, "slider", 10, 65, 130);
	m_pHorizontalSlider_FOV->setSliderValue(1.0f);
	m_pHorizontalSlider_FOV->setGUIObserver(this);

	m_pHorizontalSlider_Near = new geHorizontalSlider();
	m_pHorizontalSlider_Near->create(m_pRenderer, this, "slider", 10, 85, 130);
	m_pHorizontalSlider_Near->setSliderValue(1.0f);
	m_pHorizontalSlider_Near->setGUIObserver(this);

	m_pHorizontalSlider_Far = new geHorizontalSlider();
	m_pHorizontalSlider_Far->create(m_pRenderer, this, "slider", 10, 105, 130);
	m_pHorizontalSlider_Far->setSliderValue(1.0f);
	m_pHorizontalSlider_Far->setGUIObserver(this);

	m_pButtonApplyMainCamera = new geButton("");
	m_pButtonApplyMainCamera->create(renderer, this, "Set as main camera", 15, 10);
	m_pButtonApplyMainCamera->setGUIObserver(this);

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn();
	pWindowColumn->create(m_pRenderer, this, 10, 300.0f, 10.0f, 0.4f);
	stWindowColumnRow* row = pWindowColumn->addRow("Type");
	pWindowColumn->addControl(row, m_pCameraTypeToolBarDropMenuBtnPtr, 30.0f);
	row = pWindowColumn->addRow("FOV");
	pWindowColumn->addControl(row, m_pHorizontalSlider_FOV, 20.0f);
	row = pWindowColumn->addRow("Near Plane");
	pWindowColumn->addControl(row, m_pHorizontalSlider_Near, 20.0f);
	row = pWindowColumn->addRow("Far Plane");
	pWindowColumn->addControl(row, m_pHorizontalSlider_Far, 20.0f);
	row = pWindowColumn->addRow("");
	pWindowColumn->addControl(row, m_pButtonApplyMainCamera, 20.0f);
	//


	m_pCameraPtr=NULL;

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyCamera::~gePropertyCamera()
{

}

void gePropertyCamera::onDragDrop(int x, int y, MDataObject* dropObject)
{

}

void gePropertyCamera::drawNode()
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

void gePropertyCamera::populatePropertyOfCamera(object3d* obj)
{
	m_pCameraPtr = (gxCamera*)obj;

	m_pHorizontalSlider_FOV->setSliderValue(m_pCameraPtr->getFOV()/120.0f, false);
	m_pHorizontalSlider_Near->setSliderValue(m_pCameraPtr->getNear()/100.0f, false);
	m_pHorizontalSlider_Far->setSliderValue(m_pCameraPtr->getFar()/10000.0f, false);

	if(m_pCameraPtr->getProjectionType()==gxCamera::PERSPECTIVE_PROJECTION)
		m_pCameraTypeToolBarDropMenuBtnPtr->setMenuItem("Perspective");
	else if(m_pCameraPtr->getProjectionType()==gxCamera::ORTHOGRAPHIC_PROJECTION)
		m_pCameraTypeToolBarDropMenuBtnPtr->setMenuItem("Orthographic");
}

void gePropertyCamera::onTextChange(geGUIBase* textbox)
{
}

void gePropertyCamera::onColorChange(geGUIBase* colorControl)
{
}

void gePropertyCamera::onSliderChange(geGUIBase* slider)
{
	if(m_pHorizontalSlider_FOV==slider)
	{
		m_pCameraPtr->setFOV(m_pHorizontalSlider_FOV->getSliderValue()*120.0f);
	}
	else if(m_pHorizontalSlider_Near==slider)
	{
		m_pCameraPtr->setNear(m_pHorizontalSlider_Near->getSliderValue()*100.0f);
	}
	else if(m_pHorizontalSlider_Far==slider)
	{
		m_pCameraPtr->setFar(m_pHorizontalSlider_Far->getSliderValue()*10000.0f);
	}
}

void gePropertyCamera::onButtonClicked(geGUIBase* btn)
{
	if(btn==m_pButtonApplyMainCamera && m_pButtonApplyMainCamera->isButtonPressed())
	{
		monoWrapper::mono_engine_getWorld(0)->getActiveCamera()->setCamera(m_pCameraPtr);
	}
}

void gePropertyCamera::onCommand(int cmd)
{
	if(cmd==0x00007000)
	{
		m_pCameraPtr->setType(gxCamera::PERSPECTIVE_PROJECTION);
		m_pCameraTypeToolBarDropMenuBtnPtr->setMenuItem(cmd);
	}
	else if(cmd==0x00007001)
	{
		m_pCameraPtr->setType(gxCamera::ORTHOGRAPHIC_PROJECTION);
		m_pCameraTypeToolBarDropMenuBtnPtr->setMenuItem(cmd);
	}
}