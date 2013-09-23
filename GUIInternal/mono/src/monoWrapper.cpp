#include "monoWrapper.h"
//#include <cstdlib>
//#include <cstdio>
//#include <fstream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>

MonoObject*		monoWrapper::g_pMonoGEAREntryPointClass_Instance_Variable = NULL;
MonoDomain*		monoWrapper::g_pMonoDomain = NULL;
MonoAssembly*	monoWrapper::g_pMonoAssembly = NULL;
MonoImage*		monoWrapper::g_pImage = NULL;
MonoClass*		monoWrapper::g_pMonoGEAREntryPointClass = NULL;
MonoClass*		monoWrapper::g_pMonoobject3d = NULL;

MonoMethod* monoWrapper::g_monogear_engine_test_function_for_mono=NULL;
MonoMethod* monoWrapper::g_mono_game_start = NULL;
MonoMethod* monoWrapper::g_mono_game_run = NULL;
MonoMethod* monoWrapper::g_mono_game_onkeydown = NULL;
MonoMethod* monoWrapper::g_mono_game_onkeyup = NULL;

MonoMethod* monoWrapper::g_pMethod_engine_init = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_getWorld = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_update = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_resize = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_render = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_renderSingleObject = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_loadAndAppendFBX = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_loadFBX = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_appendObject3dToRoot = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_mouseLButtonDown = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_mouseLButtonUp = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_mouseRButtonDown = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_mouseRButtonUp = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_mouseWheel = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_mouseMove = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_setMetaFolder = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_loadTextureFromFile = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_removeObject3d = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_destroyObject3d = NULL;

MonoMethod* monoWrapper::g_mono_object3d_onObject3dChildAppend = NULL;
MonoMethod* monoWrapper::g_mono_object3d_onObject3dChildRemove = NULL;
	
void monoWrapper::initMono()
{
#ifndef USEMONOENGINE
	return;
#endif

	initDebugConsole();
	//AllocConsole();

 //   HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
 //   int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
 //   FILE* hf_out = _fdopen(hCrt, "w");
 //   setvbuf(hf_out, NULL, _IONBF, 1);
 //   *stdout = *hf_out;

 //   HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
 //   hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
 //   FILE* hf_in = _fdopen(hCrt, "r");
 //   setvbuf(hf_in, NULL, _IONBF, 128);
 //   *stdin = *hf_in;

//	FILE *fp;
 //    char *command;
	// char path[256];
     /* command contains the command string (a character array) */

     /* If you want to read output from command */
     //fp = _popen("C:\\Mono-2.10.6\\bin\\mcs.bat D:\\MYPROJECTS\\TEMP_PROJ\\TestProj\\monotest\\scripts\\LineObject.cs D:\\MYPROJECTS\\TEMP_PROJ\\TestProj\\monotest\\scripts\\testScript.cs -o D:\\MYPROJECTS\\TEMP_PROJ\\TestProj\\monotest\\scripts\\output.exe","r"); 
    //fp = _popen("start C:\\Mono-2.10.6\\bin\\mcs.bat -v","rt"); 
    //fp = _popen("C:\\Mono-2.10.6\\bin\\mono.exe -v","rt"); 
    // fp = _popen("cmd","rt"); 
     /* read output from command */
     
    //printf("%s", path);
     
	 //fclose(fp);

	/*
	 * Load the default Mono configuration file, this is needed
	 * if you are planning on using the dllmaps defined on the
	 * system configuration
	 */

	mono_set_dirs("C:/Mono-2.10.6/lib", "C:/Mono-2.10.6/etc"); 
	mono_config_parse(NULL);

	/*
	 * mono_jit_init() creates a domain: each assembly is
	 * loaded and run in a MonoDomain.
	 */

	//	MonoDomain *domain1 = mono_jit_init ("system");
#if _DEBUG
	const char* executableFile="./Debug/MonoGEAR.exe";
#else
	const char* executableFile="./Release/MonoGEAR.exe";
#endif
	//executableFile="D:\\MYPROJECTS\\TEMP_PROJ\\TestProj\\monotest\\test.exe";

	g_pMonoDomain = mono_jit_init(executableFile);
	//MonoDomain *domain = mono_jit_init_version ("test", "v2.0.50727");

	/*
	 * Optionally, add an internal call that your startup.exe
	 * code can call, this will bridge startup.exe to Mono
	 */
	//mono_add_internal_call ("test::globalFoo", getMessage);

	/*
	 * Open the executable, and run the Main method declared
	 * in the executable
	 */
	//"D:\\MYPROJECTS\\TEMP_PROJ\\TestProj\\monotest\\scripts\\out.exe"
	g_pMonoAssembly = mono_domain_assembly_open(g_pMonoDomain, executableFile);

	//if (!assembly)
	//	exit (2);
	/*
	 * mono_jit_exec() will run the Main() method in the assembly.
	 * The return value needs to be looked up from
	 * System.Environment.ExitCode.
	 */

	g_pImage = mono_assembly_get_image(g_pMonoAssembly);
	g_pMonoGEAREntryPointClass = mono_class_from_name (g_pImage, "MonoGEAR", "MonoGEAREntryPointClass");
	g_pMonoobject3d = mono_class_from_name (g_pImage, "MonoGEAR", "object3d");

	///* allocate memory for the object */
	g_pMonoGEAREntryPointClass_Instance_Variable = mono_object_new(g_pMonoDomain, g_pMonoGEAREntryPointClass);

	bindEngineMethods();
 //   /* execute the default argument-less constructor */
	mono_runtime_object_init(g_pMonoGEAREntryPointClass_Instance_Variable);

	//int argc =1;
	//char *args[1];
 //   args[0] = "Test Param"; 

	//int retVal = mono_jit_exec(g_pMonoDomain, g_pMonoAssembly, argc, args);
	//retVal=1;


	//FreeConsole();
}


