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

	int getMonoVarCount()	{	return (int)monoVariableList.size();	}
	const char* getMonoVarName(int index);
	MonoClassField* getMonoVar(int index);

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
	std::vector<MonoClassField*> monoVariableList;
    std::map<MonoObject*, unsigned int> monoObjectList;
};

class monoScriptObjectInstance : public Ref
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
    
protected:
    monoScriptObjectInstance();
    monoScriptObjectInstance(monoClassDef* script, object3d* obj);

    virtual void update(float dt){};
    virtual void render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/){};

    virtual void readScriptObject(gxFile& file){}
    virtual void writeScriptObject(gxFile& file){}

private:
    object3d*       attachedObject;
	monoClassDef* monoScriptPtr;
	MonoObject* monoObjectInstance;
	MonoMethod* monoMethod_initMonoScript;
	MonoMethod* monoMethod_update;
};