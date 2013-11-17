#ifndef GEARENGINE_H
#define GEARENGINE_H

#include "core\gxWorld.h"
#include "renderer\gxRenderer.h"
#include "hwShader\HWShaderManager.h"

#define CREATE_MATERIAL_FOR_MESH_IF_NOT_FOUND_IN_METAFILE	0

class MEngineObserver
{
public:
	virtual void onAppendToWorld(gxWorld* world, object3d* obj)
	{
	}
	virtual void onRemoveFromWorld(gxWorld* world, object3d* obj)
	{
	}
};

extern "C" {
	DllExport void engine_test_function_for_mono();
	DllExport void engine_init(int nWorldToCreate);
	DllExport gxWorld* engine_getWorld(int index);

	DllExport void engine_update(gxWorld* world, float dt);
	DllExport void engine_resize(gxWorld* world, float x, float y, float cx, float cy, float nearplane, float farplane);
	DllExport void engine_render(gxWorld* world, object3d* light);
	DllExport void engine_renderSingleObject(gxWorld* world, object3d* obj, object3d* light);
	DllExport object3d* engine_loadAndAppendFBX(gxWorld* world, const char* filename);
	DllExport object3d* engine_loadFBX(gxWorld* world, const char* filename);
	DllExport object3d* engine_appendObject3dToRoot(gxWorld* world, object3d* obj);

	DllExport void engine_mouseLButtonDown(gxWorld* world, int x, int y, int flag);
	DllExport void engine_mouseLButtonUp(gxWorld* world, int x, int y, int flag);
	DllExport void engine_mouseRButtonDown(gxWorld* world, int x, int y, int flag);
	DllExport void engine_mouseRButtonUp(gxWorld* world, int x, int y, int flag);
	DllExport void engine_mouseWheel(gxWorld* world, int zDelta, int x, int y, int flag);
	DllExport void engine_mouseMove(gxWorld* world, int x, int y, int flag);
	DllExport void engine_setMetaFolder(gxWorld* world, const char* metaFolder);
	DllExport gxTexture* engine_loadTextureFromFile(gxWorld* world, gxMaterial* material, const char* filename, int submap);
	DllExport bool engine_removeObject3d(gxWorld* world, object3d* obj);
	DllExport bool engine_destroyObject3d(gxWorld* world, object3d* obj);

	DllExport void engine_setEngineObserver(MEngineObserver* observer);
	DllExport void engine_setObject3dObserver(MObject3dObserver* observer);

	DllExport object3d* engine_createEmptyObject3d(object3d* parentObj, const char* name);
	DllExport object3d* engine_createLight(object3d* parentObj, const char* name, gxLight::ELIGHT_TYPE eType);

	DllExport HWShaderManager* engine_getHWShaderManager();
	DllExport void engine_destroy();
}

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