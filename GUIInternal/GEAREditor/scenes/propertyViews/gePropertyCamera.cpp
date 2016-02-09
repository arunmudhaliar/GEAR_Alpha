#include "gePropertyCamera.h"
#include "../../EditorApp.h"

gePropertyCamera::gePropertyCamera(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager):
	geTreeNode(renderer, parent, name, sprite, fontManager, 10)
{
	//camera type
	m_pCameraTypeToolBarDropMenuBtnPtr=new geToolBarDropMenu(rendererGUI, "CameraType", this, fontManager);
	m_pCameraTypeToolBarDropMenuBtnPtr->setGUIObserver(this);
	m_pCameraTypeToolBarDropMenuBtnPtr->setPos(10, 35);

	m_pCameraTypeToolBarDropMenuBtnPtr->appendMenuItem("Perspective", 0x00007000);
	m_pCameraTypeToolBarDropMenuBtnPtr->appendMenuItem("Orthographic", 0x00007001);

	//camera culling
	m_pCameraCullingToolBarDropMenuBtnPtr=new geToolBarDropMenu(rendererGUI, "Camera Culling", this, fontManager);
	m_pCameraCullingToolBarDropMenuBtnPtr->setGUIObserver(this);
	m_pCameraCullingToolBarDropMenuBtnPtr->setPos(10, 65);

	//attenuations
	m_pHorizontalSlider_FOV = new geHorizontalSlider(fontManager);
	m_pHorizontalSlider_FOV->create(rendererGUI, this, "slider", 10, 65, 130);
	m_pHorizontalSlider_FOV->setSliderValue(1.0f);
	m_pHorizontalSlider_FOV->setGUIObserver(this);

	m_pHorizontalSlider_Near = new geHorizontalSlider(fontManager);
	m_pHorizontalSlider_Near->create(rendererGUI, this, "slider", 10, 85, 130);
	m_pHorizontalSlider_Near->setSliderValue(1.0f);
	m_pHorizontalSlider_Near->setGUIObserver(this);

	m_pHorizontalSlider_Far = new geHorizontalSlider(fontManager);
	m_pHorizontalSlider_Far->create(rendererGUI, this, "slider", 10, 105, 130);
	m_pHorizontalSlider_Far->setSliderValue(1.0f);
	m_pHorizontalSlider_Far->setGUIObserver(this);

	m_pButtonApplyMainCamera = new geButton("", fontManager);
	m_pButtonApplyMainCamera->create(renderer, this, "Set as main camera", 15, 10);
	m_pButtonApplyMainCamera->setGUIObserver(this);

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(fontManager);
	pWindowColumn->create(rendererGUI, this, 10, 300.0f, 10.0f, 0.4f);
	stWindowColumnRow* row = pWindowColumn->addRow("Type");
	pWindowColumn->addControl(row, m_pCameraTypeToolBarDropMenuBtnPtr, 30.0f);
	row = pWindowColumn->addRow("FOV");
	pWindowColumn->addControl(row, m_pHorizontalSlider_FOV, 20.0f);
	row = pWindowColumn->addRow("Near Plane");
	pWindowColumn->addControl(row, m_pHorizontalSlider_Near, 20.0f);
	row = pWindowColumn->addRow("Far Plane");
	pWindowColumn->addControl(row, m_pHorizontalSlider_Far, 20.0f);
	row = pWindowColumn->addRow("Camera Culling");
	pWindowColumn->addControl(row, m_pCameraCullingToolBarDropMenuBtnPtr, 25.0f);
	row = pWindowColumn->addRow("");
	pWindowColumn->addControl(row, m_pButtonApplyMainCamera, 20.0f);	//


	m_pCameraPtr=NULL;

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();

	setSize(m_cSize.x, m_pButtonApplyMainCamera->getPos().y+m_pButtonApplyMainCamera->getSize().y+10);
}

gePropertyCamera::~gePropertyCamera()
{

}

//#if !defined(__APPLE__) //disable Drag-Drop
void gePropertyCamera::onDragDrop(int x, int y, MDropData* dropObject)
{

}
//#endif

void gePropertyCamera::drawNode()
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

	//m_pWindowColumn->draw();

	if(sprite)
		sprite->draw();
}

void gePropertyCamera::populatePropertyOfCamera(object3d* obj)
{
	m_pCameraPtr = (Camera*)obj;

	m_pHorizontalSlider_FOV->setSliderValue(m_pCameraPtr->getFOV()/120.0f, false);
	m_pHorizontalSlider_Near->setSliderValue(m_pCameraPtr->getNear()/100.0f, false);
	m_pHorizontalSlider_Far->setSliderValue(m_pCameraPtr->getFar()/10000.0f, false);

	if(m_pCameraPtr->getProjectionType()==Camera::PERSPECTIVE_PROJECTION)
		m_pCameraTypeToolBarDropMenuBtnPtr->setMenuItem("Perspective");
	else if(m_pCameraPtr->getProjectionType()==Camera::ORTHOGRAPHIC_PROJECTION)
		m_pCameraTypeToolBarDropMenuBtnPtr->setMenuItem("Orthographic");

	//culling mask
	m_pCameraCullingToolBarDropMenuBtnPtr->clearMenu();
	LayerManager* layerManager = monoWrapper::mono_engine_getWorld(0)->getLayerManager();
	for(int x=0;x<MAX_LAYER;x++)
	{
		Layer* layer = layerManager->getLayer(x);
		m_pCameraCullingToolBarDropMenuBtnPtr->appendMenuItem(layer->getLayerName(), 0x00003000+x, NULL, false, m_pCameraPtr->isLayerCullingMask((1<<x)));
	}

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
		monoWrapper::mono_engine_getWorld(0)->setActiveCamera(m_pCameraPtr);
	}
}

void gePropertyCamera::onCommand(int cmd)
{
	if(cmd==0x00007000)
	{
		m_pCameraPtr->setType(Camera::PERSPECTIVE_PROJECTION);
		m_pCameraTypeToolBarDropMenuBtnPtr->setMenuItem(cmd);
	}
	else if(cmd==0x00007001)
	{
		m_pCameraPtr->setType(Camera::ORTHOGRAPHIC_PROJECTION);
		m_pCameraTypeToolBarDropMenuBtnPtr->setMenuItem(cmd);
	}
	else if(cmd>=0x00003000 && cmd<=(0x00003000+32))
	{
		int layerid=(cmd-0x00003000);
		if(m_pCameraPtr->isLayerCullingMask((1<<layerid)))
		{
			m_pCameraPtr->resetLayerCullingMask((1<<layerid));
			m_pCameraCullingToolBarDropMenuBtnPtr->checkMenuItem(cmd, false);
		}
		else
		{
			m_pCameraPtr->setLayerCullingMask((1<<layerid));
			m_pCameraCullingToolBarDropMenuBtnPtr->checkMenuItem(cmd, true);
		}
	}
}