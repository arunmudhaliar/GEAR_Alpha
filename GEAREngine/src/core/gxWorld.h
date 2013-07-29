#ifndef GXWORLD_H
#define GXWORLD_H

#include "object3d.h"
#include "gxMesh.h"
#include "Camera.h"
#include "../renderer/gxRenderer.h"
#include "gxMaterial.h"
#include "TextureManager.h"

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

class gxWorld : public object3d
{
public:
	gxWorld();
	~gxWorld();

	void update(float dt);
	void render();
	//object3d* loadFBX(const char* filename);

	Camera* getActiveCamera()	{	return m_pActiveCameraPtr;	}
	Camera* setDefaultCameraActive();

	void renderSingleObject(object3d* obj);
	void resizeWorld(float x, float y, float cx, float cy);

	gxMaterial* getDefaultMaterial()					{	return &m_cDefaultMaterial;		}
	std::vector<gxMaterial*>* getMaterialList()			{	return &m_cMaterialList;		}
	std::vector<gxAnimationSet*>* getAnimationSetList()	{	return &m_vAnimationSetList;	}

	void setMetaDataFolder(const char* metaFolder)
	{
		GX_STRCPY(m_szMetaDataFolder, metaFolder);
		m_cTextureManager.setMetaDataFolder(m_szMetaDataFolder);
	}

	const char* getMetaDataFolder()
	{
		return m_szMetaDataFolder;
	}

	void calculateAABB();

	CTextureManager* getTextureManager()	{	return &m_cTextureManager;	}

	//object3d* appendObject3dToRoot(object3d* obj);

	//bool removeObject3dFromWorld(object3d* obj);
	void loadTextures(object3d* obj, const char* fbxFileName);

	gxRenderer* getRenderer()	{	return &m_cRenderer;	}

	void setWorldObserver(MWorldObserver* observer)	{	m_pObserverPtr = observer;	}

private:


	//std::vector<object3d*> m_cRootNodes;
	std::vector<gxMaterial*> m_cMaterialList;
	std::vector<gxAnimationSet*> m_vAnimationSetList;

	gxRenderer m_cRenderer;
	Camera* m_pActiveCameraPtr;	//must not delete this pointer
	Camera m_cDefaultCamera;

	gxMaterial m_cDefaultMaterial;
	CTextureManager m_cTextureManager;
	char m_szMetaDataFolder[512];
	MWorldObserver* m_pObserverPtr;	//must not delete this pointer
};

#endif