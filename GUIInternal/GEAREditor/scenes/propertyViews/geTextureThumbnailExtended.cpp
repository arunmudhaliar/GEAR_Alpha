#include "geTextureThumbnailExtended.h"
#include "../../secondryViews/geTextureDlg.h"
#include "../../EditorApp.h"

bool geTextureThumbnailExtended::onMouseLButtonDown(float x, float y, int nFlag)
{
	geTextureDlg* newTextureDlg	 = new geTextureDlg(this);
	newTextureDlg->showView(EditorApp::getMainWindowHandle());

	return true;
}