#ifndef GETEXTUREDLG_H
#define GETEXTUREDLG_H

#include "geSecondryView.h"
#include "../gui/geHorizontalSlider.h"
#include "../gui/geColorControl.h"
#include "../gui/geTextureThumbnail.h"

class geTextureDlgMainWindow : public geWindow
{
public:
	geTextureDlgMainWindow();
	~geTextureDlgMainWindow();
	virtual void onDraw();
};

class geTextureDlgInfoWindow : public geWindow
{
public:
	geTextureDlgInfoWindow();
	~geTextureDlgInfoWindow();
	virtual void onDraw();
};

class geTextureDlg : public geSecondryView, public MGUIObserver
{
public:
	geTextureDlg(geTextureThumbnail* pObserverControlPtr);
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

	geTextureDlgMainWindow* m_pWindow;
	geTextureDlgInfoWindow* m_pInfoWindow;
	geTextureThumbnail* m_pObserverControlPtr;	//must not delete this pointer

	std::vector<geGUIBase*> m_vTextureThumbs;
};

#endif