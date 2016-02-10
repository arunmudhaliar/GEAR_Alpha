#include "geSettingsFog.h"
#include "../../EditorApp.h"

geSettingsFog::geSettingsFog(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, stFog* fog_struct, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	//setSize(m_cSize.x, 115.0f);

	fogStructObject=fog_struct;

	fogToggle = new gePushButton("", fontmanager);
	fogToggle->create(renderer, this, "", 15, 10);
	fogToggle->setGUIObserver(this);

	//fog
	fogSubViewStructObject.edit_fog_start = new geHorizontalSlider(fontManagerGUI);
	fogSubViewStructObject.edit_fog_start->create(rendererGUI, this, "slider", 10, 65, 130);
	fogSubViewStructObject.edit_fog_start->setSliderValue(0.0f);
	fogSubViewStructObject.edit_fog_start->setGUIObserver(this);

	fogSubViewStructObject.edit_fog_end = new geHorizontalSlider(fontManagerGUI);
	fogSubViewStructObject.edit_fog_end->create(rendererGUI, this, "slider", 10, 65, 130);
	fogSubViewStructObject.edit_fog_end->setSliderValue(1.0f);
	fogSubViewStructObject.edit_fog_end->setGUIObserver(this);

	fogSubViewStructObject.edit_fog_density = new geHorizontalSlider(fontManagerGUI);
	fogSubViewStructObject.edit_fog_density->create(rendererGUI, this, "slider", 10, 65, 130);
	fogSubViewStructObject.edit_fog_density->setSliderValue(1.0f);
	fogSubViewStructObject.edit_fog_density->setGUIObserver(this);

	fogSubViewStructObject.color_fog_color = new geColorControl(fontmanager);
	fogSubViewStructObject.color_fog_color->create(rendererGUI, this, 10, 10);
	fogSubViewStructObject.color_fog_color->setControlColor(1.0, 1.0, 1.0, 1.0);
	fogSubViewStructObject.color_fog_color->setGUIObserver(this);

	fogSubViewStructObject.menu_fog_type=new geToolBarDropMenu(rendererGUI, "FogType", this, fontmanager);
	fogSubViewStructObject.menu_fog_type->setGUIObserver(this);
	fogSubViewStructObject.menu_fog_type->setPos(10, 35);
	fogSubViewStructObject.menu_fog_type->appendMenuItem("Linear", 0x00005100);
	fogSubViewStructObject.menu_fog_type->appendMenuItem("Exp", 0x00005101);
	fogSubViewStructObject.menu_fog_type->appendMenuItem("Exp2", 0x00005102);
	fogSubViewStructObject.menu_fog_type->setMenuItem(0);

	//window column
	fogSubViewStructObject.pWindowColumn = new geWindowColumn(fontManagerGUI);
	fogSubViewStructObject.pWindowColumn->create(rendererGUI, this, 35, 300.0f, 10.0f, 0.35f);
	stWindowColumnRow* row = fogSubViewStructObject.pWindowColumn->addRow("Fog start");
	fogSubViewStructObject.pWindowColumn->addControl(row, fogSubViewStructObject.edit_fog_start, fogSubViewStructObject.edit_fog_start->getSize().y*3);
	row = fogSubViewStructObject.pWindowColumn->addRow("Fog end");
	fogSubViewStructObject.pWindowColumn->addControl(row, fogSubViewStructObject.edit_fog_end, fogSubViewStructObject.edit_fog_end->getSize().y*3);
	row = fogSubViewStructObject.pWindowColumn->addRow("Fog density");
	fogSubViewStructObject.pWindowColumn->addControl(row, fogSubViewStructObject.edit_fog_density, fogSubViewStructObject.edit_fog_density->getSize().y*3);
	row = fogSubViewStructObject.pWindowColumn->addRow("Fog Color");
	fogSubViewStructObject.pWindowColumn->addControl(row, fogSubViewStructObject.color_fog_color, fogSubViewStructObject.color_fog_color->getSize().y+4);
	row = fogSubViewStructObject.pWindowColumn->addRow("Fog Type");
	fogSubViewStructObject.pWindowColumn->addControl(row, fogSubViewStructObject.menu_fog_type);

	setSize(m_cSize.x, row->getYPoistion()+fogSubViewStructObject.menu_fog_type->getSize().y + 5);

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

geSettingsFog::~geSettingsFog()
{
}

void geSettingsFog::drawNode()
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

	if(sprite)
		sprite->draw();
}

void geSettingsFog::onColorChange(geGUIBase* colorControl)
{
	if(colorControl==fogSubViewStructObject.color_fog_color)
	{
		fogStructObject->setFogColor(vector4f(
			fogSubViewStructObject.color_fog_color->getControlColor().x,
			fogSubViewStructObject.color_fog_color->getControlColor().y,
			fogSubViewStructObject.color_fog_color->getControlColor().z,
			fogSubViewStructObject.color_fog_color->getControlColor().z
			));
	}
}

void geSettingsFog::onCommand(int cmd)
{
	if(fogSubViewStructObject.menu_fog_type && cmd>=0x00005100 && cmd<0x00005100+fogSubViewStructObject.menu_fog_type->getMenuItemCount())
	{
		fogStructObject->setFogType((stFog::EFOG_TYPE)(cmd-0x00005100));
		fogSubViewStructObject.menu_fog_type->setMenuItem(cmd);
	}
}

void geSettingsFog::onSliderChange(geGUIBase* slider)
{
	if(fogSubViewStructObject.edit_fog_start==slider)
	{
		fogStructObject->setFogStart(fogSubViewStructObject.edit_fog_start->getSliderValue());
	}
	else if(fogSubViewStructObject.edit_fog_end==slider)
	{
		fogStructObject->setFogEnd(fogSubViewStructObject.edit_fog_end->getSliderValue());
	}
	else if(fogSubViewStructObject.edit_fog_density==slider)
	{
		fogStructObject->setFogDensity(fogSubViewStructObject.edit_fog_density->getSliderValue());
	}

	if(abs(fogStructObject->fog_end)>0.00001f || abs(fogStructObject->fog_start)>0.00001f)
		fogStructObject->calculateScale();
}