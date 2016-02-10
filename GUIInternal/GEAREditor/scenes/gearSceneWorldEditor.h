#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../../GEAREngine/src/mono/src/monoWrapper.h"
#include "../gui/geHorizontalSlider.h"
#include "../../../GEAREngine/src/hwShader/HWShaderManager.h"

#ifndef USE_FBO
    #define USE_FBO
#endif
#if defined USE_FBO
#include "../../../GEAREngine/src/core/fbo.h"
#endif

#include "../../../GEAREngine/src/audio/SoundEngine.h"

#include "../../../GEAREngine/src/postProcessor/BrightPassFilter.h"
#include "../../../GEAREngine/src/postProcessor/BlurFilter.h"
#include "../../../GEAREngine/src/postProcessor/ToneMappingFilter.h"
//#define ENABLE_FOG

class gearSceneWorldEditor : public geWindow, public MWorldObserver, public MGUIObserver
{
public:
	gearSceneWorldEditor(geFontManager* fontmanager);
	~gearSceneWorldEditor();

	void draw();
	void selectedObject3D(object3d* obj);

	gxWorld* getMainWorld()	{	return m_pMainWorldPtr;	}

	void preWorldRender();
	void postWorldRender();

	void onButtonClicked(geGUIBase* btn);
	void onSliderChange(geGUIBase* slider);

	void startFollowCam();
	void stopSimulation();

private:
	void drawCameraFrustum(Camera* camera, gxHWShader* shader);
	void getTringleCountForThisTree(object3d* obj, int& count);
	void followObject(float dt, object3d* chasedObj);

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
	virtual void onCommand(int cmd);

	void drawFBO(GLuint t, float x, float y, float cx, float cy);
	void drawFOGFBO(GLuint base_t, GLuint depth_t, float x, float y, float cx, float cy);
	void drawGrid();
	void drawSelectedObject();
	void drawOctree();
	void drawCompas();
	void drawStats();
	void drawFBO2FrameBuffer();
	void drawWorld();
	void drawShadowMapPass();

private:
	int noOfSelectedObjectTriangles;
	object3d* selectedObject;
	gxWorld* m_pMainWorldPtr;	//0th world. Must not delete this pointer

	geHorizontalSlider* ambientLightHorizontalSlider;
	geHorizontalSlider* timeScaleHorizontalSlider;

	geVector2f gridOnYAxisArray[180];
	geVector2f gridOnXAxisArray[180];
	geVector2f thickGridOnYAxisArray[18];
	geVector2f thickGridOnXAxisArray[18];
	geVector2f gridOuterBoxArray[4];

	vector2f mousePrevPosition;
	vector3f mousePreviousPositionInWorld;
	int selectedAxisID;

	bool isTransformThroughLocalAxis;
	geToolBarButton* localOrGlobalAxisToolBarButton;
	geToolBarButton* gridViewToolBarButton;
	geToolBarButton* onlyLightPassToolBarButton;
	geToolBarButton* showOOBBToolBarButton;
	geToolBarButton* showOctreeToolBarButton;

	geToolBarButton* playGameToolBarButton;
	geToolBarButton* pauseGameToolBarButton;
	bool isMonoGameInitialized;

	geToolBarButton* translateGizmoToolBarButton;
	geToolBarButton* rotateGizmoToolBarButton;
	geToolBarButton* scaleGizmoToolBarButton;

	HWShaderManager* hwShaderManager;
	Sprite2Dx lightBillBoardSprite;

#if defined USE_FBO
	FBO multiPassFBO;
	FBO fogFBO;
#endif
	bool isEnablePostProcessorBlur;
	SoundEngine soundEngine;
	int lastGLErrorID;

	bool stopFollowCam;
	vector3f debugPosition;

	BrightPassFilter brightPassFilter;
	BlurFilter blurFilter;
	ToneMappingFilter toneMappingFilter;
};