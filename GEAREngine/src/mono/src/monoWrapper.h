#pragma once

#include <mono/metadata/mono-config.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/mono-gc.h>
#include <mono/jit/jit.h>

#include "../../GEAREngine.h"   //TODO: Need to check this path
#include "monoScript.h"
#include "signalHandler.h"

class DECLSPEC monoWrapper
{
public:
    static void loadMonoModules(const std::string& monoInstallPath);
	static void reInitMono(const char* projecthomedirectory=NULL);	//projecthomedirectory is valid only for WIN32
	static void updateMono();
	static void destroyMono();
    
    static bool createAppDomain();
    static void unloadAddDomain();
	
	static void bindEngineMethods();
#if defined(GEAR_WINDOWS) && defined(GEAR_EDITOR)
	static void initDebugConsole();
	static void destroyDebugConsole();
#endif
	static void destroyUserDefinedMonoClassDefs();

	//MONO C# test function
	static int mono_engine_test_function_for_mono();
	//

	//MONO GAME WRAPPERS
	static void mono_game_start();
	static void mono_game_run(float dt);
    static void mono_game_stop();
    
	static bool mono_game_onkeydown(int charValue, int flag);
    static bool mono_game_onkeyup(int charValue, int flag);
	//

	//MONO ENGINE WRAPPERS
	static void mono_engine_init(int nWorldToCreate);
	static void mono_engine_init_for_mono_android();	//shouldn't call this function before 'mono_engine_init'
	static gxWorld* mono_engine_getWorld(int index);

	static void mono_engine_update(gxWorld* world, float dt);
	static void mono_engine_resize(gxWorld* world, float x, float y, float cx, float cy, float nearplane, float farplane);
	static void mono_engine_render(gxWorld* world, object3d* light, int renderFlag);
	static void mono_engine_renderSingleObject(gxWorld* world, object3d* obj, object3d* light, int renderFlag);

	static void mono_engine_mouseLButtonDown(gxWorld* world, int x, int y, int flag);
	static void mono_engine_mouseLButtonUp(gxWorld* world, int x, int y, int flag);
	static void mono_engine_mouseRButtonDown(gxWorld* world, int x, int y, int flag);
	static void mono_engine_mouseRButtonUp(gxWorld* world, int x, int y, int flag);
	static void mono_engine_mouseWheel(gxWorld* world, int zDelta, int x, int y, int flag);
	static void mono_engine_mouseMove(gxWorld* world, int x, int y, int flag);
	static void mono_engine_setMetaFolder(gxWorld* world, const char* metaFolder);
	static bool mono_engine_removeObject3d(gxWorld* world, object3d* obj);
	static bool mono_engine_destroyObject3d(gxWorld* world, object3d* obj);

	static void mono_object3d_onObject3dChildAppend(object3d* parent, object3d* child);
	static void mono_object3d_onObject3dChildRemove(object3d* parent, object3d* child);

#if defined(GEAR_EDITOR)
	static int traverseForCSharpFiles(const char *dirname, std::vector<std::string>* csharpfilelist);
	static bool compileCSharpScripts(std::vector<std::string>* csharpfilelist);
	static int exec_cmd(char const *cmd, char *buf);
	static int exec_cmd(char const *cmd);
#endif

	static monoScript* mono_getMonoScripDef(const char* scriptname);

    static bool mono_isSimulationRunning();
    
private:
    static std::string      g_cMonoInstallPath;
    static uint32_t         g_uMonoGEAREntryPointClass_Instance_Variable_HANDLE;
	static MonoObject*		g_pMonoGEAREntryPointClass_Instance_Variable;
	static MonoAssembly*	g_pMonoAssembly;
	static MonoImage*		g_pImage;
	static MonoClass*		g_pMonoGEAREntryPointClass;
	static MonoClass*		g_pMonoobject3d;
	static MonoClass*		g_pMonoScript;

	//USER ASSEMBLY
	static MonoAssembly*	g_pUserMonoAssembly;
	static MonoImage*		g_pUserImage;

	//ENGIINE INTERFACES
	static MonoMethod* g_monogear_engine_test_function_for_mono;
	static MonoMethod* g_mono_game_start;
	static MonoMethod* g_mono_game_run;
	static MonoMethod* g_mono_game_onkeydown;
	static MonoMethod* g_mono_game_onkeyup;

	static MonoMethod* g_pMethod_engine_init;
	static MonoMethod* g_pMethod_engine_init_for_mono_android;	
	static MonoMethod* g_pMethod_engine_getWorld;
	static MonoMethod* g_pMethod_engine_update;
	static MonoMethod* g_pMethod_engine_resize;
	static MonoMethod* g_pMethod_engine_render;
	static MonoMethod* g_pMethod_engine_renderSingleObject;

	static MonoMethod* g_pMethod_engine_mouseLButtonDown;
	static MonoMethod* g_pMethod_engine_mouseLButtonUp;
	static MonoMethod* g_pMethod_engine_mouseRButtonDown;
	static MonoMethod* g_pMethod_engine_mouseRButtonUp;
	static MonoMethod* g_pMethod_engine_mouseWheel;
	static MonoMethod* g_pMethod_engine_mouseMove;
	static MonoMethod* g_pMethod_engine_setMetaFolder;
	static MonoMethod* g_pMethod_engine_removeObject3d;
	static MonoMethod* g_pMethod_engine_destroyObject3d;

	static MonoMethod* g_mono_object3d_onObject3dChildAppend;
	static MonoMethod* g_mono_object3d_onObject3dChildRemove;
	//

    static bool g_isSimulationRunning;
	//scripts
	static std::vector<std::string> g_monoscriptlist;
	static std::vector<monoScript*> g_monoScriptClassDefs;
    static object3d* g_null_obj;
};