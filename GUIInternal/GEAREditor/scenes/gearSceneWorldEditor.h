#ifndef GEARSCENEWORLDEDITOR_H
#define GEARSCENEWORLDEDITOR_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../mono/src/monoWrapper.h"
#include "../gui/geHorizontalSlider.h"

class gearSceneWorldEditor : public geWindow
{
public:
	gearSceneWorldEditor();
	~gearSceneWorldEditor();

	void draw();
	void selectedObject3D(object3d* obj)	{	m_pSelectedObj=obj;	}

	gxWorld* getMainWorld()	{	return m_pMainWorldPtr;	}
protected:
	virtual void onCreate();
	virtual void onDraw();
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);

	virtual void onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

	object3d* m_pSelectedObj;
	gxWorld* m_pMainWorldPtr;	//0th world. Must not delete this pointer

	geHorizontalSlider* m_pHorizontalSlider_LightAmbient;
};

#endif