#include "gePropertyAddComponent.h"
#include "../../AssetImporter.h"
#include "../../EditorApp.h"

void gePropertyAddComponent::onButtonClicked(geGUIBase* btn)
{
	//if(btn==m_pAddComponentToolBarDropMenuBtnPtr)
	//{
	//	if(m_pAddComponentToolBarDropMenuBtnPtr->isButtonPressed())
	//	{
	//		object3d* obj=m_pObject3dPtr;
	//		if(obj)
	//		{
	//			monoWrapper::mono_engine_getWorld(0)->getPhysicsEngine()->addRigidBody(obj);
	//		}
	//	}
	//}
}

void gePropertyAddComponent::onCommand(int cmd)
{
	/*
	int base=0x00005200;
	geToolBarDropMenu::stDropMenuItem* mitem = m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Physics", base++);
	m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("RigidBody", base++, mitem);
	m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Box Collider", base++, mitem);
	m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Sphere Collider", base++, mitem);
	m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Capsule Collider", base++, mitem);
	m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Mesh Collider", base++, mitem);
	*/

	if(cmd==0x00005200)
	{
	}
	else if(cmd==0x00005201)
	{
#if USE_BULLET
		if(m_pObject3dPtr)
		{
			monoWrapper::mono_engine_getWorld(0)->getPhysicsEngine()->addRigidBody(m_pObject3dPtr);
		}
#endif
	}
	else if(cmd==0x00005202)
	{
#if USE_BULLET
		if(m_pObject3dPtr)
		{
			monoWrapper::mono_engine_getWorld(0)->getPhysicsEngine()->addBoxCollider(m_pObject3dPtr);
		}
#endif
	}
}