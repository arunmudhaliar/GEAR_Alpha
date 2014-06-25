#ifndef GEARENGINE_H
#define GEARENGINE_H

#include "core/gxWorld.h"
#include "renderer/gxRenderer.h"
#include "hwShader/HWShaderManager.h"
#include "core/Timer.h"

#define CREATE_MATERIAL_FOR_MESH_IF_NOT_FOUND_IN_METAFILE	0

////////////////////GEARENGINE C# BRIDGE////////////////////
extern "C" {
	//init
	DECLSPEC int engine_test_function_for_mono();
	DECLSPEC void engine_init(int nWorldToCreate);
	DECLSPEC void engine_resize(gxWorld* world, float x, float y, float cx, float cy, float nearplane, float farplane);

	//destroy interface
	DECLSPEC void engine_destroy();

	//set-get
	DECLSPEC void engine_setMetaFolder(gxWorld* world, const char* metaFolder);
	DECLSPEC gxWorld* engine_getWorld(int index);
	//hw shader
	DECLSPEC HWShaderManager* engine_getHWShaderManager();

	//message pump
	DECLSPEC void engine_update(gxWorld* world, float dt);
	DECLSPEC void engine_render(gxWorld* world, object3d* light);
	DECLSPEC void engine_renderSingleObject(gxWorld* world, object3d* obj, object3d* light);

	//load
	DECLSPEC object3d* engine_loadAndAppendMesh(gxWorld* world, const char* filename);

	//mouse events
	DECLSPEC void engine_mouseLButtonDown(gxWorld* world, int x, int y, int flag);
	DECLSPEC void engine_mouseLButtonUp(gxWorld* world, int x, int y, int flag);
	DECLSPEC void engine_mouseRButtonDown(gxWorld* world, int x, int y, int flag);
	DECLSPEC void engine_mouseRButtonUp(gxWorld* world, int x, int y, int flag);
	DECLSPEC void engine_mouseWheel(gxWorld* world, int zDelta, int x, int y, int flag);
	DECLSPEC void engine_mouseMove(gxWorld* world, int x, int y, int flag);

	//object3d func : need to verify its use inside the code (c++ & c#)
	DECLSPEC bool engine_removeObject3d(gxWorld* world, object3d* obj);
	DECLSPEC bool engine_destroyObject3d(gxWorld* world, object3d* obj);

	//interface : engine observers
	DECLSPEC void engine_setEngineObserver(MEngineObserver* observer);
	DECLSPEC void engine_setObject3dObserver(MObject3dObserver* observer);

	//object3d creation interfaces
	DECLSPEC object3d* engine_createEmptyObject3d(object3d* parentObj, const char* name);
	DECLSPEC object3d* engine_createLight(object3d* parentObj, const char* name, gxLight::ELIGHT_TYPE eType);
	DECLSPEC object3d* engine_createCamera(object3d* parentObj, const char* name, gxRenderer* renderer);

	//logs
	DECLSPEC void engine_consoleLog(const char* msg, int msgtype=0);	//0=info, 1=warning, 2=error



}
//


class GEAREngine
{
public:
	GEAREngine();
	~GEAREngine();

	void initEngine(int nWorldToCreate);
	void resetEngine();

	gxWorld* getWorld(int index)		{	return m_cWorlds[index];	}

	HWShaderManager* getHWShaderManager()	{	return &m_cHWShaderManager;	}
private:
	std::vector<gxWorld*> m_cWorlds;
	HWShaderManager m_cHWShaderManager;
};

#endif
