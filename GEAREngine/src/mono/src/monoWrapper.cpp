#include "monoWrapper.h"

#include <sys/types.h>
#include <assert.h>
#include <dirent.h>
#ifdef _WIN32
#include <direct.h>
#endif
#include <limits.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <CommCtrl.h>
#include "../../util/gxUtil.h"
//#include "../../GEAREditor/EditorApp.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#endif

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
MonoMethod* monoWrapper::g_pMethod_engine_init_for_mono_android = NULL;	
MonoMethod* monoWrapper::g_pMethod_engine_getWorld = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_update = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_resize = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_render = NULL;
MonoMethod* monoWrapper::g_pMethod_engine_renderSingleObject = NULL;
//MonoMethod* monoWrapper::g_pMethod_engine_loadAndAppendFBX = NULL;
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

void monoWrapper::loadMonoModules()
{
#ifndef USEMONOENGINE
	return;
#endif

#ifdef _WIN32
	mono_set_dirs("C:/Mono-2.10.6/lib", "C:/Mono-2.10.6/etc"); 
#else
	mono_set_dirs("/storage/emulated/0/gear/", "/storage/emulated/0/gear/");
#endif

	mono_config_parse(NULL);
	g_pMonoDomain = mono_jit_init("system");
}

void monoWrapper::reInitMono(const char* projecthomedirectory)
{
#ifndef USEMONOENGINE
	return;
#endif
	//destroyMono();
#ifdef _WIN32
	std::vector<std::string> csharpfileslist;
	traverseForCSharpFiles(projecthomedirectory, &csharpfileslist);
	compileCSharpScripts(&csharpfileslist);
	csharpfileslist.clear();

#if _DEBUG
	const char* executableFile="./Debug/out.exe";
#else
	const char* executableFile="./Release/out.exe";
#endif
#else
	const char* executableFile="/storage/emulated/0/gear/out.exe";
#endif

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
}

#ifdef _WIN32
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
#endif

void monoWrapper::bindEngineMethods()
{
#ifndef USEMONOENGINE
	return;
#endif
	//use MonoObject* for return value
	g_monogear_engine_test_function_for_mono=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "monogear_engine_test_function_for_mono", 0);
	g_mono_game_start						=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "mono_game_start", 0);
	g_mono_game_run							=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "mono_game_run", 1);
	g_mono_game_onkeydown					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "mono_game_onkeydown", 2);
	g_mono_game_onkeyup						=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "mono_game_onkeyup", 2);

	g_pMethod_engine_init					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_init", 1);
	g_pMethod_engine_init_for_mono_android	=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "monogear_engine_init_for_mono_android", 0);
	g_pMethod_engine_getWorld				=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_getWorld", 1);
	g_pMethod_engine_update					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_update", 2);
	g_pMethod_engine_resize					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_resize", 7);
	g_pMethod_engine_render					=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_render", 2);
	g_pMethod_engine_renderSingleObject		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_renderSingleObject", 3);
	//g_pMethod_engine_loadAndAppendFBX		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_loadAndAppendFBX", 2);
	g_pMethod_engine_loadFBX				=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_loadFBX", 3);
	g_pMethod_engine_appendObject3dToRoot	=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_appendObject3dToRoot", 2);
	g_pMethod_engine_mouseLButtonDown		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseLButtonDown", 4);
	g_pMethod_engine_mouseLButtonUp			=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseLButtonUp", 4);
	g_pMethod_engine_mouseRButtonDown		=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseRButtonDown", 4);
	g_pMethod_engine_mouseWheel				=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseWheel", 5);
	g_pMethod_engine_mouseMove				=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_mouseMove", 4);
	g_pMethod_engine_setMetaFolder			=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_setMetaFolder", 2);
	g_pMethod_engine_loadTextureFromFile	=  mono_class_get_method_from_name(g_pMonoGEAREntryPointClass, "engine_loadTextureFromFile", 4);
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
	if(g_pMonoDomain)
	{
		mono_jit_cleanup(g_pMonoDomain);
		g_pMonoDomain=NULL;
	}
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
	return false;
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
	return false;
}

//MONO ENGINE WRAPPERS

