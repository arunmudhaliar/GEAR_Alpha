#pragma once

#include <vector>
#include <map>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/threads.h>
#include <mono/jit/jit.h>
#include "../../core/basicIncludes.h"
#include <string>

class DECLSPEC monoScript
{
public:
	monoScript(std::string script, MonoDomain* pMonoDomain, MonoClass* klass, std::string klassname, std::string knamespace, MonoClass* monoscript_klass);
	~monoScript();

	std::string getScriptFileName()	{	return scriptFileName;	}
	MonoObject* createNewObject();
	MonoClass* getMonoClass()	{	return monoObjectClass;	}

	bool isMonoScript()	{ 	return isValidScript;	}
	MonoMethod* getSetHandleMethod()	{	return monoMethod_setHandle;	}

	int getMonoVarCount()	{	return (int)monoVariableList.size();	}
	const char* getMonoVarName(int index);
	MonoClassField* getMonoVar(int index);

private:
	std::string scriptFileName;
	std::string monoClassName;
	std::string monoNameSpace;
	bool isValidScript;

	MonoClass* monoObjectClass;
	MonoDomain* monoDomain;
	MonoMethod* monoMethod_setHandle;

	std::vector<MonoClassField*> monoVariableList;
    std::map<MonoObject*, unsigned int> monoObjectList;
};

class monoScriptObjectInstance
{
public:
	monoScriptObjectInstance(monoScript* script, MonoObject* monoobj, void* obj);
	~monoScriptObjectInstance();

	monoScript* getScriptPtr()	{	return monoScriptPtr;	}
	void update();
    
private:
	monoScript* monoScriptPtr;
	MonoObject* monoObjectInstance;
	MonoMethod* monoMethod_initMonoScript;
	MonoMethod* monoMethod_update;
};