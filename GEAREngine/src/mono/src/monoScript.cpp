#include "monoScript.h"

monoScript::monoScript(std::string script, MonoDomain* pMonoDomain, MonoClass* klass, std::string klassname, std::string knamespace)
{
	m_cScript=script;
	m_pMonoObjectClass = klass;	
	m_cMonoClassName = klassname;
	m_cMonoNameSpace = knamespace;
	m_pMonoDomain = pMonoDomain;
}

monoScript::~monoScript()
{
}

MonoObject* monoScript::createNewObject()
{
	return mono_object_new(m_pMonoDomain, m_pMonoObjectClass);
}

/////////////////////////////////////////////////////////////////////

monoScriptObjectInstance::monoScriptObjectInstance(monoScript* script, MonoObject* monoobj)
{
	m_pScriptPtr = script;
	m_pMonoObjectInstance = monoobj;

	m_pMethod_update =  mono_class_get_method_from_name(script->getMonoClass(), "update", 0);

}

monoScriptObjectInstance::~monoScriptObjectInstance()
{
}

void monoScriptObjectInstance::update()
{
	mono_runtime_invoke(m_pMethod_update, m_pMonoObjectInstance, NULL, NULL);

}