void monoWrapper::initDebugConsole()
{
	AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;
}

void monoWrapper::destroyDebugConsole()
{
	FreeConsole();
}

void monoWrapper::bindEngineMethods()
{
#ifndef USEMONOENGINE
	return;
#endif
	//use MonoObject* for return value
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

	g_monogear_engine_test_function_for_mono=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "monogear_engine_test_function_for_mono", 0);
	g_mono_game_start						=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "mono_game_start", 0);
	g_mono_game_run							=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "mono_game_run", 1);
	g_mono_game_onkeydown					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "mono_game_onkeydown", 2);
	g_mono_game_onkeyup						=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "mono_game_onkeyup", 2);

	g_pMethod_engine_init					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_init", 1);
	g_pMethod_engine_getWorld				=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_getWorld", 1);
	g_pMethod_engine_update					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_update", 2);
	g_pMethod_engine_resize					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_resize", 5);
	g_pMethod_engine_render					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_render", 1);
	g_pMethod_engine_renderSingleObject		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_renderSingleObject", 2);
	g_pMethod_engine_loadAndAppendFBX		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_loadAndAppendFBX", 2);
	g_pMethod_engine_loadFBX				=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_loadFBX", 2);
	g_pMethod_engine_appendObject3dToRoot	=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_appendObject3dToRoot", 2);
	g_pMethod_engine_mouseLButtonDown		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseLButtonDown", 4);
	g_pMethod_engine_mouseLButtonUp			=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseLButtonUp", 4);
	g_pMethod_engine_mouseRButtonDown		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseRButtonDown", 4);
	g_pMethod_engine_mouseWheel				=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseWheel", 5);
	g_pMethod_engine_mouseMove				=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseMove", 4);
	g_pMethod_engine_setMetaFolder			=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_setMetaFolder", 2);
	g_pMethod_engine_loadTextureFromFile	=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_loadTextureFromFile", 3);
	g_pMethod_engine_removeObject3d			=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_removeObject3d", 2);
	g_pMethod_engine_destroyObject3d		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_destroyObject3d", 2);

	g_mono_object3d_onObject3dChildAppend =  mono_class_get_method_from_name(g_pMonoobject3d, "mono_object3d_onObject3dChildAppend", 2);
	g_mono_object3d_onObject3dChildRemove =  mono_class_get_method_from_name(g_pMonoobject3d, "mono_object3d_onObject3dChildRemove", 2);
}

void monoWrapper::updateMono()
{
	//mono_runtime_invoke(method, my_class_instance, NULL, NULL);
}

void monoWrapper::destroyMono()
{
#ifdef USEMONOENGINE
	mono_jit_cleanup(g_pMonoDomain);
	destroyDebugConsole();
#endif
}

//MONO GAME WRAPPERS
void monoWrapper::mono_game_start()
{
#ifdef USEMONOENGINE
	mono_runtime_invoke(g_mono_game_start, g_pMonoGEAREntryPointClass_Instance_Variable, NULL, NULL);
#endif
}

