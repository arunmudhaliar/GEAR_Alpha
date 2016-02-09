#include "monoScript.h"

monoScript::monoScript(std::string script, MonoDomain* pMonoDomain, MonoClass* klass, std::string klassname, std::string knamespace, MonoClass* monoscript_klass)
{
	scriptFileName=script;
	monoObjectClass = klass;	
	monoClassName = klassname;
	monoNameSpace = knamespace;
	monoDomain = pMonoDomain;
	monoMethod_setHandle = NULL;
	isValidScript=false;
	if(mono_class_is_subclass_of(monoObjectClass, monoscript_klass, false))
	{
		isValidScript=true;
		monoMethod_setHandle =  mono_class_get_method_from_name(monoscript_klass, "setHandle", 2);

		for(int x=0;x<mono_class_num_fields(monoObjectClass);x++)
		{
			MonoClassField* classfiled = mono_class_get_fields(monoObjectClass, (void**)&x);
			monoVariableList.push_back(classfiled);
		}
	}
}

const char* monoScript::getMonoVarName(int index)
{
	return mono_field_get_name(monoVariableList[index]);
}

MonoClassField* monoScript::getMonoVar(int index)
{
	return monoVariableList[index];
}

monoScript::~monoScript()
{
    typedef std::map<MonoObject*, unsigned int>::iterator it_type;
    for(it_type iterator = monoObjectList.begin(); iterator != monoObjectList.end(); iterator++)
    {
        mono_gchandle_free(iterator->second);
    }
    
    monoObjectList.clear();
	monoVariableList.clear();
}

MonoObject* monoScript::createNewObject()
{
    auto newmonoobj = mono_object_new(monoDomain, monoObjectClass);
    uint32_t newmonoobj_handle = mono_gchandle_new(newmonoobj, false);
    auto ungarbagecollected_mono_obj = mono_gchandle_get_target(newmonoobj_handle);
    mono_runtime_object_init(ungarbagecollected_mono_obj);
    monoObjectList[ungarbagecollected_mono_obj] = newmonoobj_handle;
    
	return ungarbagecollected_mono_obj;
}

/////////////////////////////////////////////////////////////////////

monoScriptObjectInstance::monoScriptObjectInstance(monoScript* script, MonoObject* monoobj, void* obj)
{
	monoScriptPtr = script;
	monoObjectInstance = monoobj;

	if(monoScriptPtr->isMonoScript())
	{
		void* args[2]={this, &obj};
		mono_runtime_invoke(monoScriptPtr->getSetHandleMethod(), monoObjectInstance, args, NULL);

		monoMethod_update =  mono_class_get_method_from_name(monoScriptPtr->getMonoClass(), "update", 0);
		monoMethod_initMonoScript =  mono_class_get_method_from_name(monoScriptPtr->getMonoClass(), "initMonoScript", 0);
        
        MonoObject* exception=nullptr;
		mono_runtime_invoke(monoMethod_initMonoScript, monoObjectInstance, NULL, &exception);
        if (exception) {
            printf ("An exception was thrown in monoScriptObjectInstance::monoScriptObjectInstance() at mono_runtime_invoke.\n");
        }
	}
	
}

monoScriptObjectInstance::~monoScriptObjectInstance()
{
}

void monoScriptObjectInstance::update()
{
	if(monoScriptPtr->isMonoScript())
	{
        //mono_thread_attach(mono_get_root_domain ());
        //printf("%p, %p\n", monoObjectInstance, monoMethod_update);
        MonoObject* exception=nullptr;
		mono_runtime_invoke(monoMethod_update, monoObjectInstance, NULL, &exception);
        if (exception) {
            printf ("An exception was thrown in monoScriptObjectInstance::update() at mono_runtime_invoke.\n");
        }
	}
}