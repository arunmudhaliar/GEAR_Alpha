#include "monoClassDef.h"

monoClassDef* monoClassDef::create(const stMonoScriptArgs& args)
{
    auto newObject = new monoClassDef(args);
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

monoClassDef::monoClassDef(const stMonoScriptArgs& args):
    Ref()
{
    init(args);
}

monoClassDef::monoClassDef(std::string script, MonoDomain* pMonoDomain, MonoClass* klass, std::string klassname, std::string knamespace, MonoClass* monoscript_klass):
    Ref()
{
    stMonoScriptArgs args;
    args.script=script;
    args.domain = pMonoDomain;
    args.klass = klass;
    args.klassname=klassname;
    args.knamespace=knamespace;
    args.monoscript_klass=monoscript_klass;
    init(args);
}

void monoClassDef::init(const stMonoScriptArgs& args)
{
    scriptFileName=args.script;
    monoObjectClass = args.klass;
    monoClassName = args.klassname;
    monoNameSpace = args.knamespace;
    monoDomain = args.domain;
    monoMethod_setHandle = NULL;
    isValidMonoScript=false;
    
    if(mono_class_is_subclass_of(monoObjectClass, args.monoscript_klass, false))
    {
        isValidMonoScript=true;
        monoMethod_setHandle =  mono_class_get_method_from_name(args.monoscript_klass, "setHandle", 1);
    }
    
    //    DEBUG_PRINT("------------------Mono Class Definitions (%s)------------------", klassname.c_str());
    //    DEBUG_PRINT("monoClassDef class(%s) : no of methods : %d", klassname.c_str(), mono_class_num_methods(monoObjectClass));
    //    DEBUG_PRINT("monoClassDef class(%s) : no of fields : %d", klassname.c_str(), mono_class_num_fields(monoObjectClass));
    //    DEBUG_PRINT("monoClassDef class(%s) : no of properties : %d", klassname.c_str(), mono_class_num_properties(monoObjectClass));
    
    //properties
    void** iter_property[mono_class_num_methods(monoObjectClass)];
    memset(iter_property, 0, sizeof(iter_property));
    for(int x=0;x<mono_class_num_properties(monoObjectClass);x++)
    {
        MonoProperty* property = mono_class_get_properties(monoObjectClass, (void**)iter_property);
        if(!property)
        {
            continue;
        }
        
        monoPropertyList.push_back(property);
        //        DEBUG_PRINT("property : %s", mono_property_get_name(property));
    }
    
    //method
    void** iter_method[mono_class_num_methods(monoObjectClass)];
    memset(iter_method, 0, sizeof(iter_method));
    for(int x=0;x<mono_class_num_methods(monoObjectClass);x++)
    {
        
        MonoMethod* method = mono_class_get_methods(monoObjectClass, (void**)iter_method);
        if(!method)
        {
            continue;
        }
        
        monoMethodList.push_back(method);
        //        DEBUG_PRINT("method : %s", mono_method_get_name(method));
    }
    
    //field
    void** iter_field[mono_class_num_fields(monoObjectClass)];
    memset(iter_field, 0, sizeof(iter_field));
    for(int x=0;x<mono_class_num_fields(monoObjectClass);x++)
    {
        MonoClassField* classfiled = mono_class_get_fields(monoObjectClass, (void**)iter_field);
        if(!classfiled)
        {
            continue;   //THIS SHOULD NEVER HAPPEN. THIS MEANS THE BINARY IS NOT COMPILED PROPER.
        }
        
        monoVariableList.push_back(classfiled);
        //        DEBUG_PRINT("field : %s", mono_field_get_name(classfiled));
    }
}

const char* monoClassDef::getMonoVarName(int index)
{
	return mono_field_get_name(monoVariableList[index]);
}

MonoClassField* monoClassDef::getMonoVar(int index)
{
	return monoVariableList[index];
}

const char* monoClassDef::getMonoVarTypeName(int index)
{
    auto mvar = getMonoVar(index);
    auto mtype = mono_field_get_type(mvar);
    return mono_type_get_name(mtype);
}

monoClassDef::~monoClassDef()
{
    typedef std::map<MonoObject*, unsigned int>::iterator it_type;
    for(it_type iterator = monoObjectList.begin(); iterator != monoObjectList.end(); iterator++)
    {
        mono_gchandle_free(iterator->second);
    }
    
    monoObjectList.clear();
	monoVariableList.clear();
}

MonoObject* monoClassDef::createNewObject()
{
    auto newmonoobj = mono_object_new(monoDomain, monoObjectClass);
    uint32_t newmonoobj_handle = mono_gchandle_new(newmonoobj, false);
    auto ungarbagecollected_mono_obj = mono_gchandle_get_target(newmonoobj_handle);
    mono_runtime_object_init(ungarbagecollected_mono_obj);
    monoObjectList[ungarbagecollected_mono_obj] = newmonoobj_handle;
    
	return ungarbagecollected_mono_obj;
}

bool monoClassDef::destroyObject(MonoObject* obj)
{
    auto it = monoObjectList.find(obj);
    if(it->first==obj)
    {
        mono_gchandle_free(it->second);
        monoObjectList.erase(it);
        return true;
    }
    
    return false;
}

/////////////////////////////////////////////////////////////////////

extern "C" {
    extern DECLSPEC object3d* monoScriptObjectInstance_getAttachedObject(monoScriptObjectInstance* instance)
    {
        return instance->getAttachedObject();
    }
}

monoScriptObjectInstance* monoScriptObjectInstance::create(monoClassDef* script, object3d* obj)
{
    auto newObject = new monoScriptObjectInstance(script, obj);
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

monoScriptObjectInstance::monoScriptObjectInstance(monoClassDef* script, object3d* obj):
    Ref()
{
	monoScriptPtr = script;
    attachedObject = obj;
	monoObjectInstance = nullptr;

	if(monoScriptPtr->isMonoScript())
	{
		monoMethod_update =  mono_class_get_method_from_name(monoScriptPtr->getMonoClass(), "update", 0);
		monoMethod_initMonoScript =  mono_class_get_method_from_name(monoScriptPtr->getMonoClass(), "initMonoScript", 0);
	}
}

monoScriptObjectInstance::~monoScriptObjectInstance()
{
}

void monoScriptObjectInstance::start()
{
    if(monoScriptPtr->isMonoScript())
    {
        if(monoObjectInstance==nullptr)
        {
            monoObjectInstance = monoScriptPtr->createNewObject();
            void* args[1]={this};
            mono_runtime_invoke(monoScriptPtr->getSetHandleMethod(), monoObjectInstance, args, NULL);
        }
        
        MonoObject* exception=nullptr;
        mono_runtime_invoke(monoMethod_initMonoScript, monoObjectInstance, NULL, &exception);
        if (exception) {
            printf ("An exception was thrown in monoScriptObjectInstance::monoScriptObjectInstance() at mono_runtime_invoke.\n");
        }
    }
}

void monoScriptObjectInstance::stop()
{
    if(monoScriptPtr->isMonoScript())
    {
        if(monoObjectInstance)
        {
            monoScriptPtr->destroyObject(monoObjectInstance);
            monoObjectInstance = nullptr;
        }
    }
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

void monoScriptObjectInstance::read(gxFile& file)
{
    readScriptObject(file);
}

void monoScriptObjectInstance::write(gxFile& file)
{
    writeScriptObject(file);
}
