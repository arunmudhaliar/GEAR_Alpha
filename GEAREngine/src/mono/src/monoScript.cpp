#include "monoScript.h"

monoScript::monoScript(std::string script, MonoDomain* pMonoDomain, MonoClass* klass, std::string klassname, std::string knamespace, MonoClass* monoscript_klass)
{
	m_cScript=script;
	m_pMonoObjectClass = klass;	
	m_cMonoClassName = klassname;
	m_cMonoNameSpace = knamespace;
	m_pMonoDomain = pMonoDomain;
	m_pSetHandle_method = NULL;
	m_bMonoScript=false;
	if(mono_class_is_subclass_of(m_pMonoObjectClass, monoscript_klass, false))
	{
		m_bMonoScript=true;
		m_pSetHandle_method =  mono_class_get_method_from_name(monoscript_klass, "setHandle", 2);

		for(int x=0;x<mono_class_num_fields(m_pMonoObjectClass);x++)
		{
			MonoClassField* classfiled = mono_class_get_fields(m_pMonoObjectClass, (void**)&x);
			m_vMonoVars.push_back(classfiled);
		}
	}
}

const char* monoScript::getMonoVarName(int index)
{
	return mono_field_get_name(m_vMonoVars[index]);
}

MonoClassField* monoScript::getMonoVar(int index)
{
	return m_vMonoVars[index];
}

monoScript::~monoScript()
{
	m_vMonoVars.clear();
}

MonoObject* monoScript::createNewObject()
{
	MonoObject* newmonoobj = mono_object_new(m_pMonoDomain, m_pMonoObjectClass);
	return newmonoobj;
}

/////////////////////////////////////////////////////////////////////

monoScriptObjectInstance::monoScriptObjectInstance(monoScript* script, MonoObject* monoobj, void* obj)
{
	m_pScriptPtr = script;
	m_pMonoObjectInstance = monoobj;

	if(m_pScriptPtr->isMonoScript())
	{
		void* args[2]={this, &obj};
		mono_runtime_invoke(m_pScriptPtr->getSetHandleMethod(), m_pMonoObjectInstance, args, NULL);

		m_pMethod_update =  mono_class_get_method_from_name(m_pScriptPtr->getMonoClass(), "update", 0);
		m_pMethod_initMonoScript =  mono_class_get_method_from_name(m_pScriptPtr->getMonoClass(), "initMonoScript", 0);
		mono_runtime_invoke(m_pMethod_initMonoScript, m_pMonoObjectInstance, NULL, NULL);
	}
	
}

monoScriptObjectInstance::~monoScriptObjectInstance()
{
}

void monoScriptObjectInstance::update()
{
	if(m_pScriptPtr->isMonoScript())
	{
		mono_runtime_invoke(m_pMethod_update, m_pMonoObjectInstance, NULL, NULL);
	}
}