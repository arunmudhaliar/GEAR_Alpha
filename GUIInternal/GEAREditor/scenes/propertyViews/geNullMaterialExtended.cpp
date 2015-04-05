#include "geNullMaterialExtended.h"
#include "../../EditorApp.h"

void geNullMaterialExtended::draw()
{
	geTextureThumbnail::draw();
	geFontManager::g_pFontArial10_80Ptr->drawString("Drag-drop a material.", getPos().x, geFontManager::g_pFontArial10_80Ptr->getLineHeight()+10, m_cSize.x);
}