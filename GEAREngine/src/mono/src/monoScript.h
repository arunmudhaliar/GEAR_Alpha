#ifndef MONOSCRIPT_H
#define MONOSCRIPT_H

#include <vector>
#include <mono\metadata\mono-config.h>
#include <mono\metadata\metadata.h>
#include <mono\metadata\assembly.h>
#include <mono\jit\jit.h>
#include "../../core/basicIncludes.h"
#include <string>

class DECLSPEC monoScript
{
public:
	monoScript(std::string script, MonoDomain* pMonoDomain, MonoClass* klass, std::string klassname, std::string knamespace, MonoClass* monoscript_klass);
	~monoScript();

	std::string getMonoScript()	{	return m_cScript;	}
	MonoObject* createNewObject();
	MonoClass* getMonoClass()	{	return m_pMonoObjectClass;	}

	bool isMonoScript()	{ 	return m_bMonoScript;	}
	MonoMethod* getSetHandleMethod()	{	return m_pSetHandle_method;	}

	int getMonoVarCount()	{	return m_vMonoVars.size();	}
	const char* getMonoVarName(int index);
	MonoClassField* getMonoVar(int index);

private:
	std::string m_cScript;
	std::string m_cMonoClassName;
	std::string m_cMonoNameSpace;
	bool m_bMonoScript;

	MonoClass* m_pMonoObjectClass;
	MonoDomain* m_pMonoDomain;
	MonoMethod* m_pSetHandle_method;

	std::vector<MonoClassField*> m_vMonoVars;
};

class monoScriptObjectInstance
{
public:
	monoScriptObjectInstance(monoScript* script, MonoObject* monoobj, void* obj);
	~monoScriptObjectInstance();

	monoScript* getScriptPtr()	{	return m_pScriptPtr;	}

	void update();
private:
	monoScript* m_pScriptPtr;
	MonoObject* m_pMonoObjectInstance;
	MonoMethod* m_pMethod_initMonoScript;
	MonoMethod* m_pMethod_update;
};

#endif