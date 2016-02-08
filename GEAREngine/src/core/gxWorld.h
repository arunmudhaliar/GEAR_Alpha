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

	void resetWorld(bool bDontCreateDefaultCamera=false);

	void update(float dt);
	void render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);
	void renderShadow(gxRenderer* renderer, int renderFlag);

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera()	{	return m_pActiveCameraPtr;	}
	Camera* createDefaultCameraAndSetActive();

	void renderSingleObject(object3d* obj, object3d* lightPtr, int renderFlag);
	void resizeWorld(float x, float y, float cx, float cy, float nearplane, float farplane);

	gxMaterial* getDefaultMaterial()					{	return &m_cDefaultMaterial;		}
	std::vector<gxMaterial*>* getMaterialList()			{	return &m_cMaterialList;		}
	std::vector<gxAnimationSet*>* getAnimationSetList()	{	return &m_vAnimationSetList;	}

	void createOctree(int minTransformObj, int maxLevel);
	COctree* getOctree()	{	return m_pOctree;	}

	bool appendAnimationSetToWorld(gxAnimationSet* animset);

	void setMetaDataFolder(const char* metaFolder);
	const char* getMetaDataFolder();

	CTextureManager* getTextureManager()	{	return &m_cTextureManager;	}

	gxRenderer* getRenderer()	{	return &m_cRenderer;	}

	void setWorldObserver(MWorldObserver* observer)	{	m_pObserverPtr = observer;	}

	std::vector<gxLight*>* getLightList()	{	return &m_vLightList;		}
	gxLight* getLight(int index)			{	return m_vLightList[index];	}

#ifdef USE_BULLET
	physicsEngine* getPhysicsEngine()		{	return &m_cPhysicsEngine;	}
#endif

	void callback_object3dRemovedFromTree(object3d* child);
	void callback_object3dAppendToTree(object3d* child);
	void callback_object3dDestroyedFromTree(object3d* child);
	void callback_object3dLayerChanged(object3d* child, int oldLayerID);

	object3d* loadAndAppendFBXForDevice(const char* filename);
	object3d* loadFromCRCFile(int crc);
	void populateBonesToMeshNode(object3d* obj, object3d* rootNode);
	void loadAnmationFromObject3d(object3d* obj3d, int crc);
	void loadMaterialFromObject3d(object3d* obj3d);
	void tryLoadTexturesFromObject3d(object3d* obj3d, const char* filepath);

	LayerManager* getLayerManager()	{	return &m_cLayerManager;	}

private:
	void read3dFile(gxFile& file, object3d* obj);
	void renderFromOctreeList(gxRenderer* renderer, ExpandableArray<object3d*>* list, int renderFlag);

	std::vector<gxMaterial*> m_cMaterialList;
	std::vector<gxAnimationSet*> m_vAnimationSetList;
	std::vector<gxLight*> m_vLightList;
	std::vector<Camera*> m_vCameraList;

	gxRenderer m_cRenderer;
	Camera* m_pActiveCameraPtr;	//must not delete this pointer
	Camera m_cDefaultCamera;
	gxCamera m_cDefaultCameraStruct;

	gxMaterial m_cDefaultMaterial;
	CTextureManager m_cTextureManager;
	char m_szMetaDataFolder[512];
	MWorldObserver* m_pObserverPtr;	//must not delete this pointer
#ifdef USE_BULLET
	physicsEngine m_cPhysicsEngine;
#endif
	COctree* m_pOctree;
	LayerManager m_cLayerManager;
};