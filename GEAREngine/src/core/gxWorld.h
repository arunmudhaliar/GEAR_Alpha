#ifndef GXWORLD_H
#define GXWORLD_H

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

class DllExport gxWorld : public object3d
{
public:
	gxWorld();
	~gxWorld();

	void resetWorld();

	void update(float dt);
	void render(gxRenderer* renderer, object3d* light);

	Camera* getActiveCamera()	{	return m_pActiveCameraPtr;	}
	Camera* setDefaultCameraActive();

	void renderSingleObject(object3d* obj, object3d* light);
	void resizeWorld(float x, float y, float cx, float cy, float nearplane, float farplane);

	gxMaterial* getDefaultMaterial()					{	return &m_cDefaultMaterial;		}
	std::vector<gxMaterial*>* getMaterialList()			{	return &m_cMaterialList;		}
	std::vector<gxAnimationSet*>* getAnimationSetList()	{	return &m_vAnimationSetList;	}

	bool appendAnimationSetToWorld(gxAnimationSet* animset)
	{
		if(std::find(m_vAnimationSetList.begin(), m_vAnimationSetList.end(), animset)==m_vAnimationSetList.end())
		{
			m_vAnimationSetList.push_back(animset);
			return true;
		}

		return false;
	}

	void setMetaDataFolder(const char* metaFolder)
	{
		GX_STRCPY(m_szMetaDataFolder, metaFolder);
		m_cTextureManager.setMetaDataFolder(m_szMetaDataFolder);
	}

	const char* getMetaDataFolder()
	{
		return m_szMetaDataFolder;
	}

	CTextureManager* getTextureManager()	{	return &m_cTextureManager;	}
#if 0
	void loadTextures(object3d* obj, const char* fbxFileName);
#endif

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

	object3d* loadAndAppendFBX(const char* filename);
	object3d* loadAndAppendFBXForDevice(const char* filename);
	void populateBonesToMeshNode(object3d* obj, object3d* rootNode);
	void loadAnmationFromObject3d(object3d* obj3d, int crc);
	void loadMaterialFromObject3d(object3d* obj3d);
	void read3dFile2(gxFile& file, object3d* obj);

private:
	std::vector<gxMaterial*> m_cMaterialList;
	std::vector<gxAnimationSet*> m_vAnimationSetList;
	std::vector<gxLight*> m_vLightList;

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
};

#endif