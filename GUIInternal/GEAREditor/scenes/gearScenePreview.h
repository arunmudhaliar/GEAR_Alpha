#ifndef GEARSCENEPREVIEW_H
#define GEARSCENEPREVIEW_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../mono/src/monoWrapper.h"

class gearScenePreview : public geWindow
{
public:
	gearScenePreview();
	~gearScenePreview();

	void draw();
	void selectedObject3D(object3d* obj)	{	m_pSelectedObj=obj;	}

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);

	virtual void onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

	object3d* m_pSelectedObj;
	gxWorld* m_pPreviewWorldPtr;	//1th world. Must not delete this pointer
};

#endif