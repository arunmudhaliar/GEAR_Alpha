#include "gePropertyAddComponent.h"
#include "../../AssetImporter.h"
#include "../../EditorApp.h"

void gePropertyAddComponent::onButtonClicked(geGUIBase* btn)
{
	//if(btn==addComponentToolBarDropMenuButton)
	//{
	//	if(addComponentToolBarDropMenuButton->isButtonPressed())
	//	{
	//		object3d* obj=object3dPtr;
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
	geToolBarDropMenu::stDropMenuItem* mitem = addComponentToolBarDropMenuButton->appendMenuItem("Physics", base++);
	addComponentToolBarDropMenuButton->appendMenuItem("RigidBody", base++, mitem);
	addComponentToolBarDropMenuButton->appendMenuItem("Box Collider", base++, mitem);
	addComponentToolBarDropMenuButton->appendMenuItem("Sphere Collider", base++, mitem);
	addComponentToolBarDropMenuButton->appendMenuItem("Capsule Collider", base++, mitem);
	addComponentToolBarDropMenuButton->appendMenuItem("Mesh Collider", base++, mitem);
	*/

	if(cmd==0x00005200)
	{
	}
	else if(cmd==0x00005201)
	{
#if USE_BULLET
		if(object3dPtr)
		{
			monoWrapper::mono_engine_getWorld(0)->getPhysicsEngine()->addRigidBody(object3dPtr);
		}
#endif
	}
	else if(cmd==0x00005202)
	{
#if USE_BULLET
		if(object3dPtr)
		{
			monoWrapper::mono_engine_getWorld(0)->getPhysicsEngine()->addBoxCollider(object3dPtr);
		}
#endif
	}
}