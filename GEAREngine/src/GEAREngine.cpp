#include "GEAREngine.h"
#include "fbxImporter\fbxImporter.h"

extern "C" {
static GEAREngine g_cGEAREngine;
vector2i g_cMousePrevPos;

extern DllExport void engine_test_function_for_mono()
{
}

extern DllExport void engine_init(int nWorldToCreate)
{
	g_cGEAREngine.initEngine(nWorldToCreate);
	g_cMousePrevPos.zero();
}

extern DllExport gxWorld* engine_getWorld(int index)
{
	return g_cGEAREngine.getWorld(index);
}

extern DllExport void engine_update(gxWorld* world, float dt)
{
	world->update(dt);
}

extern DllExport void engine_resize(gxWorld* world, float x, float y, float cx, float cy)
{
	world->resizeWorld(x, y, cx, cy);
}

extern DllExport void engine_render(gxWorld* world)
{
	world->render();
}

void engine_renderSingleObject(gxWorld* world, object3d* obj)
{
	world->renderSingleObject(obj);
}

extern DllExport object3d* engine_loadAndAppendFBX(gxWorld* world, const char* filename)
{
	fbxImporter importer;
	object3d* root_object_node=importer.loadMyFBX(filename, world->getMaterialList());
	world->appendChild(root_object_node);
	world->loadTextures(root_object_node, filename);
	return root_object_node;
}

extern DllExport object3d* engine_loadFBX(gxWorld* world, const char* filename)
{
	fbxImporter importer;
	object3d* root_object_node=importer.loadMyFBX(filename, world->getMaterialList());
	return root_object_node;
}

extern DllExport object3d* engine_appendObject3dToRoot(gxWorld* world, object3d* obj)
{
	return world->appendChild(obj);
}

extern DllExport void engine_mouseLButtonDown(gxWorld* world, int x, int y, int flag)
{
}

extern DllExport void engine_mouseLButtonUp(gxWorld* world, int x, int y, int flag)
{
}

extern DllExport void engine_mouseRButtonDown(gxWorld* world, int x, int y, int flag)
{
}

extern DllExport void engine_mouseRButtonUp(gxWorld* world, int x, int y, int flag)
{
}

extern DllExport void engine_mouseWheel(gxWorld* world, int zDelta, int x, int y, int flag)
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

extern DllExport void engine_mouseMove(gxWorld* world, int x, int y, int flag)
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


	if(flag&MK_MBUTTON /*&& !(flag&MK_CONTROL)*/)
	{
		float d=camera->getPosition().length();
		if(flag&MK_SHIFT)
			camera->updateLocalPositionf((d/5000.0f)*Pos_dx*aDirX, (d/5000.0f)*Pos_dy*aDirY, 0);
		else
			camera->updateLocalPositionf((d/500.0f)*Pos_dx*aDirX, (d/500.0f)*Pos_dy*aDirY, 0);
	}
	else if(flag&MK_RBUTTON /*&& flag&MK_CONTROL*/)
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

extern DllExport void engine_setMetaFolder(gxWorld* world, const char* metaFolder)
{
	world->setMetaDataFolder(metaFolder);
}

extern gxTexture* engine_loadTextureFromFile(gxWorld* world, gxMaterial* material, const char* filename)
{
	return material->loadTextureFromFile(*world->getTextureManager(), filename);
}

extern bool engine_removeObject3d(gxWorld* world, object3d* obj)
{
	return world->removeChild(obj);
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
	for(std::vector<gxWorld*>::iterator it = m_cWorlds.begin(); it != m_cWorlds.end(); ++it)
	{
		gxWorld* obj = *it;
		GX_DELETE(obj);
	}
	m_cWorlds.clear();
}

void GEAREngine::initEngine(int nWorldToCreate)
{
	for(int x=0;x<nWorldToCreate;x++)
	{
		gxWorld* newWorld = new gxWorld();
		m_cWorlds.push_back(newWorld);
	}
}

//void GEAREngine::updateEngine(gxWorld* world, float dt)
//{
//	world->update(dt);
//}
//
//void GEAREngine::renderEngine(gxWorld* world)
//{
//	world->render();
//}
//
//void GEAREngine::renderSingleObject(gxWorld* world, object3d* obj)
//{
//	world->renderSingleObject(obj);
//}
//
//void GEAREngine::resizeEngine(gxWorld* world, float x, float y, float cx, float cy)
//{
//	world->resizeWorld(x, y, cx, cy);
//}

//object3d* GEAREngine::loadFBX(const char* filename)
//{
//	fbxImporter importer;
//	object3d* root_object_node=importer.loadMyFBX(filename);
//	m_cRootNodes.push_back(root_object_node);
//
//	return root_object_node;
//}
//
//Camera* GEAREngine::setDefaultCameraActive()
//{
//	m_pActiveCameraPtr=&m_cDefaultCamera;
//
//	return m_pActiveCameraPtr;
//}