#include "gePropertyCamera.h"
#include "../../EditorApp.h"

gePropertyCamera::gePropertyCamera(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager):
	geTreeNode(renderer, parent, name, sprite, fontManager, 10)
{
	//camera type
	cameraTypeToolBarDropMenuButton=new geToolBarDropMenu(rendererGUI, "CameraType", this, fontManager);
	cameraTypeToolBarDropMenuButton->setGUIObserver(this);
	cameraTypeToolBarDropMenuButton->setPos(10, 35);

	cameraTypeToolBarDropMenuButton->appendMenuItem("Perspective", 0x00007000);
	cameraTypeToolBarDropMenuButton->appendMenuItem("Orthographic", 0x00007001);

	//camera culling
	cameraCullingToolBarDropMenuButton=new geToolBarDropMenu(rendererGUI, "Camera Culling", this, fontManager);
	cameraCullingToolBarDropMenuButton->setGUIObserver(this);
	cameraCullingToolBarDropMenuButton->setPos(10, 65);

	//attenuations
	fovHorizontalSlider = new geHorizontalSlider(fontManager);
	fovHorizontalSlider->create(rendererGUI, this, 10, 65, 130);
	fovHorizontalSlider->setSliderValue(1.0f);
	fovHorizontalSlider->setGUIObserver(this);

	nearHorizontalSlider = new geHorizontalSlider(fontManager);
	nearHorizontalSlider->create(rendererGUI, this, 10, 85, 130);
	nearHorizontalSlider->setSliderValue(1.0f);
	nearHorizontalSlider->setGUIObserver(this);

	farHorizontalSlider = new geHorizontalSlider(fontManager);
	farHorizontalSlider->create(rendererGUI, this, 10, 105, 130);
	farHorizontalSlider->setSliderValue(1.0f);
	farHorizontalSlider->setGUIObserver(this);

	applyMainCameraButton = new geButton("", fontManager);
	applyMainCameraButton->create(renderer, this, "Set as main camera", 15, 10);
	applyMainCameraButton->setGUIObserver(this);

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(fontManager);
	pWindowColumn->create(rendererGUI, this, 10, 300.0f, 10.0f, 0.4f);
	stWindowColumnRow* row = pWindowColumn->addRow("Type");
	pWindowColumn->addControl(row, cameraTypeToolBarDropMenuButton, 30.0f);
	row = pWindowColumn->addRow("FOV");
	pWindowColumn->addControl(row, fovHorizontalSlider, 20.0f);
	row = pWindowColumn->addRow("Near Plane");
	pWindowColumn->addControl(row, nearHorizontalSlider, 20.0f);
	row = pWindowColumn->addRow("Far Plane");
	pWindowColumn->addControl(row, farHorizontalSlider, 20.0f);
	row = pWindowColumn->addRow("Camera Culling");
	pWindowColumn->addControl(row, cameraCullingToolBarDropMenuButton, 25.0f);
	row = pWindowColumn->addRow("");
	pWindowColumn->addControl(row, applyMainCameraButton, 20.0f);	//


	camera=NULL;

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();

	setSize(m_cSize.x, applyMainCameraButton->getPos().y+applyMainCameraButton->getSize().y+10);
}

gePropertyCamera::~gePropertyCamera()
{

}

//#if !defined(GEAR_APPLE) //disable Drag-Drop
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

	//windowColumnControl->draw();

	if(sprite)
		sprite->draw();
}

void gePropertyCamera::populatePropertyOfCamera(object3d* obj)
{
	camera = (Camera*)obj;

	fovHorizontalSlider->setSliderValue(camera->getFOV()/120.0f, false);
	nearHorizontalSlider->setSliderValue(camera->getNear()/100.0f, false);
	farHorizontalSlider->setSliderValue(camera->getFar()/10000.0f, false);

	if(camera->getProjectionType()==Camera::PERSPECTIVE_PROJECTION)
		cameraTypeToolBarDropMenuButton->setMenuItem("Perspective");
	else if(camera->getProjectionType()==Camera::ORTHOGRAPHIC_PROJECTION)
		cameraTypeToolBarDropMenuButton->setMenuItem("Orthographic");

	//culling mask
	cameraCullingToolBarDropMenuButton->clearMenu();
	LayerManager* layerManager = monoWrapper::mono_engine_getWorld(0)->getLayerManager();
	for(int x=0;x<MAX_LAYER;x++)
	{
		Layer* layer = layerManager->getLayer(x);
		cameraCullingToolBarDropMenuButton->appendMenuItem(layer->getLayerName(), 0x00003000+x, NULL, false, camera->isLayerCullingMask((1<<x)));
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
	if(fovHorizontalSlider==slider)
	{
		camera->setFOV(fovHorizontalSlider->getSliderValue()*120.0f);
	}
	else if(nearHorizontalSlider==slider)
	{
		camera->setNear(nearHorizontalSlider->getSliderValue()*100.0f);
	}
	else if(farHorizontalSlider==slider)
	{
		camera->setFar(farHorizontalSlider->getSliderValue()*10000.0f);
	}
}

void gePropertyCamera::onButtonClicked(geGUIBase* btn)
{
	if(btn==applyMainCameraButton && applyMainCameraButton->isButtonPressed())
	{
		monoWrapper::mono_engine_getWorld(0)->setActiveCamera(camera);
	}
}

void gePropertyCamera::onCommand(int cmd)
{
	if(cmd==0x00007000)
	{
		camera->setType(Camera::PERSPECTIVE_PROJECTION);
		cameraTypeToolBarDropMenuButton->setMenuItem(cmd);
	}
	else if(cmd==0x00007001)
	{
		camera->setType(Camera::ORTHOGRAPHIC_PROJECTION);
		cameraTypeToolBarDropMenuButton->setMenuItem(cmd);
	}
	else if(cmd>=0x00003000 && cmd<=(0x00003000+32))
	{
		int layerid=(cmd-0x00003000);
		if(camera->isLayerCullingMask((1<<layerid)))
		{
			camera->resetLayerCullingMask((1<<layerid));
			cameraCullingToolBarDropMenuButton->checkMenuItem(cmd, false);
		}
		else
		{
			camera->setLayerCullingMask((1<<layerid));
			cameraCullingToolBarDropMenuButton->checkMenuItem(cmd, true);
		}
	}
}