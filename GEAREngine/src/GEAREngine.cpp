#include "GEAREngine.h"
#ifndef GEAR_ANDROID
#include "fbxImporter/fbxImporter.h"
#endif
#include "core/object3d.h"
#include "core/gxMesh.h"
#include "core/gxSkinnedMesh.h"

#include "core/gxMetaStructures.h"
#include "mono/src/monoWrapper.h"

extern "C" {
static GEAREngine g_cGEAREngine;
static MEngineObserver* g_EngineObserver = NULL;
static MObject3dObserver* g_Object3dObserver = NULL;
vector2i g_cMousePrevPos;

extern DECLSPEC void engine_setEngineObserver(MEngineObserver* observer)
{
	g_EngineObserver=observer;
}

extern DECLSPEC void engine_setObject3dObserver(MObject3dObserver* observer)
{
	g_Object3dObserver=observer;
}

extern DECLSPEC int engine_test_function_for_mono()
{
	return 100;
}

extern DECLSPEC void engine_init(int nWorldToCreate)
{
    g_isEngineRunning = true;
	g_cGEAREngine.initEngine(nWorldToCreate);
	g_cMousePrevPos.zero();
}

extern DECLSPEC void engine_resize(gxWorld* world, float x, float y, float cx, float cy, float nearplane, float farplane)
{
	world->resizeWorld(x, y, cx, cy, nearplane, farplane);
}

extern DECLSPEC void engine_destroy()
{
	g_cGEAREngine.resetEngine();
    AutoReleasePool::getInstance().clearPool();
    g_isEngineRunning=false;
}

extern DECLSPEC void engine_setMetaFolder(gxWorld* world, const char* metaFolder)
{
	world->setMetaDataFolder(metaFolder);
}

extern DECLSPEC gxWorld* engine_getWorld(int index)
{
	return g_cGEAREngine.getWorld(index);
}
extern DECLSPEC HWShaderManager* engine_getHWShaderManager()
{
	return g_cGEAREngine.getHWShaderManager();
}

extern DECLSPEC void engine_update(gxWorld* world, float dt)
{
	world->update(dt);
}

extern DECLSPEC void engine_render(gxWorld* world, monoScriptObjectInstance* lightScriptInstance, int renderFlag)
{
	world->render(world->getRenderer(), lightScriptInstance, renderFlag);
}

void engine_consoleLog(const char* msg, int msgtype)
{
#if defined(GEAR_EDITOR)
	if(g_EngineObserver)
		g_EngineObserver->onConsoleLogFromMono(msg, msgtype);
#else
	DEBUG_PRINT(msg);
#endif
}

extern DECLSPEC object3d* engine_loadAndAppendMesh(gxWorld* world, const char* filename)
{
	return world->loadAndAppendFBXForDevice(filename);
}

extern DECLSPEC void engine_mouseLButtonDown(gxWorld* world, int x, int y, int flag)
{
}

extern DECLSPEC void engine_mouseLButtonUp(gxWorld* world, int x, int y, int flag)
{
}

extern DECLSPEC void engine_mouseRButtonDown(gxWorld* world, int x, int y, int flag)
{
}

extern DECLSPEC void engine_mouseRButtonUp(gxWorld* world, int x, int y, int flag)
{
}

extern DECLSPEC void engine_mouseWheel(gxWorld* world, int zDelta, int x, int y, int flag)
{
	Camera* camera=world->getActiveCamera();
	int dir = (zDelta<0)?1:-1;
	vector3f aCamForwardDir(camera->getZAxis());
	float d=camera->getPosition().length();
	float factor=20.0f;
	//if(nFlags&MK_SHIFT)
	//	factor=500.0f;
	aCamForwardDir.x=aCamForwardDir.x*(d/factor)*(dir);
	aCamForwardDir.y=aCamForwardDir.y*(d/factor)*(dir);
	aCamForwardDir.z=aCamForwardDir.z*(d/factor)*(dir);
	camera->updatePositionf(aCamForwardDir.x, aCamForwardDir.y, aCamForwardDir.z);
}

extern DECLSPEC void engine_mouseMove(gxWorld* world, int x, int y, int flag)
{
	Camera* camera=world->getActiveCamera();

	int xx=x;
	int yy=y;
	int xPos = xx;
	int yPos = yy;
	int Pos_dx	= abs(xPos-g_cMousePrevPos.x);
	int Pos_dy	= abs(yPos-g_cMousePrevPos.y);

	int aDirX=-1;
	int aDirY=1;
	if(g_cMousePrevPos.x>xPos)		aDirX=1;
	if(g_cMousePrevPos.y>yPos)		aDirY=-1;

#ifdef GEAR_WINDOWS
	if(flag&MK_MBUTTON /*&& !(flag&MK_CONTROL)*/)
	{
		float d=camera->getPosition().length();
		if(flag&MK_SHIFT)
			camera->updateLocalPositionf((d/5000.0f)*Pos_dx*aDirX, (d/5000.0f)*Pos_dy*aDirY, 0);
		else
			camera->updateLocalPositionf((d/500.0f)*Pos_dx*aDirX, (d/500.0f)*Pos_dy*aDirY, 0);
	}
	else if(flag&MK_RBUTTON /*&& flag&MK_CONTROL*/)
#endif
	{

		vector3f aUP(0, 0, 1);
		//aUP=camera->getYAxis();
		vector3f aVect(0, 0, 0);
		//aVect=m_cPickObjectCenter;	//can modify this later to rotate around mesh center

		camera->rotateArb(0.5f*Pos_dx*aDirX, &aUP.x, aVect);
		vector3f left=camera->getXAxis();
		camera->rotateArb(0.5f*Pos_dy*-aDirY, &left.x, aVect);
	}

	//DEBUG_PRINT("%f, %f\n", delta.x, delta.y);
	g_cMousePrevPos.x=xx;
	g_cMousePrevPos.y=yy;

}

extern bool engine_removeObject3d(gxWorld* world, object3d* obj)
{
	if(world->removeChild(obj))
	{
		if(g_EngineObserver)
			g_EngineObserver->onRemoveFromWorld(world, obj);
		return true;
	}

	return false;
}

extern bool engine_destroyObject3d(gxWorld* world, object3d* obj)
{
	if(world->removeChild(obj))
	{
		if(g_EngineObserver)
			g_EngineObserver->onRemoveFromWorld(world, obj);
		//GX_DELETE(obj);
		return true;
	}

	return false;
}

extern DECLSPEC object3d* engine_createEmptyObject3d(object3d* parentObj, const char* name)
{
    object3d* emptyObject = object3d::create(OBJECT3D_DUMMY);
	emptyObject->setObject3dObserver(g_Object3dObserver);
	emptyObject->setName(name);
	parentObj->appendChild(emptyObject);
    REF_RELEASE(emptyObject);
	return emptyObject;
}

extern DECLSPEC monoScriptObjectInstance* engine_createLight(object3d* parentObj, const char* name, gxLight::ELIGHT_TYPE eType)
{
#if REFACTOR_MONO_SCRIPT
    gxLight* light = gxLight::create();
	light->setObject3dObserver(g_Object3dObserver);
	light->setName(name);
	light->setLightType(eType);
	parentObj->appendChild(light);
    REF_RELEASE(light);
	return light;
#else
    auto obj3d = object3d::create(OBJECT3D_LIGHT);
    obj3d->setObject3dObserver(g_Object3dObserver);
    obj3d->setName(name);
    
    auto lightScriptInstance = gxLight::create(monoWrapper::mono_getMonoScriptClass("gxLight.cs"), obj3d);
    obj3d->attachMonoScrip(lightScriptInstance);
    REF_RELEASE(lightScriptInstance);
    lightScriptInstance->setLightType(eType);
    parentObj->appendChild(obj3d);
    REF_RELEASE(obj3d);
    return lightScriptInstance;
#endif
    return nullptr;
}

extern DECLSPEC object3d* engine_createCamera(object3d* parentObj, const char* name, gxRenderer* renderer)
{
    Camera* camera = Camera::create();
	camera->initCamera(renderer);
	camera->setObject3dObserver(g_Object3dObserver);
	camera->setName(name);
	parentObj->appendChild(camera);
    REF_RELEASE(camera);
	return camera;
}
    
extern DECLSPEC bool engine_isRunning()
{
    return g_isEngineRunning;
}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////GEAR ENGINE////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GEAREngine::GEAREngine()
{
}

GEAREngine::~GEAREngine()
{
	resetEngine();
}

void GEAREngine::resetEngine()
{
	for(std::vector<gxWorld*>::iterator it = worldList.begin(); it != worldList.end(); ++it)
	{
		gxWorld* obj = *it;
		REF_RELEASE(obj);
	}
	worldList.clear();
}

void GEAREngine::initEngine(int nWorldToCreate)
{
	for(int x=0;x<nWorldToCreate;x++)
	{
        gxWorld* newWorld = gxWorld::create();
		//newWorld->setLayer(0, true);	//no need to put world inside layer, by default it is considered to be in default layer
		worldList.push_back(newWorld);
	}
}
