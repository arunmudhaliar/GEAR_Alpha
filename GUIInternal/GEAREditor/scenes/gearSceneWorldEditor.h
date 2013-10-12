#ifndef GEARSCENEWORLDEDITOR_H
#define GEARSCENEWORLDEDITOR_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../mono/src/monoWrapper.h"
#include "../gui/geHorizontalSlider.h"

#include "../../../GEAREngine/src/hwShader/HWShaderManager.h"

#define USE_FBO
#if defined USE_FBO
#include "../../../GEAREngine/src/core/fbo.h"
#endif

class gearSceneWorldEditor : public geWindow, public MWorldObserver, public MGUIObserver
{
public:
	gearSceneWorldEditor();
	~gearSceneWorldEditor();

	void draw();
	void selectedObject3D(object3d* obj)	{	m_pSelectedObj=obj;	}

	gxWorld* getMainWorld()	{	return m_pMainWorldPtr;	}

	void preWorldRender();
	void postWorldRender();

	void onButtonClicked(geGUIBase* btn);
	void onSliderChange(geGUIBase* slider);

protected:
	virtual void onCreate();
	virtual void onDraw();
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);

	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

	void drawFBO(GLuint t, float x, float y, float cx, float cy);
	void drawGrid();
	void drawSelectedObject();
	void drawStats();
	void drawLightsOnMultiPass();

	object3d* m_pSelectedObj;
	gxWorld* m_pMainWorldPtr;	//0th world. Must not delete this pointer

	geHorizontalSlider* m_pHorizontalSlider_LightAmbient;
	geHorizontalSlider* m_pHorizontalSlider_TimeScale;

	geVector2f m_cGridOnYAxis[180];
	geVector2f m_cGridOnXAxis[180];
	geVector2f m_cThickGridOnYAxis[18];
	geVector2f m_cThickGridOnXAxis[18];
	geVector2f m_cGridOuterBox[4];

	vector2f m_cMousePrevPos;
	vector3f m_cMousePrevPosInWorld;
	int m_iAxisSelected;

	bool m_bTransformThroughLocalAxis;
	geToolBarButton* m_pLocalOrGlobalAxis;
	geToolBarButton* m_pTBGridView;

	geToolBarButton* m_pPlayButton;
	geToolBarButton* m_pPauseButton;
	bool m_bMonoGameInitialized;

	geToolBarButton* m_pTranslateGizmo;
	geToolBarButton* m_pRotateGizmo;
	geToolBarButton* m_pScaleGizmo;

	HWShaderManager* m_pHWShaderManager;

#if defined USE_FBO
	FBO m_cMultiPassFBO;
#endif
};

#endif