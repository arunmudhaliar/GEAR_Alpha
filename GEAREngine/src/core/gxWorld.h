#pragma once

#include "object3d.h"
#include "gxMesh.h"
#include "gxLight.h"
#include "Camera.h"
#include "../renderer/gxRenderer.h"
#include "gxMaterial.h"
#include "TextureManager.h"
#ifdef USE_BULLET
#include "physicsEngine.h"
#endif
#include "COctree.h"
#include "LayerManager.h"
#include "../util/gxExpandableArray.h"


class MWorldObserver
{
public:
	virtual void preWorldRender()
	{
	}
	virtual void postWorldRender()
	{
	}
};

class COctree;
class DECLSPEC gxWorld : public object3d
{
public:
	gxWorld();
	~gxWorld();
    static gxWorld* create();
    
	void resetWorld(bool bDontCreateDefaultCamera=false);

	void update(float dt);
	void render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);
	void renderShadow(int renderFlag);

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera()	{	return activeCamera;	}
	Camera* createDefaultCameraAndSetActive();

	void renderSingleObject(object3d* obj, object3d* lightPtr, int renderFlag);
	void resizeWorld(float x, float y, float cx, float cy, float nearplane, float farplane);

	gxMaterial* getDefaultMaterial()					{	return &defaultMaterial;		}
	const std::vector<gxMaterial*>* getMaterialList()			{	return &materialList;		}
	const std::vector<gxAnimationSet*>* getAnimationSetList()	{	return &animationSetList;	}
    bool appendMaterialToWorld(gxMaterial* mat);
    
	void createOctree(int minTransformObj, int maxLevel);
	COctree* getOctree()	{	return octree;	}

	bool appendAnimationSetToWorld(gxAnimationSet* animset);

	void setMetaDataFolder(const char* metaFolder);
	const char* getMetaDataFolder();

	CTextureManager* getTextureManager()	{	return &textureManager;	}

	gxRenderer* getRenderer()	{	return &renderer;	}

	void setWorldObserver(MWorldObserver* observer)	{	worldObserver = observer;	}

	std::vector<gxLight*>* getLightList()	{	return &lightList;		}
	gxLight* getLight(int index)			{	return lightList[index];	}

#ifdef USE_BULLET
	physicsEngine* getPhysicsEngine()		{	return &m_cPhysicsEngine;	}
#endif

	void callback_object3dRemovedFromTree(object3d* child);
	void callback_object3dAppendToTree(object3d* child);
	void callback_object3dDestroyedFromTree(object3d* child);
	void callback_object3dLayerChanged(object3d* child, int oldLayerID);

	object3d* loadAndAppendFBXForDevice(const char* filename);
	object3d* loadFromCRCFile(int crc);
    object3d* loadObjectsFromFile(gxFile& file, int crc);
    
	void populateBonesToMeshNode(object3d* obj3d, object3d* rootNode);
	void loadAnmationFromObject3d(object3d* obj3d, int crc);
	void loadMaterialFromObject3d(object3d* obj3d);
	void tryLoadTexturesFromObject3d(object3d* obj3d, const char* filepath);

	LayerManager* getLayerManager()	{	return &layerManager;	}

private:
	void read3dFile(gxFile& file, object3d* obj);
	void renderFromOctreeList(gxRenderer* renderer, ExpandableArray<object3d*>* list, int renderFlag);

	std::vector<gxMaterial*> materialList;
	std::vector<gxAnimationSet*> animationSetList;
	std::vector<gxLight*> lightList;
	std::vector<Camera*> cameraList;

	gxRenderer renderer;
	Camera* activeCamera;	//must not delete this pointer
	Camera defaultCamera;
	gxCamera defaultCameraStruct;

	gxMaterial defaultMaterial;
	CTextureManager textureManager;
	char metaDataFolder[512];
	MWorldObserver* worldObserver;	//must not delete this pointer
#ifdef USE_BULLET
	physicsEngine m_cPhysicsEngine;
#endif
	COctree* octree;
	LayerManager layerManager;
};