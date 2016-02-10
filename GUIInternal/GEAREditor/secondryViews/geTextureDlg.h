#pragma once

#include "geSecondryView.h"
#include "../gui/geHorizontalSlider.h"
#include "../gui/geColorControl.h"
#include "../gui/geTextureThumbnail.h"

class geTextureDlgMainWindow : public geWindow
{
public:
	geTextureDlgMainWindow(geFontManager* fontmanager);
	~geTextureDlgMainWindow();
	virtual void onDraw();
};

class geTextureDlgInfoWindow : public geWindow
{
public:
	geTextureDlgInfoWindow(geFontManager* fontmanager);
	~geTextureDlgInfoWindow();
	virtual void onDraw();
};

class geTextureDlg : public geSecondryView, public MGUIObserver
{
public:
	geTextureDlg(geTextureThumbnail* pObserverControlPtr, geFontManager* fontmanager, rendererGL10* mainRenderer);
	virtual ~geTextureDlg();

	virtual void onCreate();
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();

	virtual bool onMouseMove(float x, float y, int flag);

protected:
	gxTexture* loadTextureFromMeta(CTextureManager& textureManager, unsigned int crc);
	gxTexture* loadTextureFromFile(CTextureManager& textureManager, const char* filename);
	int find_textures(rendererGL10* renderer, const char *dirname, geGUIBase* dlg, std::vector<geGUIBase*>& textureThumbList);

	geTextureDlgMainWindow* textureDlgMainWindow;
	geTextureDlgInfoWindow* textureDlgInfoWindow;
	geTextureThumbnail* observerControlPtr;	//must not delete this pointer

	std::vector<geGUIBase*> textureThumbnailList;
};