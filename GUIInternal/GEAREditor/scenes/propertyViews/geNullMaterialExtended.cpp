#include "geNullMaterialExtended.h"
#include "../../EditorApp.h"

void geNullMaterialExtended::draw()
{
	geTextureThumbnail::draw();
	geGUIManager::g_pFontArial10_80Ptr->drawString("Drag-drop a material.", getPos().x, geGUIManager::g_pFontArial10_80Ptr->getLineHeight()+10, m_cSize.x);
}