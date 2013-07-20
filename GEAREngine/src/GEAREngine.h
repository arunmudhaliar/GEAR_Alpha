#ifndef GEARENGINE_H
#define GEARENGINE_H

#include "core\gxWorld.h"
#include "renderer\gxRenderer.h"

extern "C" {
	DllExport void engine_test_function_for_mono();
	DllExport void engine_init(int nWorldToCreate);
	DllExport gxWorld* engine_getWorld(int index);

	DllExport void engine_update(gxWorld* world, float dt);
	DllExport void engine_resize(gxWorld* world, float x, float y, float cx, float cy);
	DllExport void engine_render(gxWorld* world);
	DllExport void engine_renderSingleObject(gxWorld* world, object3d* obj);
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
	DllExport gxTexture* engine_loadTextureFromFile(gxWorld* world, gxMaterial* material, const char* filename);
	DllExport bool engine_removeObject3d(gxWorld* world, object3d* obj);
}

class GEAREngine
{
public:
	GEAREngine();
	~GEAREngine();

	void initEngine(int nWorldToCreate);
	//void updateEngine(gxWorld* world, float dt);
	//void renderEngine(gxWorld* world);
	//void renderSingleObject(gxWorld* world, object3d* obj);

	//void resizeEngine(gxWorld* world, float x, float y, float cx, float cy);

	gxWorld* getWorld(int index)		{	return m_cWorlds[index];	}

private:
	std::vector<gxWorld*> m_cWorlds;
};

#endif