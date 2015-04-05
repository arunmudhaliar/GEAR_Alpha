#include "gePropertyLayers.h"
#include "../../EditorApp.h"

gePropertyLayers::gePropertyLayers(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	LayerManager* layerManager = monoWrapper::mono_engine_getWorld(0)->getLayerManager();

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(m_pFontManagerPtr);
	stWindowColumnRow* row=NULL;
	pWindowColumn->create(m_pRenderer, this, 10, 300.0f, 10.0f, 0.15f);
	for(int x=0;x<MAX_LAYER;x++)
	{
		m_pTextBoxLayers[x] = new geTextBox(layerManager->getLayer(x)->getLayerName(), m_pFontManagerPtr);
		m_pTextBoxLayers[x]->create(renderer, this, layerManager->getLayer(x)->getLayerName(), 0, 0, 150, 16);
		m_pTextBoxLayers[x]->setGUIObserver(this);

		char buffer[32];
		sprintf(buffer, "%d", x);
		row = pWindowColumn->addRow(buffer);
		pWindowColumn->addControl(row, m_pTextBoxLayers[x], m_pTextBoxLayers[x]->getSize().y+2);
	}

	setSize(m_cSize.x, row->getYPoistion()+row->getHeight()+10);

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyLayers::~gePropertyLayers()
{
}

void gePropertyLayers::drawNode()
{
	drawRect(&m_cVBClientArea);

	geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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

	if(m_pSprite)
		m_pSprite->draw();
}

void gePropertyLayers::onTextChange(geGUIBase* textbox)
{
	LayerManager* layerManager = monoWrapper::mono_engine_getWorld(0)->getLayerManager();

	for(int x=0;x<MAX_LAYER;x++)
	{
		if(m_pTextBoxLayers[x]==textbox)
		{
			if(textbox->getName() && strlen(textbox->getName())>0)
			{
				layerManager->getLayer(x)->setLayerName(textbox->getName());
			}
			else
			{
				textbox->setName("Layer");
			}
			break;
		}
	}
}