void monoWrapper::mono_game_run(float dt)
{
#ifdef USEMONOENGINE
	void* args[1]={&dt};
	mono_runtime_invoke(g_mono_game_run, g_pMonoGEAREntryPointClass_Instance_Variable, args, NULL);
#endif
}

bool monoWrapper::mono_game_onkeydown(int charValue, int flag)
{
#ifdef USEMONOENGINE
	void* args[2]={&charValue, &flag};
	MonoObject* returnValue=mono_runtime_invoke(g_mono_game_onkeydown, g_pMonoGEAREntryPointClass_Instance_Variable, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);

	//don't know this casting is right or wrong		- arun-check
	return (bool)underlyingType;
#endif
}

bool monoWrapper::mono_game_onkeyup(int charValue, int flag)
{
	#ifdef USEMONOENGINE
	void* args[2]={&charValue, &flag};
	MonoObject* returnValue=mono_runtime_invoke(g_mono_game_onkeyup, g_pMonoGEAREntryPointClass_Instance_Variable, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);

	//don't know this casting is right or wrong		- arun-check
	return (bool)underlyingType;
#endif
}

//MONO ENGINE WRAPPERS

void monoWrapper::mono_engine_test_function_for_mono()
{
#ifdef USEMONOENGINE
	mono_runtime_invoke(g_monogear_engine_test_function_for_mono, NULL, NULL, NULL);
#endif
}

void monoWrapper::mono_engine_init(int nWorldToCreate)
{
#ifdef USEMONOENGINE
	void* args[1]={&nWorldToCreate};
	mono_runtime_invoke(g_pMethod_engine_init, NULL, args, NULL);
#else
	engine_init(nWorldToCreate);
#endif
}

gxWorld* monoWrapper::mono_engine_getWorld(int index)
{
#ifdef USEMONOENGINE
	void* args[1]={&index};
	MonoObject* returnValue = mono_runtime_invoke(g_pMethod_engine_getWorld, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);
	gxWorld* world=(gxWorld*)underlyingType;
	return world;
#else
	return engine_getWorld(index);
#endif
}

void monoWrapper::mono_engine_update(gxWorld* world, float dt)
{
#ifdef USEMONOENGINE
	void* args[2]={&world, &dt};
	mono_runtime_invoke(g_pMethod_engine_update, NULL, args, NULL);
#else
	engine_update(world, dt);
#endif
}

void monoWrapper::mono_engine_resize(gxWorld* world, float x, float y, float cx, float cy)
{
#ifdef USEMONOENGINE
	void* args[5]={&world, &x, &y, &cx, &cy};
	mono_runtime_invoke(g_pMethod_engine_resize, NULL, args, NULL);
#else
	engine_resize(world, x, y, cx, cy);
#endif
}

void monoWrapper::mono_engine_render(gxWorld* world)
{
#ifdef USEMONOENGINE
	void* args[1]={&world};
	mono_runtime_invoke(g_pMethod_engine_render, NULL, args, NULL);
#else
	engine_render(world);
#endif
}

void monoWrapper::mono_engine_renderSingleObject(gxWorld* world, object3d* obj)
{
#ifdef USEMONOENGINE
	void* args[2]={&world, &obj};
	mono_runtime_invoke(g_pMethod_engine_renderSingleObject, NULL, args, NULL);
#else
	engine_renderSingleObject(world, obj);
#endif
}

object3d* monoWrapper::mono_engine_loadAndAppendFBX(gxWorld* world, const char* filename)
{
#ifdef USEMONOENGINE
	void* args[2]={&world, mono_string_new(g_pMonoDomain, filename)};
	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_loadAndAppendFBX, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);
	return (object3d*)underlyingType;
#else
	return engine_loadAndAppendFBX(world, filename);
#endif
}

object3d* monoWrapper::mono_engine_loadFBX(gxWorld* world, const char* filename)
{
#ifdef USEMONOENGINE
	void* args[2]={&world, mono_string_new(g_pMonoDomain, filename)};
	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_loadFBX, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);
	return (object3d*)underlyingType;
#else
	return engine_loadFBX(world, filename);
#endif
}

object3d* monoWrapper::mono_engine_appendObject3dToRoot(gxWorld* world, object3d* obj)
{
#ifdef USEMONOENGINE
	void* args[2]={&world, &obj};
	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_appendObject3dToRoot, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);
	return (object3d*)underlyingType;
#else
	return engine_appendObject3dToRoot(world, obj);
#endif
}

