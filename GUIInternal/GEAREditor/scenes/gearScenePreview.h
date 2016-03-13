#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../../GEAREngine/src/mono/src/monoWrapper.h"

#define CAM_SPEED	3.0f
#define CAMERA_LOOKAT_OFFSET vector3f(0, 0, 0)
#define CAMERA_EYE_OFFSET	vector3f(-75, 0, 0)

class gearScenePreview : public geWindow
{
public:
	gearScenePreview(geFontManager* fontmanager);
	~gearScenePreview();

	void draw();
	void selectedObject3D(object3d* obj);
	object3d* getSelectedObject3D()			{	return selectedObject;	}
	void reinitPreviewWorld();

protected:
	virtual void onCreate(float cx, float cy);
	virtual void onDraw();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);

	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

private:
	void followObject(float dt, object3d* chasedObj);

	object3d* selectedObject;
	gxWorld* previewWorld;	//1th world. Must not delete this pointer
	object3d* previewLight;
	bool stopFollowCam;
	vector2i previousMousePosition;
};