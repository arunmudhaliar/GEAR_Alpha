#pragma once

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
#include "../mono/src/monoClassDef.h"
#include "../autoReleasePool/autoReleasePool.h"
#include <type_traits>

using namespace GEAR::Memory;

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
class monoClassDef;
class monoScriptObjectInstance;

class gxAnimationSet;

class DECLSPEC object3d : public transform, public GEARAsset, public MRootObserver, public Ref
{
private:
	//restricting from calling the default constructor
    object3d():Ref()
    {}
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

    static object3d* create(int objID);
    static object3d* create();
    
    //clone
    virtual object3d* clone();
    
	//message pump
	virtual void update(float dt);
	virtual void render(gxRenderer* renderer, monoScriptObjectInstance* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);	//
    void startMono();
	void updateMono();
    void stopMono();
    
	//transform-callback
	virtual void transformationChangedf();

	//basic properties
	int getID()				{	return objectID;	}
    const std::string& getName()	{	return name;	}
    void setName(const std::string& name)	{	this->name=name;	}

	//child: child-parent
	int getChildCount()				{	return (int)childList.size();	}
	object3d* getChild(int index)	{	return childList[index];	}
    object3d* find(const std::string& name);
	void setParent(object3d* pParentPtr)	{	parent=pParentPtr;	}
	object3d* getParent()					{	return parent;		}

	//child: add-remove
	object3d* appendChild(object3d* child);
	bool removeChild(object3d* child);

#ifdef USE_BXLIST
	bxLinkedList<object3d*>* getChildList()         {	return &childList;	}
#else
	const std::vector<object3d*>* getChildList()	{	return &childList;	}
#endif
	//

	//bit flags
	void	resetAllBaseFlags(bool recursive=false);
	void	setBaseFlag(EOBJEC3DTFLAGS eFlags, bool recursive=false);
	void	reSetBaseFlag(EOBJEC3DTFLAGS eFlags, bool recursive=false);
	bool	isBaseFlag(EOBJEC3DTFLAGS eFlags)		{	return (baseFlag&eFlags)?true:false;	};
	unsigned int getBaseFlag()							{	return baseFlag;				}

	//bounds
	gxAABBf& getAABB()	{	return aabb;	}
	gxOOBBf& getOOBB()	{	return oobb;	}
	virtual void calculateAABB();

    gxAnimation* getAnimationController();
	gxAnimationSet* applyAnimationSetRecursive(int index);
	gxAnimationSet* applyAnimationSetRecursive(gxAnimationSet* animset);
	IAnimationTrack* getAnimationTrack()	{	return animationTrack;	}

	//serialize
	void write(gxFile& file);
	void read(gxFile& file);

	//user defined data
	void setEditorUserData(void* userData)	{	editorUserData=userData;	}
	void* getEditorUserData()				{	return editorUserData;	}

	//observers
	void setObject3dObserver(MObject3dObserver* observer)			{	object3DObserver = observer;        }
	void setObject3dObserverRecursive(MObject3dObserver* observer);
	void setEngineObserver(MEngineObserver* observer)				{	engineObserver = observer;       }
	void setRootObserverOfTree(MRootObserver* rootObserver)			{	this->rootObserver=rootObserver;	}
	MRootObserver* getRootObserverOfThisTree()						{	return rootObserver;                }

	//physics
#if USE_BULLET
	void setRigidBody(btRigidBody* rb)			{	rigidBody = rb;	}
	btRigidBody* getRigidBody()					{	return rigidBody;	}
#endif

	//flags
	void setVisited(bool flag)	{	is_Visited=flag;	}
	bool isVisited()			{	return is_Visited;	}

	//layer
	void setLayer(int layer, bool bRecursive);
	int getLayer()				{	return layerID;	}

	//mono scripts
	void attachMonoScrip(monoScriptObjectInstance* scriptInstance);
	monoScriptObjectInstance* getMonoScriptInstance(int index);
	int getMonoScriptInstanceCount();
    const std::vector<monoScriptObjectInstance*>& getAttachedMonoScripts()  {   return attachedScriptInstanceList;  }
    
    template <class T>
    T getMonoScriptInstance()
    {
        for(auto script: attachedScriptInstanceList)
        {
            if(dynamic_cast<T>(script))
                return (T)script;
        }
        
        return nullptr;
    }
    
protected:
    
    virtual void writeData(gxFile& file)    {   }
    virtual void readData(gxFile& file)     {   }

    //child: child callbacks
    virtual void onAppendObject3dChild(object3d* child);
    virtual void onRemoveObject3dChild(object3d* child);

    void writeAnimationController(gxFile& file);
    void readAnimationController(gxFile& file);

    std::string name;
	int objectID;
	object3d* parent;
	unsigned int baseFlag;
#ifdef USE_BXLIST
	bxLinkedList<object3d*> childList;
#else
	std::vector<object3d*> childList;
#endif
	gxAABBf aabb;
	gxOOBBf oobb;
	gxAnimation* animationController;
	IAnimationTrack* animationTrack;	//must not delete this pointer
	void* editorUserData;				//must not delete this pointer
	MObject3dObserver* object3DObserver;	//must not delete this pointer
	MRootObserver* rootObserver;			//must not delete this pointer
	MEngineObserver* engineObserver;		//must not delete this pointer
#if USE_BULLET
	btRigidBody* rigidBody;	//must not delete this pointer
#endif
	bool is_Visited;
	int layerID;

	std::vector<monoScriptObjectInstance*> attachedScriptInstanceList;
    
private:
    void updateAnimationFrameToObject3d(int frame);
    void setAnimationTrack(IAnimationTrack* track);
    void clearAnimTrackOnAllNodes();
    
    //std::map<std::string, IComponent*> components;
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