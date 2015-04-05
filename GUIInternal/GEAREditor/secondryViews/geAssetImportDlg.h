#ifndef GEASSETIMPORTDLG_H
#define GEASSETIMPORTDLG_H

#include "geSecondryView.h"
#include "../gui/geHorizontalSlider.h"

class geAssetImportDlg : public geSecondryView, public MGUIObserver
{
public:
	geAssetImportDlg(geFontManager* fontManager, rendererGL10* mainRenderer);
	virtual ~geAssetImportDlg();

	virtual void onCreate();
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();
    
    void setBuffer(const char* assetPath);

protected:
	geWindow* m_pWindow;
    char m_cszBuffer[2048];
};

#endif