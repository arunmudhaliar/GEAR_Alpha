#ifndef OBJECT3D_H
#define OBJECT3D_H

//#define USE_BXLIST

#include "transform.h"
#ifdef USE_BXLIST
#include "../util/bxLinkedList.h"
#else
#include <vector>
#include <algorithm>
#endif

#include "basicIncludes.h"
#include "aabb.h"
#include "oobb.h"
#include "gxAnimation.h"
#include "../util/gxFile.h"
#include "../renderer/gxRenderer.h"
#if USE_BULLET
#include "../physics/btBulletDynamicsCommon.h"
#endif
#include "quaternion.h"
#include "GEARAsset.h"


//M* Observers
class DECLSPEC MRootObserver
{
public:
	virtual void callback_object3dRemovedFromTree(object3d* child){};
	virtual void callback_object3dAppendToTree(object3d* child){};
	virtual void callback_object3dDestroyedFromTree(object3d* child){};
	virtual void callback_object3dLayerChanged(object3d* child, int oldLayerID){};
};

class DECLSPEC MObject3dObserver
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
	virtual void onConsoleLogFromMono(const char* msg, int msgtype)
	{
	}
};
//

//object3d ids
#define OBJECT3D_OBJECT			0
#define OBJECT3D_MESH			100
#define OBJECT3D_SKINNED_MESH	110
#define OBJECT3D_LIGHT			130
#define OBJECT3D_CAMERA_STRUCT	140
#define OBJECT3D_CAMERA			150
#define OBJECT3D_BONE			160
#define OBJECT3D_DUMMY			170
#define OBJECT3D_WORLD			999

//Default layers
enum EDEFAULT_LAYERS
{
	ELAYER_DEFAULT,
	ELAYER_GUI,
	ELAYER_LAYER1
};
//

class gxAnimation;
class gxAnimationSet;
class DECLSPEC object3d : public transform, public GEARAsset, public MRootObserver
{
private:
	//restricting from calling the default constructor
	object3d(){}
public:

	//These flags are used as attributes and properties of an object3d instance
	enum EOBJEC3DTFLAGS
	{
		eObject3dBaseFlag_None		= 0,
		eObject3dBaseFlag_Visible	= (1<<0),
		eObject3dBaseFlag_Alpha		= (1<<1),	//any type of transparency
		eObject3dBaseFlag_Static	= (1<<2)	//static object
	};

	//These public flags are used to control the render flow of the engine from outside
	enum EOBJECT3DRENDERFLAGS
	{
		eObject3dBase_RenderFlag_None					= 0,
		eObject3dBase_RenderFlag_FirstPass				= (1<<0),
		eObject3dBase_RenderFlag_SecondPass				= (1<<1),
		eObject3dBase_RenderFlag_ThirdPass				= (1<<2),
		eObject3dBase_RenderFlag_FourthPass				= (1<<3),
		eObject3dBase_RenderFlag_Reserved1				= (1<<4),
		eObject3dBase_RenderFlag_Reserved2				= (1<<5),
		eObject3dBase_RenderFlag_Reserved3				= (1<<6),
		eObject3dBase_RenderFlag_Reserved4				= (1<<7),
		eObject3dBase_RenderFlag_Reserved5				= (1<<8),
		eObject3dBase_RenderFlag_DontRenderChilds		= (1<<9),
		eObject3dBase_RenderFlag_NormalRenderPass		= eObject3dBase_RenderFlag_FirstPass
																		| eObject3dBase_RenderFlag_SecondPass
																		| eObject3dBase_RenderFlag_ThirdPass
																		| eObject3dBase_RenderFlag_FourthPass,
		eObject3dBase_RenderFlag_NormalRenderPassWithOutOctreeCulling = eObject3dBase_RenderFlag_NormalRenderPass,
		eObject3dBase_RenderFlag_NormalRenderPassWithOctreeCulling = eObject3dBase_RenderFlag_NormalRenderPass
																		| eObject3dBase_RenderFlag_DontRenderChilds,

	};

	//constructor-destructor
	object3d(int objID);
	virtual ~object3d();

	//message pump
	virtual void update(float dt);
	virtual void render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);	//

	//transform-callback
	virtual void transformationChangedf();

	//basic properties
	int getID()				{	return m_iObjectID;	}
	const char* getName()	{	return m_cszName;	}
	void setName(const char* name)	{	GX_STRCPY(m_cszName, name);	}

	//child: child-parent
	int getChildCount()				{	return m_cChilds.size();	}
	object3d* getChild(int index)	{	return m_cChilds[index];	}
	object3d* find(const char* name);
	void setParent(object3d* pParentPtr)	{	m_pParentPtr=pParentPtr;	}
	object3d* getParent()					{	return m_pParentPtr;		}

	//child: add-remove
	object3d* appendChild(object3d* child);
	bool removeChild(object3d* child);

	//child: child callbacks
	virtual void onAppendObject3dChild(object3d* child);
	virtual void onRemoveObject3dChild(object3d* child);
