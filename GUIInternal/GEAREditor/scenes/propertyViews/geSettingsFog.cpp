#include "geSettingsFog.h"
#include "../../EditorApp.h"

geSettingsFog::geSettingsFog(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, stFog* fog_struct, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	//setSize(m_cSize.x, 115.0f);

	m_pFog_struct=fog_struct;

	m_pPushBtn_Fog = new gePushButton("", fontmanager);
	m_pPushBtn_Fog->create(renderer, this, "", 15, 10);
	m_pPushBtn_Fog->setGUIObserver(this);

	//fog
	m_cFogSubView.edit_fog_start = new geHorizontalSlider(fontManagerGUI);
	m_cFogSubView.edit_fog_start->create(rendererGUI, this, "slider", 10, 65, 130);
	m_cFogSubView.edit_fog_start->setSliderValue(0.0f);
	m_cFogSubView.edit_fog_start->setGUIObserver(this);

	m_cFogSubView.edit_fog_end = new geHorizontalSlider(fontManagerGUI);
	m_cFogSubView.edit_fog_end->create(rendererGUI, this, "slider", 10, 65, 130);
	m_cFogSubView.edit_fog_end->setSliderValue(1.0f);
	m_cFogSubView.edit_fog_end->setGUIObserver(this);

	m_cFogSubView.edit_fog_density = new geHorizontalSlider(fontManagerGUI);
	m_cFogSubView.edit_fog_density->create(rendererGUI, this, "slider", 10, 65, 130);
	m_cFogSubView.edit_fog_density->setSliderValue(1.0f);
	m_cFogSubView.edit_fog_density->setGUIObserver(this);

	m_cFogSubView.color_fog_color = new geColorControl(fontmanager);
	m_cFogSubView.color_fog_color->create(rendererGUI, this, 10, 10);
	m_cFogSubView.color_fog_color->setControlColor(1.0, 1.0, 1.0, 1.0);
	m_cFogSubView.color_fog_color->setGUIObserver(this);

	m_cFogSubView.menu_fog_type=new geToolBarDropMenu(rendererGUI, "FogType", this, fontmanager);
	m_cFogSubView.menu_fog_type->setGUIObserver(this);
	m_cFogSubView.menu_fog_type->setPos(10, 35);
	m_cFogSubView.menu_fog_type->appendMenuItem("Linear", 0x00005100);
	m_cFogSubView.menu_fog_type->appendMenuItem("Exp", 0x00005101);
	m_cFogSubView.menu_fog_type->appendMenuItem("Exp2", 0x00005102);
	m_cFogSubView.menu_fog_type->setMenuItem(0);

	//window column
	m_cFogSubView.pWindowColumn = new geWindowColumn(fontManagerGUI);
	m_cFogSubView.pWindowColumn->create(rendererGUI, this, 35, 300.0f, 10.0f, 0.35f);
	stWindowColumnRow* row = m_cFogSubView.pWindowColumn->addRow("Fog start");
	m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.edit_fog_start, m_cFogSubView.edit_fog_start->getSize().y*3);
	row = m_cFogSubView.pWindowColumn->addRow("Fog end");
	m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.edit_fog_end, m_cFogSubView.edit_fog_end->getSize().y*3);
	row = m_cFogSubView.pWindowColumn->addRow("Fog density");
	m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.edit_fog_density, m_cFogSubView.edit_fog_density->getSize().y*3);
	row = m_cFogSubView.pWindowColumn->addRow("Fog Color");
	m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.color_fog_color, m_cFogSubView.color_fog_color->getSize().y+4);
	row = m_cFogSubView.pWindowColumn->addRow("Fog Type");
	m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.menu_fog_type);

	setSize(m_cSize.x, row->getYPoistion()+m_cFogSubView.menu_fog_type->getSize().y + 5);

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
	if(colorControl==m_cFogSubView.color_fog_color)
	{
		m_pFog_struct->setFogColor(vector4f(
			m_cFogSubView.color_fog_color->getControlColor().x,
			m_cFogSubView.color_fog_color->getControlColor().y,
			m_cFogSubView.color_fog_color->getControlColor().z,
			m_cFogSubView.color_fog_color->getControlColor().z
			));
	}
}

void geSettingsFog::onCommand(int cmd)
{
	if(m_cFogSubView.menu_fog_type && cmd>=0x00005100 && cmd<0x00005100+m_cFogSubView.menu_fog_type->getMenuItemCount())
	{
		m_pFog_struct->setFogType((stFog::EFOG_TYPE)(cmd-0x00005100));
		m_cFogSubView.menu_fog_type->setMenuItem(cmd);
	}
}

void geSettingsFog::onSliderChange(geGUIBase* slider)
{
	if(m_cFogSubView.edit_fog_start==slider)
	{
		m_pFog_struct->setFogStart(m_cFogSubView.edit_fog_start->getSliderValue());
	}
	else if(m_cFogSubView.edit_fog_end==slider)
	{
		m_pFog_struct->setFogEnd(m_cFogSubView.edit_fog_end->getSliderValue());
	}
	else if(m_cFogSubView.edit_fog_density==slider)
	{
		m_pFog_struct->setFogDensity(m_cFogSubView.edit_fog_density->getSliderValue());
	}

	if(abs(m_pFog_struct->fog_end)>0.00001f || abs(m_pFog_struct->fog_start)>0.00001f)
		m_pFog_struct->calculateScale();
}