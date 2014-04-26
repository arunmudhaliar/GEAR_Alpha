#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "transform.h"
#include <vector>
#include <algorithm>
#include "basicIncludes.h"
#include "aabb.h"
#include "oobb.h"
#include "gxAnimation.h"
#include "../util/gxFile.h"
#include "../renderer/gxRenderer.h"
#if USE_BULLET
#include "../physics/btBulletDynamicsCommon.h"
#endif

class DllExport MRootObserver
{
public:
	virtual void callback_object3dRemovedFromTree(object3d* child){};
	virtual void callback_object3dAppendToTree(object3d* child){};
	virtual void callback_object3dDestroyedFromTree(object3d* child){};
};

class DllExport MObject3dObserver
{
public:
	virtual void onObject3dChildAppend(object3d* child)=0;
	virtual void onObject3dChildRemove(object3d* child)=0;
	virtual void onObject3dDestroy(object3d* obj)=0;
};

class MEngineObserver
{
public:
	virtual void onAppendToWorld(object3d* world, object3d* obj)
	{
	}
	virtual void onRemoveFromWorld(object3d* world, object3d* obj)
	{
	}
};

#define OBJECT3D_OBJECT			0
#define OBJECT3D_MESH			100
#define OBJECT3D_SKINNED_MESH	101
#define OBJECT3D_LIGHT			3
#define OBJECT3D_CAMERA			102
#define OBJECT3D_WORLD			999

class gxAnimation;
class gxAnimationSet;
class DllExport object3d : public transform, public MRootObserver
{
public:

	enum EOBJEC3DTFLAGS
	{
		eObject3dBaseFlag_None		= 0,
		eObject3dBaseFlag_Visible	= (1<<0)
	};

	object3d(int objID);
	virtual ~object3d();

	virtual void update(float dt);

	void updateAnimationFrameToObject3d(int frame);
	virtual void render(gxRenderer* renderer, object3d* light);


	virtual void transformationChangedf();

	virtual void calculateAABB();
	void calculateInitialAABB();

	int getID()				{	return m_iObjectID;	}
	const char* getName()	{	return m_cszName;	}
	void setName(const char* name)	{	GX_STRCPY(m_cszName, name);	}

	object3d* appendChild(object3d* child);
	bool removeChild(object3d* child);
	std::vector<object3d*>* getChildList()	{	return &m_cChilds;	}

	int getChildCount()				{	return m_cChilds.size();	}
	object3d* getChild(int index)	{	return m_cChilds[index];	}

	void setParent(object3d* pParentPtr)	{	m_pParentPtr=pParentPtr;	}
	object3d* getParent()					{	return m_pParentPtr;		}

	void	resetAllBaseFlags()						{	m_eBaseFlags=0;						}
	void	setBaseFlag(EOBJEC3DTFLAGS eFlags)		{	m_eBaseFlags=m_eBaseFlags|eFlags;	};
	void	reSetBaseFlag(EOBJEC3DTFLAGS eFlags)	{	m_eBaseFlags=m_eBaseFlags&~eFlags;	};
	bool	isBaseFlag(EOBJEC3DTFLAGS eFlags)		{	return (m_eBaseFlags&eFlags)?true:false;	};
	int		getBaseFlag()							{	return m_eBaseFlags;				}

	gxAABBf& getAABB()	{	return m_cAABB;	}
	gxOOBBf& getOOBB()	{	return m_cOOBB;	}

	object3d* find(const char* name);

	//animation
	gxAnimation* createAnimationController();
	gxAnimation* getAnimationController()				{	return m_pAnimationController;	}
	gxAnimationSet* applyAnimationSetRecursive(int index);
	gxAnimationSet* applyAnimationSetRecursive(gxAnimationSet* animset);
	void setAnimationTrack(gxAnimationTrack* track);
	gxAnimationTrack* getAnimationTrack()	{	return m_pAnimationTrack;	}

	void setFileCRC(int crc)	{	m_iFileCRC = crc;	}
	int getFileCRC()			{	return m_iFileCRC;	}

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);

	void writeAnimationController(gxFile& file);
	void readAnimationController(gxFile& file);

	void setEditorUserData(void* userData)	{	m_pEditorUserDataPtr=userData;	}
	void* getEditorUserData()				{	return m_pEditorUserDataPtr;	}

	void setObject3dObserver(MObject3dObserver* observer)	{	m_pObject3dObserver = observer;	}
	void setObject3dObserverRecursive(MObject3dObserver* observer);

	void setEngineObserver(MEngineObserver* observer)	{	m_pEngineObserver = observer;	}

	void setRootObserverOfTree(MRootObserver* rootObserver)	{	m_pRootObserver=rootObserver;	}
	MRootObserver* getRootObserverOfThisTree()				{	return m_pRootObserver;			}

#if USE_BULLET
	void setRigidBody(btRigidBody* rb)			{	m_pPhysics_RigidBodyPtr = rb;	}
	btRigidBody* getRigidBody()					{	return m_pPhysics_RigidBodyPtr;	}
#endif

protected:
	void clearAnimTrackOnAllNodes();

	char m_cszName[64];
	int m_iObjectID;
	object3d* m_pParentPtr;
	unsigned int m_eBaseFlags;
	std::vector<object3d*> m_cChilds;
	gxAABBf m_cAABB;
	gxOOBBf m_cOOBB;
	gxAnimation* m_pAnimationController;
	gxAnimationTrack* m_pAnimationTrack;	//must not delete this pointer
	int m_iFileCRC;
	void* m_pEditorUserDataPtr;				//must not delete this pointer
	MObject3dObserver* m_pObject3dObserver;	//must not delete this pointer
	MRootObserver* m_pRootObserver;			//must not delete this pointer
	MEngineObserver* m_pEngineObserver;		//must not delete this pointer
#if USE_BULLET
	btRigidBody* m_pPhysics_RigidBodyPtr;	//must not delete this pointer
#endif
};

extern "C" {
	DllExport const char* object3d_getName(object3d* obj);
	DllExport int object3d_getID(object3d* obj);
	DllExport object3d* object3d_find(object3d* obj, const char* name);
	DllExport int object3d_getChildCount(object3d* obj);
	DllExport object3d* object3d_getChild(object3d* obj, int index);

	DllExport gxAnimation* object3d_createAnimationController(object3d* obj);
	DllExport gxAnimation* object3d_getAnimationController(object3d* obj);
	DllExport gxAnimationSet* object3d_applyAnimationSetRecursive(object3d* obj, int index);

	DllExport object3d* object3d_appendChild(object3d* obj, object3d* child);
	DllExport bool object3d_removeChild(object3d* obj, object3d* child);
	DllExport object3d* object3d_getParent(object3d* obj);
}

#endif
