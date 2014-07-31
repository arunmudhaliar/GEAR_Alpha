#ifndef MONOSCRIPT_H
#define MONOSCRIPT_H

#include <vector>
#include <mono\metadata\mono-config.h>
#include <mono\metadata\metadata.h>
#include <mono\metadata\assembly.h>
#include <mono\jit\jit.h>

class monoScript
{
public:
	monoScript(std::string script, MonoDomain* pMonoDomain, MonoClass* klass, std::string klassname, std::string knamespace);
	~monoScript();

	std::string getMonoScript()	{	return m_cScript;	}
	MonoObject* createNewObject();
	MonoClass* getMonoClass()	{	return m_pMonoObjectClass;	}

private:
	std::string m_cScript;
	std::string m_cMonoClassName;
	std::string m_cMonoNameSpace;

	MonoClass* m_pMonoObjectClass;
	MonoDomain* m_pMonoDomain;
};

class monoScriptObjectInstance
{
public:
	monoScriptObjectInstance(monoScript* script, MonoObject* monoobj);
	~monoScriptObjectInstance();

	monoScript* getScriptPtr()	{	return m_pScriptPtr;	}

	void update();
private:
	monoScript* m_pScriptPtr;
	MonoObject* m_pMonoObjectInstance;
	MonoMethod* m_pMethod_update;
};

#endif