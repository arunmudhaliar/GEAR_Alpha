#pragma once

#include "geSecondryView.h"
#include "../gui/geHorizontalSlider.h"

class geAssetImportDlg : public geSecondryView, public MGUIObserver
{
public:
	geAssetImportDlg(geFontManager* fontManager, rendererGL10* mainRenderer);
	virtual ~geAssetImportDlg();

	virtual void onCreate(float cx, float cy);
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();
    
    void setBuffer(const char* assetPath);

protected:
	geWindow* m_pWindow;
    char m_cszBuffer[2048];
};