void monoWrapper::mono_engine_mouseLButtonDown(gxWorld* world, int x, int y, int flag)
{
#ifdef USEMONOENGINE
	void* args[4]={&world, &x, &y, &flag};
	mono_runtime_invoke(g_pMethod_engine_mouseLButtonDown, NULL, args, NULL);
#else
	engine_mouseLButtonDown(world, x, y, flag);
#endif
}

void monoWrapper::mono_engine_mouseLButtonUp(gxWorld* world, int x, int y, int flag)
{
#ifdef USEMONOENGINE
	void* args[4]={&world, &x, &y, &flag};
	mono_runtime_invoke(g_pMethod_engine_mouseLButtonUp, NULL, args, NULL);
#else
	engine_mouseLButtonUp(world, x, y, flag);
#endif
}

void monoWrapper::mono_engine_mouseRButtonDown(gxWorld* world, int x, int y, int flag)
{
#ifdef USEMONOENGINE
	void* args[4]={&world, &x, &y, &flag};
	mono_runtime_invoke(g_pMethod_engine_mouseRButtonDown, NULL, args, NULL);
#else
	engine_mouseRButtonDown(world, x, y, flag);
#endif
}

void monoWrapper::mono_engine_mouseRButtonUp(gxWorld* world, int x, int y, int flag)
{
#ifdef USEMONOENGINE
	void* args[4]={&world, &x, &y, &flag};
	mono_runtime_invoke(g_pMethod_engine_mouseRButtonUp, NULL, args, NULL);
#else
	engine_mouseRButtonUp(world, x, y, flag);
#endif
}

void monoWrapper::mono_engine_mouseWheel(gxWorld* world, int zDelta, int x, int y, int flag)
{
#ifdef USEMONOENGINE
	void* args[5]={&world, &zDelta, &x, &y, &flag};
	mono_runtime_invoke(g_pMethod_engine_mouseWheel, NULL, args, NULL);
#else
	engine_mouseWheel(world, zDelta, x, y, flag);
#endif
}

void monoWrapper::mono_engine_mouseMove(gxWorld* world, int x, int y, int flag)
{
#ifdef USEMONOENGINE
	void* args[4]={&world, &x, &y, &flag};
	mono_runtime_invoke(g_pMethod_engine_mouseMove, NULL, args, NULL);
#else
	engine_mouseMove(world, x, y, flag);
#endif
}

void monoWrapper::mono_engine_setMetaFolder(gxWorld* world, const char* metaFolder)
{
#ifdef USEMONOENGINE
	void* args[2]={&world, mono_string_new(g_pMonoDomain, metaFolder)};
	mono_runtime_invoke(g_pMethod_engine_setMetaFolder, NULL, args, NULL);
#else
	engine_setMetaFolder(world, metaFolder);
#endif
}

gxTexture* monoWrapper::mono_engine_loadTextureFromFile(gxWorld* world, gxMaterial* material, const char* filename)
{
#ifdef USEMONOENGINE
	void* args[3]={&world, &material, mono_string_new(g_pMonoDomain, filename)};
	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_loadTextureFromFile, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);
	return (gxTexture*)underlyingType;
#else
	return engine_loadTextureFromFile(world, material, filename);
#endif
}

bool monoWrapper::mono_engine_removeObject3d(gxWorld* world, object3d* obj)
{
#ifdef USEMONOENGINE
	void* args[2]={&world, &obj};
	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_removeObject3d, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);

	//don't know this casting is right or wrong		- arun-check
	return (bool)underlyingType;
#else
	return engine_removeObject3d(world, obj);
#endif
}

bool monoWrapper::mono_engine_destroyObject3d(gxWorld* world, object3d* obj)
{
#ifdef USEMONOENGINE
	void* args[2]={&world, &obj};
	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_destroyObject3d, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);

	//don't know this casting is right or wrong		- arun-check
	return (bool)underlyingType;
#else
	return engine_destroyObject3d(world, obj);
#endif
}

void monoWrapper::mono_object3d_onObject3dChildAppend(object3d* parent, object3d* child)
{
#ifdef USEMONOENGINE
	void* args[2]={&parent, &child};
	mono_runtime_invoke(g_mono_object3d_onObject3dChildAppend, NULL, args, NULL);
#endif
}

void monoWrapper::mono_object3d_onObject3dChildRemove(object3d* parent, object3d* child)
{
#ifdef USEMONOENGINE
	void* args[2]={&parent, &child};
	mono_runtime_invoke(g_mono_object3d_onObject3dChildRemove, NULL, args, NULL);
#endif
}
