#pragma once

#include <vector>
#include <map>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/threads.h>
#include <mono/jit/jit.h>
#include "../../core/basicIncludes.h"
#include "../../renderer/gxRenderer.h"
#include "../../core/object3d.h"
#include "../../animation/gxKeyFrameAnimationTrack.h"
#include <string>

struct stMonoScriptArgs
{
    std::string script;
    MonoDomain* domain;
    MonoClass* klass;
    std::string klassname;
    std::string knamespace;
    MonoClass* monoscript_klass;
};

class monoScriptObjectInstance;
class gxKeyFrameAnimationTrack;
class DECLSPEC monoClassDef : public Ref
{
public:
    static monoClassDef* create(const stMonoScriptArgs& args);
    
	~monoClassDef();
    
	const std::string& getScriptFileName()	{	return scriptFileName;	}
	MonoObject* createNewObject();
    bool destroyObject(MonoObject* obj);
    
	MonoClass* getMonoClass()	{	return monoObjectClass;	}

	bool isMonoScript()	{ 	return isValidMonoScript;	}
	MonoMethod* getSetHandleMethod()	{	return monoMethod_setHandle;	}

	static const char* getMonoFieldName(MonoClassField* field);
    static const char* getMonoFieldTypeName(MonoClassField* field);
    
    const std::vector<MonoClassField*>& getMonoFields()     {   return monoFieldList;   }
    const std::vector<MonoMethod*>& getMonoMethods()        {   return monoMethodList;  }
    const std::vector<MonoProperty*>& getMonoProperties()   {   return monoPropertyList;}
    const char* getPropertyName(MonoProperty* property);
    
    static MonoMethod* getPropertyGetMethod(MonoProperty* property);
    static MonoMethod* getPropertySetMethod(MonoProperty* property);
    static MonoType* getPropertyGetMethodType(MonoProperty* property);
    static MonoType* getPropertySetMethodType(MonoProperty* property);

    static const char* getMonoPropertyTypeName(MonoType* type);

protected:
    monoClassDef(const stMonoScriptArgs& args);
    monoClassDef(std::string script, MonoDomain* pMonoDomain, MonoClass* klass, std::string klassname, std::string knamespace, MonoClass* monoscript_klass);
    
    void init(const stMonoScriptArgs& args);

private:
    monoClassDef(){}
    
	std::string scriptFileName;
	std::string monoClassName;
	std::string monoNameSpace;
	bool isValidMonoScript;

	MonoClass* monoObjectClass;
	MonoDomain* monoDomain;
	MonoMethod* monoMethod_setHandle;

    std::vector<MonoProperty*> monoPropertyList;
    std::vector<MonoMethod*> monoMethodList;
	std::vector<MonoClassField*> monoFieldList;
    std::map<MonoObject*, unsigned int> monoObjectList;
    
    std::map<MonoClassField*, gxKeyFrameAnimationTrack*> animationKeyFrameMapForClassFields;
    std::map<MonoProperty*, gxKeyFrameAnimationTrack*> animationKeyFrameMapForProperties;
    
};

extern "C" {
    DECLSPEC object3d* monoScriptObjectInstance_getAttachedObject(monoScriptObjectInstance* instance);
}

class DECLSPEC monoScriptObjectInstance : public Ref
{
public:
	virtual ~monoScriptObjectInstance();

    static monoScriptObjectInstance* create(monoClassDef* script, object3d* obj);
    
	monoClassDef* getScriptPtr()      {	return monoScriptPtr;	}
    object3d* getAttachedObject()   {   return attachedObject;  }
    
    void start();
	void update();
    void stop();
    
    void read(gxFile& file);
    void write(gxFile& file);

    virtual void update(float dt){};
    virtual void render(gxRenderer* renderer, monoScriptObjectInstance* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/){};

protected:
	monoScriptObjectInstance(){}
    monoScriptObjectInstance(monoClassDef* script, object3d* obj);

    virtual void readScriptObject(gxFile& file){}
    virtual void writeScriptObject(gxFile& file){}

private:
    object3d*       attachedObject;
	monoClassDef* monoScriptPtr;
	MonoObject* monoObjectInstance;
	MonoMethod* monoMethod_initMonoScript;
	MonoMethod* monoMethod_update;
};