int monoWrapper::mono_engine_test_function_for_mono()
{
#ifdef USEMONOENGINE
	MonoObject* returnValue=mono_runtime_invoke(g_monogear_engine_test_function_for_mono, NULL, NULL, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);

	//don't know this casting is right or wrong		- arun-check
	return (int)underlyingType;
#endif
	return 0;
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

void monoWrapper::mono_engine_init_for_mono_android()
{
#ifdef USEMONOENGINE
	mono_runtime_invoke(g_pMethod_engine_init_for_mono_android, NULL, NULL, NULL);
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

void monoWrapper::mono_engine_resize(gxWorld* world, float x, float y, float cx, float cy, float nearplane, float farplane)
{
#ifdef USEMONOENGINE
	void* args[7]={&world, &x, &y, &cx, &cy, &nearplane, &farplane};
	mono_runtime_invoke(g_pMethod_engine_resize, NULL, args, NULL);
#else
	engine_resize(world, x, y, cx, cy, nearplane, farplane);
#endif
}

void monoWrapper::mono_engine_render(gxWorld* world, object3d* light)
{
#ifdef USEMONOENGINE
	object3d null_obj(999);
	object3d* lightPtr=light;
	if(lightPtr==NULL)
		lightPtr=&null_obj;
	void* args[2]={&world, &lightPtr};
	mono_runtime_invoke(g_pMethod_engine_render, NULL, args, NULL);
#else
	engine_render(world, light);
#endif
}

void monoWrapper::mono_engine_renderSingleObject(gxWorld* world, object3d* obj, object3d* light)
{
#ifdef USEMONOENGINE
	object3d null_obj(999);
	object3d* lightPtr=light;
	if(lightPtr==NULL)
		lightPtr=&null_obj;
	void* args[3]={&world, &obj, &lightPtr};
	mono_runtime_invoke(g_pMethod_engine_renderSingleObject, NULL, args, NULL);
#else
	engine_renderSingleObject(world, obj, light);
#endif
}

//object3d* monoWrapper::mono_engine_loadAndAppendFBX(gxWorld* world, const char* filename)
//{
//#ifdef USEMONOENGINE
//	void* args[2]={&world, mono_string_new(g_pMonoDomain, filename)};
//	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_loadAndAppendFBX, NULL, args, NULL);
//	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);
//	return (object3d*)underlyingType;
//#else
//	return engine_loadAndAppendFBX(world, filename);
//#endif
//}

object3d* monoWrapper::mono_engine_loadFBX(gxWorld* world, const char* filename, const char* projecthomedirectory)
{
#ifdef USEMONOENGINE
	void* args[3]={&world, mono_string_new(g_pMonoDomain, filename), mono_string_new(g_pMonoDomain, projecthomedirectory)};
	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_loadFBX, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);
	return (object3d*)underlyingType;
#else
	return engine_loadFBX(world, filename, projecthomedirectory);
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

gxTexture* monoWrapper::mono_engine_loadTextureFromFile(gxWorld* world, gxMaterial* material, const char* filename, int submap)
{
#ifdef USEMONOENGINE
	void* args[4]={&world, &material, mono_string_new(g_pMonoDomain, filename), &submap};
	MonoObject* returnValue=mono_runtime_invoke(g_pMethod_engine_loadTextureFromFile, NULL, args, NULL);
	MonoType *underlyingType = *(MonoType **) mono_object_unbox(returnValue);
	return (gxTexture*)underlyingType;
#else
	return engine_loadTextureFromFile(world, material, filename, submap);
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

#ifdef _WIN32
int monoWrapper::traverseForCSharpFiles(const char *dirname, std::vector<std::string>* csharpfilelist)
{
    DIR *dir;
    char buffer[PATH_MAX + 2];
    char *p = buffer;
    const char *src;
    char *end = &buffer[PATH_MAX];
    int ok;

    /* Copy directory name to buffer */
    src = dirname;
    while (p < end  &&  *src != '\0') {
        *p++ = *src++;
    }
    *p = '\0';

    /* Open directory stream */
    dir = opendir (dirname);
    if (dir != NULL) {
        struct dirent *ent;

        /* Print all files and directories within the directory */
        while ((ent = readdir (dir)) != NULL) {
            char *q = p;
            char c;

            /* Get final character of directory name */
            if (buffer < q) {
                c = q[-1];
            } else {
                c = ':';
            }

            /* Append directory separator if not already there */
            if (c != ':'  &&  c != '/'  &&  c != '\\') {
                *q++ = '/';
            }

            /* Append file name */
            src = ent->d_name;
            while (q < end  &&  *src != '\0') {
                *q++ = *src++;
            }
            *q = '\0';

            /* Decide what to do with the directory entry */
            switch (ent->d_type) {
            case DT_REG:
                {
					if(gxUtil::GX_IS_EXTENSION(buffer, ".cs") || gxUtil::GX_IS_EXTENSION(buffer, ".CS"))
					{
						csharpfilelist->push_back(buffer);
					}
				}
                break;

            case DT_DIR:
                /* Scan sub-directory recursively */
                if (strcmp (ent->d_name, ".") != 0  &&  strcmp (ent->d_name, "..") != 0)
				{
                    traverseForCSharpFiles(buffer, csharpfilelist);
                }
                break;

            default:
                /* Do not device entries */
                /*NOP*/;
            }

        }

        closedir (dir);
        ok = 1;

    } else {
        /* Could not open directory */
        printf ("Cannot open directory %s\n", dirname);
        ok = 0;
    }

    return ok;
}

bool monoWrapper::compileCSharpScripts(std::vector<std::string>* list)
{
	std::string command_buffer;
	command_buffer = "gmcs ";

	for(std::vector<std::string>::iterator it = list->begin(); it != list->end(); ++it)
	{
		std::string csharpfile = *it;
		command_buffer += csharpfile +" ";
	}

	//command_buffer += "-o "+EditorApp::getProjectHomeDirectory()+"/MetaData/out.exe";

#if _DEBUG
	command_buffer += "-v -o Debug//out.exe";
#else
	command_buffer += "-v -o Release//out.exe";
#endif

	char responsebuffer[4096];
	exec_cmd(command_buffer.c_str(), responsebuffer);
	printf("\n================GMCS COMPILATION RESULT===============\n");
	printf(responsebuffer);
	printf("\n======================================================\n");

	return true;
}

char monoWrapper::exec_cmd(char const *cmd, char *buf)
{
	char output[1024*10], start[1024*10];
	char *s;
	FILE *fpo;
	int size;
	int ret;
	if((fpo = _popen(cmd, "r") )== NULL)
	{
		sprintf(start, "error");
		size = 6;
	}
	else
	{
		sprintf(start, "");
		size =0;
		while((s =fgets(output, 1024, fpo)) != NULL)
		{
			strcat(start, output);
			size += (strlen(output)+1);
			if(output == NULL)
				break;
		}
	}
	strcpy(buf, start);
	ret = _pclose(fpo);
	return (ret);
}/* exec_cmd */
#endif