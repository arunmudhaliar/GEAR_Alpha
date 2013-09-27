#include "GEAREngine.h"
#include "fbxImporter\fbxImporter.h"
#include "core\gxMetaStructures.h"

extern "C" {
static GEAREngine g_cGEAREngine;
static MEngineObserver* g_EngineObserver = NULL;
static MObject3dObserver* g_Object3dObserver = NULL;
vector2i g_cMousePrevPos;

extern DllExport void engine_setEngineObserver(MEngineObserver* observer)
{
	g_EngineObserver=observer;
}

extern DllExport void engine_setObject3dObserver(MObject3dObserver* observer)
{
	g_Object3dObserver=observer;
}

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

void read3dFile2(gxFile& file, object3d* obj)
{
	int nChild=0;
	file.Read(nChild);

	for(int x=0;x<nChild; x++)
	{
		int objID=0;
		file.Read(objID);
		object3d* tempObj=NULL;
		if(objID==0 || objID==1)
		{
			tempObj = new object3d(objID);
		}
		else if(objID==100)
		{
			tempObj = new gxMesh();
		}
		tempObj->setObject3dObserver(g_Object3dObserver);
		tempObj->read(file);
		obj->appendChild(tempObj);
		read3dFile2(file, tempObj);
	}
}

void loadMaterialFromObject3d(gxWorld* world, object3d* obj3d)
{
	if(obj3d->getID()==100)
	{
		gxMesh* mesh = (gxMesh*)obj3d;
		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			gxTriInfo* triInfo = mesh->getTriInfo(x);
			int materialCRC=triInfo->getMaterialCRCFromFileReadInfo();
			char crcFile[1024];
			sprintf(crcFile, "%s/%x", world->getMetaDataFolder(), materialCRC);

			if(materialCRC==0)
				continue;

			gxFile file_meta;
			if(file_meta.OpenFile(crcFile))
			{
				stMetaHeader metaHeader;
				file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

				gxMaterial* material = new gxMaterial();
				material->read(file_meta);

				//check if the material name already exists in our list or not
				std::vector<gxMaterial*>* materialList = world->getMaterialList();
				for(std::vector<gxMaterial*>::iterator it = materialList->begin(); it != materialList->end(); ++it)
				{
					gxMaterial* material_in_list = *it;
					if(material_in_list->getFileCRC()==material->getFileCRC())
					{
						//match found, so assing and delete the new material object
						triInfo->setMaterial(material_in_list);
						GX_DELETE(material);
						break;
					}
				}

				if(triInfo->getMaterial()==NULL)
				{
					//assign the new materiak
					triInfo->setMaterial(material);
					materialList->push_back(material);
				}
				file_meta.CloseFile();
			}
			else
			{
				triInfo->setMaterial(gxMaterial::createNewMaterial());
				world->getMaterialList()->push_back(triInfo->getMaterial());
			}
		}
	}

	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		loadMaterialFromObject3d(world, childobj);
	}
}

void loadAnmationFromObject3d(gxWorld* world, object3d* obj3d, int crc)
{
	gxAnimation* animationController = obj3d->getAnimationController();
	if(animationController)
	{
		std::vector<gxAnimationSet*>* animationSetList=animationController->getAnimationSetList();
		for(std::vector<gxAnimationSet*>::iterator it = animationSetList->begin(); it != animationSetList->end(); ++it)
		{
			gxAnimationSet* animationSet = *it;

			bool bFound=false;
			//check if the animation already on world list
			std::vector<gxAnimationSet*>* world_animationSetList=world->getAnimationSetList();
			for(std::vector<gxAnimationSet*>::iterator it2 = world_animationSetList->begin(); it2 != world_animationSetList->end(); ++it2)
			{
				gxAnimationSet* worldanimationSet = *it2;
				if(worldanimationSet->getCRCOfMeshData()==crc && strcmp(worldanimationSet->getAnimationName(), animationSet->getAnimationName())==0)
				{
					bFound=true;
					break;
				}
			}

			if(!bFound)
			{
				animationSet->setCRCOfMeshData(crc);
				world->appendAnimationSetToWorld(animationSet);
			}
		}
	}

	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		loadAnmationFromObject3d(world, childobj, crc);
	}
}

extern DllExport object3d* engine_loadAndAppendFBX(gxWorld* world, const char* filename)
{
	object3d* root_object_node=NULL;
	if (gxUtil::GX_IS_EXTENSION(filename, ".fbx") || gxUtil::GX_IS_EXTENSION(filename, ".FBX"))
	{
		object3d* obj = NULL;
		char metaInfoFileName[256];
		sprintf(metaInfoFileName, "%s.meta",filename);

		gxFile metaInfoFile;
		if(metaInfoFile.OpenFile(metaInfoFileName))
		{
			int crc=0;
			metaInfoFile.Read(crc);
			metaInfoFile.CloseFile();

			char crcFile[1024];
			sprintf(crcFile, "%s/%x", world->getMetaDataFolder(), crc);

			gxFile file_meta;
			if(file_meta.OpenFile(crcFile))
			{
				stMetaHeader metaHeader;
				file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

				int objID=0;
				file_meta.Read(objID);

				object3d* tempObj=NULL;
				if(objID==0 || objID==1)
				{
					tempObj = new object3d(objID);
					tempObj->setObject3dObserver(g_Object3dObserver);
					tempObj->read(file_meta);
					world->appendChild(tempObj);
					read3dFile2(file_meta, tempObj);
					obj=tempObj;
					loadMaterialFromObject3d(world, obj);
					loadAnmationFromObject3d(world, obj, crc);
					obj->transformationChangedf();
					root_object_node=obj;


					if(g_EngineObserver)
						g_EngineObserver->onAppendToWorld(world, obj);
				}
				file_meta.CloseFile();
			}
		}
	}

	return root_object_node;
}


extern DllExport object3d* engine_loadFBX(gxWorld* world, const char* filename)
{
	fbxImporter importer;
	object3d* root_object_node=importer.loadMyFBX(filename, world->getMaterialList(), world->getAnimationSetList());
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
		GX_DELETE(obj);
		return true;
	}

	return false;
}

extern DllExport object3d* engine_createEmptyObject3d(object3d* parentObj, const char* name)
{
	object3d* emptyObject=new object3d(1);
	emptyObject->setObject3dObserver(g_Object3dObserver);
	emptyObject->setName(name);
	parentObj->appendChild(emptyObject);
	return emptyObject;
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