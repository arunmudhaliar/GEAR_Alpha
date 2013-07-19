#ifndef MONOWRAPPER_H
#define MONOWRAPPER_H

#include <mono\metadata\mono-config.h>
#include <mono\metadata\metadata.h>
#include <mono\metadata\assembly.h>
#include <mono\jit\jit.h>

#include "../../../GEAREngine/src/GEAREngine.h"

class monoWrapper
{
public:
	static void initMono();
	static void updateMono();
	static void destroyMono();
	
	static void bindEngineMethods();

	static void initDebugConsole();
	static void destroyDebugConsole();

	//MONO C# test function
	static void mono_engine_test_function_for_mono();
	//

	//MONO ENGINE WRAPPERS
	static void mono_engine_init(int nWorldToCreate);
	static gxWorld* mono_engine_getWorld(int index);

	static void mono_engine_update(gxWorld* world, float dt);
	static void mono_engine_resize(gxWorld* world, float x, float y, float cx, float cy);
	static void mono_engine_render(gxWorld* world);
	static void mono_engine_renderSingleObject(gxWorld* world, object3d* obj);
	static object3d* mono_engine_loadAndAppendFBX(gxWorld* world, const char* filename);
	static object3d* mono_engine_loadFBX(gxWorld* world, const char* filename);
	static object3d* mono_engine_appendObject3dToRoot(gxWorld* world, object3d* obj);

	static void mono_engine_mouseLButtonDown(gxWorld* world, int x, int y, int flag);
	static void mono_engine_mouseLButtonUp(gxWorld* world, int x, int y, int flag);
	static void mono_engine_mouseRButtonDown(gxWorld* world, int x, int y, int flag);
	static void mono_engine_mouseRButtonUp(gxWorld* world, int x, int y, int flag);
	static void mono_engine_mouseWheel(gxWorld* world, int zDelta, int x, int y, int flag);
	static void mono_engine_mouseMove(gxWorld* world, int x, int y, int flag);
	static void mono_engine_setMetaFolder(gxWorld* world, const char* metaFolder);
	static gxTexture* mono_engine_loadTextureFromFile(gxWorld* world, gxMaterial* material, const char* filename);
	static bool mono_engine_removeObject3d(gxWorld* world, object3d* obj);
	//

private:
	static MonoObject*		g_pMonoGEAREntryPointClass_Instance_Variable;
	static MonoDomain*		g_pMonoDomain;
	static MonoAssembly*	g_pMonoAssembly;
	static MonoImage*		g_pImage;
	static MonoClass*		g_pMonoGEAREntryPointClass;

	//ENGIINE INTERFACES
	/*
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
	*/

	static MonoMethod* g_monogear_engine_test_function_for_mono;
	static MonoMethod* g_pMethod_engine_init;
	static MonoMethod* g_pMethod_engine_getWorld;
	static MonoMethod* g_pMethod_engine_update;
	static MonoMethod* g_pMethod_engine_resize;
	static MonoMethod* g_pMethod_engine_render;
	static MonoMethod* g_pMethod_engine_renderSingleObject;
	static MonoMethod* g_pMethod_engine_loadAndAppendFBX;
	static MonoMethod* g_pMethod_engine_loadFBX;
	static MonoMethod* g_pMethod_engine_appendObject3dToRoot;
	static MonoMethod* g_pMethod_engine_mouseLButtonDown;
	static MonoMethod* g_pMethod_engine_mouseLButtonUp;
	static MonoMethod* g_pMethod_engine_mouseRButtonDown;
	static MonoMethod* g_pMethod_engine_mouseRButtonUp;
	static MonoMethod* g_pMethod_engine_mouseWheel;
	static MonoMethod* g_pMethod_engine_mouseMove;
	static MonoMethod* g_pMethod_engine_setMetaFolder;
	static MonoMethod* g_pMethod_engine_loadTextureFromFile;
	static MonoMethod* g_pMethod_engine_removeObject3d;
	//
};

#endif