#ifdef USE_BXLIST
	bxLinkedList<object3d*>* getChildList()	{	return &m_cChilds;	}
#else
	std::vector<object3d*>* getChildList()	{	return &m_cChilds;	}
#endif
	//

	//bit flags
	void	resetAllBaseFlags(bool recursive=false);
	void	setBaseFlag(EOBJEC3DTFLAGS eFlags, bool recursive=false);
	void	reSetBaseFlag(EOBJEC3DTFLAGS eFlags, bool recursive=false);
	bool	isBaseFlag(EOBJEC3DTFLAGS eFlags)		{	return (m_eBaseFlags&eFlags)?true:false;	};
	unsigned int getBaseFlag()							{	return m_eBaseFlags;				}

	//bounds
	gxAABBf& getAABB()	{	return m_cAABB;	}
	gxOOBBf& getOOBB()	{	return m_cOOBB;	}
	virtual void calculateAABB();

	//animation
	gxAnimation* createAnimationController();
	void resetAnimationControllerAndAssignItToObject(object3d* obj);	//only used for FBX import: assigns the animation controller to new object and clears its pointer
	void setAnimationController(gxAnimation* controller);
	gxAnimation* getAnimationController()				{	return m_pAnimationController;	}
	gxAnimationSet* applyAnimationSetRecursive(int index);
	gxAnimationSet* applyAnimationSetRecursive(gxAnimationSet* animset);
	void setAnimationTrack(gxAnimationTrack* track);
	gxAnimationTrack* getAnimationTrack()	{	return m_pAnimationTrack;	}
	void updateAnimationFrameToObject3d(int frame);

	//serialize
	virtual void write(gxFile& file);
	virtual void read(gxFile& file);
	void writeAnimationController(gxFile& file);
	void readAnimationController(gxFile& file);

	//user defined data
	void setEditorUserData(void* userData)	{	m_pEditorUserDataPtr=userData;	}
	void* getEditorUserData()				{	return m_pEditorUserDataPtr;	}

	//observers
	void setObject3dObserver(MObject3dObserver* observer)			{	m_pObject3dObserver = observer;	}
	void setObject3dObserverRecursive(MObject3dObserver* observer);
	void setEngineObserver(MEngineObserver* observer)				{	m_pEngineObserver = observer;	}
	void setRootObserverOfTree(MRootObserver* rootObserver)			{	m_pRootObserver=rootObserver;	}
	MRootObserver* getRootObserverOfThisTree()						{	return m_pRootObserver;			}

	//physics
#if USE_BULLET
	void setRigidBody(btRigidBody* rb)			{	m_pPhysics_RigidBodyPtr = rb;	}
	btRigidBody* getRigidBody()					{	return m_pPhysics_RigidBodyPtr;	}
#endif

	//flags
	void setVisited(bool flag)	{	m_bVisited=flag;	}
	bool isVisited()			{	return m_bVisited;	}

	//layer
	void setLayer(int layer, bool bRecursive);
	int getLayer()				{	return m_iLayer;	}

protected:
	void clearAnimTrackOnAllNodes();

	char m_cszName[256];
	int m_iObjectID;
	object3d* m_pParentPtr;
	unsigned int m_eBaseFlags;
#ifdef USE_BXLIST
	bxLinkedList<object3d*> m_cChilds;
#else
	std::vector<object3d*> m_cChilds;
#endif
	gxAABBf m_cAABB;
	gxOOBBf m_cOOBB;
	gxAnimation* m_pAnimationController;
	gxAnimationTrack* m_pAnimationTrack;	//must not delete this pointer
	void* m_pEditorUserDataPtr;				//must not delete this pointer
	MObject3dObserver* m_pObject3dObserver;	//must not delete this pointer
	MRootObserver* m_pRootObserver;			//must not delete this pointer
	MEngineObserver* m_pEngineObserver;		//must not delete this pointer
#if USE_BULLET
	btRigidBody* m_pPhysics_RigidBodyPtr;	//must not delete this pointer
#endif
	bool m_bVisited;
	int m_iLayer;
};

////////////////////OBJECT3D C# BRIDGE////////////////////
extern "C" {
	DECLSPEC const char* object3d_getName(object3d* obj);
	DECLSPEC int object3d_getID(object3d* obj);
	DECLSPEC object3d* object3d_find(object3d* obj, const char* name);
	DECLSPEC int object3d_getChildCount(object3d* obj);
	DECLSPEC object3d* object3d_getChild(object3d* obj, int index);

	DECLSPEC gxAnimation* object3d_createAnimationController(object3d* obj);
	DECLSPEC gxAnimation* object3d_getAnimationController(object3d* obj);
	DECLSPEC gxAnimationSet* object3d_applyAnimationSetRecursive(object3d* obj, int index);

	DECLSPEC object3d* object3d_appendChild(object3d* obj, object3d* child);
	DECLSPEC bool object3d_removeChild(object3d* obj, object3d* child);
	DECLSPEC object3d* object3d_getParent(object3d* obj);
}
//

#endif
