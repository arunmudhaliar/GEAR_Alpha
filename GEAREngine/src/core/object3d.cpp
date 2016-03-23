#include "object3d.h"
#include "../mono/src/monoWrapper.h"

extern "C" {
extern DECLSPEC const char* object3d_getName(object3d* obj)
{
	return obj->getName().c_str();
}

extern DECLSPEC int object3d_getID(object3d* obj)
{
	return obj->getID();
}
extern DECLSPEC object3d* object3d_find(object3d* obj, const char* name)
{
	return obj->find(name);
}
extern DECLSPEC int object3d_getChildCount(object3d* obj)
{
	return obj->getChildCount();
}
extern DECLSPEC object3d* object3d_getChild(object3d* obj, int index)
{
	return obj->getChild(index);
}

extern DECLSPEC gxAnimation* object3d_createAnimationController(object3d* obj)
{
	return obj->getAnimationController();
}

extern DECLSPEC gxAnimation* object3d_getAnimationController(object3d* obj)
{
	return obj->getAnimationController();
}

extern DECLSPEC gxAnimationSet* object3d_applyAnimationSetRecursive(object3d* obj, int index)
{
	return obj->applyAnimationSetRecursive(index);
}

extern DECLSPEC object3d* object3d_appendChild(object3d* obj, object3d* child)
{
	return obj->appendChild(child);
}

extern DECLSPEC bool object3d_removeChild(object3d* obj, object3d* child)
{
	return obj->removeChild(child);
}

extern DECLSPEC object3d* object3d_getParent(object3d* obj)
{
	return obj->getParent();
}
}

object3d* object3d::create(int objID)
{
    auto newObject = new object3d(objID);
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

object3d* object3d::create()
{
    return create(OBJECT3D_OBJECT);
}

object3d::object3d(int objID):
    Ref(),
	transform(),
	GEARAsset(),
	objectID(objID)
{
	setObject3dObserver(NULL);
	setEditorUserData(NULL);
	parent=NULL;
	baseFlag=0;
	setBaseFlag(eObject3dBaseFlag_Visible);
	animationController=NULL;
	animationTrack=NULL;
	setRootObserverOfTree(NULL);
#if USE_BULLET
	rigidBody=NULL;
#endif
	engineObserver = NULL;
	is_Visited=false;

	layerID=-1;	//Currently this object is not belong to any layer
}

object3d::~object3d()
{
	if(object3DObserver)
		object3DObserver->onObject3dDestroy(this);

	if(rootObserver)
		rootObserver->callback_object3dDestroyedFromTree(this);

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		GX_DELETE(obj);
        node=node->getNext();
	}
	childList.clearAll();
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		REF_RELEASE(obj);
	}
	childList.clear();
#endif

	for(std::vector<monoScriptObjectInstance*>::iterator it = attachedScriptInstanceList.begin(); it != attachedScriptInstanceList.end(); ++it)
	{
		monoScriptObjectInstance* scriptinstance = *it;
		GX_DELETE(scriptinstance);
	}
	attachedScriptInstanceList.clear();

    REF_RELEASE(animationTrack);
	//if(animationTrack)
	//	animationTrack->setObject3d(NULL);
    
	assetFileCRC=0;
	parent=NULL;
	animationTrack=NULL;
	REF_RELEASE(animationController);
}

object3d* object3d::clone()
{
    //Not implemented
    assert(false);
    return nullptr;
}

void object3d::resetAllBaseFlags(bool recursive)
{
	baseFlag=0;

	if(recursive)
	{
		for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
		{
			object3d* obj = *it;
			obj->resetAllBaseFlags(recursive);
		}
	}
}

void object3d::setBaseFlag(EOBJEC3DTFLAGS eFlags, bool recursive)
{
	baseFlag=baseFlag|eFlags;

	if(recursive)
	{
		for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
		{
			object3d* obj = *it;
			obj->setBaseFlag(eFlags, recursive);
		}
	}
}

void object3d::reSetBaseFlag(EOBJEC3DTFLAGS eFlags, bool recursive)
{
	baseFlag=baseFlag&~eFlags;
	
	if(recursive)
	{
		for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
		{
			object3d* obj = *it;
			obj->reSetBaseFlag(eFlags, recursive);
		}
	}
}

void object3d::setObject3dObserverRecursive(MObject3dObserver* observer)
{
	setObject3dObserver(observer);

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->setObject3dObserverRecursive(observer);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		obj->setObject3dObserverRecursive(observer);
	}
#endif
}

void object3d::update(float dt)
{
#if USE_BULLET
	if(rigidBody)
	{
		btTransform physics_tm;
		physics_tm=rigidBody->getWorldTransform();
		object3d* parent=getParent();	//we need to get the root parent instead of immediate parent.

		if(parent)
		{
			physics_tm.getOpenGLMatrix(worldTransformationMatrix.getOGLMatrix());
			matrix4x4f* p = this;
			*p = (parent->getWorldMatrix()->getInverse()) * worldTransformationMatrix;

			transformationChangedf();
		}
		else
		{
			physics_tm.getOpenGLMatrix(getOGLMatrix());
			transformationChangedf();
		}
	}
#endif

//#ifdef _WIN32	//arun:special case
	if(animationController)
	{
		animationController->update(dt);
		updateAnimationFrameToObject3d((int)animationController->getCurrentFrame());
	}
//#endif

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->update(dt);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		obj->update(dt);
	}
#endif
}

void object3d::attachMonoScrip(monoScript* script)
{
    monoScriptObjectInstance* newscript = new monoScriptObjectInstance(script, this);
    
    attachedScriptInstanceList.push_back(newscript);
}

monoScriptObjectInstance* object3d::getMonoScriptInstance(int index)
{
    monoScriptObjectInstance* instance = attachedScriptInstanceList.at(index);
    return instance;
}

int object3d::getMonoScriptInstanceCount()
{
    return (int)attachedScriptInstanceList.size();
}

void object3d::startMono()
{
    for(auto scriptinstance : attachedScriptInstanceList)
    {
        scriptinstance->start();
    }

    //TODO: iterator is not safe since any object3d instance can be added/removed to the list at runtime.
//    for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
//    {
//        object3d* obj = *it;
//        obj->startMono();
//    }
    
    //TODO: Try optimize this loop.
    for (auto obj : childList)
    {
        obj->startMono();
    }
}

void object3d::updateMono()
{
    for(auto scriptinstance : attachedScriptInstanceList)
    {
        scriptinstance->update();
    }

    //TODO: iterator is not safe since any object3d instance can be added/removed to the list at runtime.
//	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
//	{
//		object3d* obj = *it;
//		obj->updateMono();
//	}
    
    //TODO: Try optimize this loop.
    for (auto obj : childList)
    {
        obj->updateMono();
    }
}

void object3d::stopMono()
{
    for(auto scriptinstance : attachedScriptInstanceList)
    {
        scriptinstance->stop();
    }
    
    //TODO: iterator is not safe since any object3d instance can be added/removed to the list at runtime.
//    for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
//    {
//        object3d* obj = *it;
//        obj->stopMono();
//    }
    
    //TODO: Try optimize this loop.
    for (auto obj : childList)
    {
        obj->stopMono();
    }
}

void object3d::updateAnimationFrameToObject3d(int frame)
{
	if(animationTrack)
	{
        animationTrack->getFrame(frame, *(matrix4x4f*)this);
	}

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->updateAnimationFrameToObject3d(frame);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		obj->updateAnimationFrameToObject3d(frame);
	}
#endif
}

void object3d::render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/)
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

	if((renderFlag&eObject3dBase_RenderFlag_DontRenderChilds))
		return;

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->render(renderer, light, renderFlag);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		obj->render(renderer, light, renderFlag);
	}
#endif
}

object3d* object3d::appendChild(object3d* child)
{
	child->setParent(this);
	child->setRootObserverOfTree(getRootObserverOfThisTree());

	if(object3DObserver)
		object3DObserver->onObject3dChildAppend(child);
	if(rootObserver)
		rootObserver->callback_object3dAppendToTree(child);
	if(child->getLayer()==-1)
		child->setLayer(ELAYER_DEFAULT, false);
	else
		child->setLayer(child->getLayer(), false);

    REF_RETAIN(child);
#ifdef USE_BXLIST
	childList.insertTail(child);
#else
	childList.push_back(child);
#endif

	child->transformationChangedf();
	transformationChangedf();
	onAppendObject3dChild(child);

	return child;
}

bool object3d::removeChild(object3d* child)
{
	int old_sz=(int)childList.size();
	if(old_sz==0) return false;

#ifdef USE_BXLIST
	childList.remove(child);
#else
	childList.erase(std::remove(childList.begin(), childList.end(), child), childList.end());
#endif

	if((old_sz>childList.size()))
	{
		onRemoveObject3dChild(child);
		if(object3DObserver)
			object3DObserver->onObject3dChildRemove(child);
		if(rootObserver)
			rootObserver->callback_object3dRemovedFromTree(child);
		child->setParent(NULL);
		//child->setAnimationTrack(NULL);	need to test fully
        REF_RELEASE(child);
		transformationChangedf();
		return true;
	}
	else
	{
#ifdef USE_BXLIST
		stLinkNode<object3d*>* node=childList.getHead();
		while(node)
		{
			object3d* obj=node->getData();
			if(obj->removeChild(child))
				return true;
			node=node->getNext();
		}
#else
		for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
		{
			object3d* obj = *it;
			if(obj->removeChild(child))
				return true;
		}
#endif
	}
	return false;
}

void object3d::setLayer(int layer, bool bRecursive)
{
	if(layer==layerID)
		return;

	int oldlayer=layerID;
	layerID=layer;

	if(rootObserver)
		rootObserver->callback_object3dLayerChanged(this, oldlayer);

	if(bRecursive)
	{
		for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
		{
			object3d* obj = *it;
			obj->setLayer(layer, bRecursive);
		}
	}
}

void object3d::onAppendObject3dChild(object3d* child)
{
}

void object3d::onRemoveObject3dChild(object3d* child)
{
}

void object3d::transformationChangedf()
{
	object3d* parent=getParent();
#if USE_BULLET
	if(!rigidBody)
#endif
	{
		if(parent)
			worldTransformationMatrix = *(parent->getWorldMatrix()) * *this;
		else
			worldTransformationMatrix = *this;
	}


#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
	while(node)
	{
		object3d* obj=node->getData();
		obj->transformationChangedf();
		node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		obj->transformationChangedf();
	}
#endif

	calculateAABB();
}

void object3d::calculateAABB()
{
	float min_x,min_y,min_z;
	float max_x,max_y,max_z;
	min_x=min_y=min_z=1e16f;
	max_x=max_y=max_z=-1e16f;
	
	vector3f bound_vertices[8];

	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;

		vector3f aabb_min(obj->getAABB().m_min);
		vector3f aabb_max(obj->getAABB().m_max);
		//vector3f diif(aabb_max-aabb_min);
		//if(diif.lengthSquared()==0.0f) continue;

		bound_vertices[0].set(vector3f(aabb_min.x, aabb_min.y, aabb_min.z));
		bound_vertices[1].set(vector3f(aabb_min.x, aabb_min.y, aabb_max.z));
		bound_vertices[2].set(vector3f(aabb_max.x, aabb_min.y, aabb_max.z));
		bound_vertices[3].set(vector3f(aabb_max.x, aabb_min.y, aabb_min.z));
		bound_vertices[4].set(vector3f(aabb_min.x, aabb_max.y, aabb_min.z));
		bound_vertices[5].set(vector3f(aabb_min.x, aabb_max.y, aabb_max.z));
		bound_vertices[6].set(vector3f(aabb_max.x, aabb_max.y, aabb_max.z));
		bound_vertices[7].set(vector3f(aabb_max.x, aabb_max.y, aabb_min.z));

		//m_min.x, m_min.y, m_min.z,		//0
		//m_min.x, m_min.y, m_max.z,		//1
		//m_max.x, m_min.y, m_max.z,		//2
		//m_max.x, m_min.y, m_min.z,		//3
		//m_min.x, m_max.y, m_min.z,		//4
		//m_min.x, m_max.y, m_max.z,		//5
		//m_max.x, m_max.y, m_max.z,		//6	
		//m_max.x, m_max.y, m_min.z,		//7

		for(int x=0;x<8;x++)
		{
			if(min_x>bound_vertices[x].x)		min_x	= bound_vertices[x].x;
			if(min_y>bound_vertices[x].y)		min_y	= bound_vertices[x].y;
			if(min_z>bound_vertices[x].z)		min_z	= bound_vertices[x].z;

			if(max_x<bound_vertices[x].x)		max_x	= bound_vertices[x].x;
			if(max_y<bound_vertices[x].y)		max_y	= bound_vertices[x].y;
			if(max_z<bound_vertices[x].z)		max_z	= bound_vertices[x].z;
		}
	}

	if(getID()!=OBJECT3D_WORLD)
	{
		vector3f oobb_min(getOOBB().m_min);
		vector3f oobb_max(getOOBB().m_max);

		bound_vertices[0].set(*this->getWorldMatrix() * vector3f(oobb_min.x, oobb_min.y, oobb_min.z));
		bound_vertices[1].set(*this->getWorldMatrix() * vector3f(oobb_min.x, oobb_min.y, oobb_max.z));
		bound_vertices[2].set(*this->getWorldMatrix() * vector3f(oobb_max.x, oobb_min.y, oobb_max.z));
		bound_vertices[3].set(*this->getWorldMatrix() * vector3f(oobb_max.x, oobb_min.y, oobb_min.z));
		bound_vertices[4].set(*this->getWorldMatrix() * vector3f(oobb_min.x, oobb_max.y, oobb_min.z));
		bound_vertices[5].set(*this->getWorldMatrix() * vector3f(oobb_min.x, oobb_max.y, oobb_max.z));
		bound_vertices[6].set(*this->getWorldMatrix() * vector3f(oobb_max.x, oobb_max.y, oobb_max.z));
		bound_vertices[7].set(*this->getWorldMatrix() * vector3f(oobb_max.x, oobb_max.y, oobb_min.z));

		for(int x=0;x<8;x++)
		{
			if(min_x>bound_vertices[x].x)		min_x	= bound_vertices[x].x;
			if(min_y>bound_vertices[x].y)		min_y	= bound_vertices[x].y;
			if(min_z>bound_vertices[x].z)		min_z	= bound_vertices[x].z;

			if(max_x<bound_vertices[x].x)		max_x	= bound_vertices[x].x;
			if(max_y<bound_vertices[x].y)		max_y	= bound_vertices[x].y;
			if(max_z<bound_vertices[x].z)		max_z	= bound_vertices[x].z;
		}
	}
	aabb.set(vector3f(min_x, min_y, min_z), vector3f(max_x, max_y, max_z));
}

gxAnimation* object3d::getAnimationController()
{
    if(animationController==nullptr)
        animationController = gxAnimation::create();
    
    return animationController;
}

void object3d::setAnimationTrack(IAnimationTrack* track)
{
    if(track==animationTrack)
        return;
    
    REF_RELEASE(animationTrack);
    animationTrack=track;
    REF_RETAIN(animationTrack);
    
	//if(animationTrack)
	//	animationTrack->setObject3d(this);
}

object3d* object3d::find(const std::string& name)
{
	if(this->name.compare(name)==0)
	{
		return this;
	}

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		object3d* return_obj = obj->find(name);
		if(return_obj)
			return return_obj;
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		object3d* return_obj = obj->find(name);
		if(return_obj)
			return return_obj;
	}
#endif

	return NULL;
}

void object3d::clearAnimTrackOnAllNodes()
{
	setAnimationTrack(NULL);

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->clearAnimTrackOnAllNodes();
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		obj->clearAnimTrackOnAllNodes();
	}
#endif
}

gxAnimationSet* object3d::applyAnimationSetRecursive(gxAnimationSet* animset)
{
	if(animationController==NULL)
		return NULL;

	clearAnimTrackOnAllNodes();
    for(auto animationTrack : animset->getTrackList())
	{
		object3d* obj_found=find(animationTrack->getName());
		if(obj_found)
		{
			obj_found->setAnimationTrack(animationTrack);
		}
		else
		{
			DEBUG_PRINT("object3d instance not found for track %s", animationTrack->getName());
		}
	}

	return animset;
}

gxAnimationSet* object3d::applyAnimationSetRecursive(int index)
{
	gxAnimationSet* animSet=animationController->getAnimationSetList()->at(index);
	return applyAnimationSetRecursive(animSet);
}

void object3d::write(gxFile& file)
{
	file.Write(objectID);
	file.Write(baseFlag);
	file.Write(name.c_str());
	file.WriteBuffer((unsigned char*)m, sizeof(m));
	file.WriteBuffer((unsigned char*)&oobb, sizeof(oobb));
	file.Write(assetFileCRC);
	writeAnimationController(file);
    
	//
	file.Write((int)attachedScriptInstanceList.size());
	for(std::vector<monoScriptObjectInstance*>::iterator it = attachedScriptInstanceList.begin(); it != attachedScriptInstanceList.end(); ++it)
	{
		monoScriptObjectInstance* scriptinstance = *it;
		file.Write(scriptinstance->getScriptPtr()->getScriptFileName().c_str());
	}
	//

    writeData(file);
    
	file.Write((int)childList.size());
#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=childList.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->write(file);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		obj->write(file);
	}
#endif
}

void object3d::read(gxFile& file)
{
	file.Read(baseFlag);
	char* temp=file.ReadString();
    name.assign(temp, strlen(temp));
	GX_DELETE_ARY(temp);
	file.ReadBuffer((unsigned char*)m, sizeof(m));
	file.ReadBuffer((unsigned char*)&oobb, sizeof(oobb));
	file.Read(assetFileCRC);
	readAnimationController(file);

	//
	int nAttachedScripts=0;
	file.Read(nAttachedScripts);
	for(int x=0;x<nAttachedScripts;x++)
	{
		char* scriptname=file.ReadString();
		char temp_scriptname[FILENAME_MAX];
		GX_STRCPY(temp_scriptname, scriptname);
		GX_DELETE_ARY(scriptname);
		DEBUG_PRINT("attached script %s", temp_scriptname);
		monoScript* script = monoWrapper::mono_getMonoScripDef(temp_scriptname);

		attachMonoScrip(script);
		if(script==nullptr)
			DEBUG_PRINT("script==nullptr");
	}
	//
    
    readData(file);
}

void object3d::writeAnimationController(gxFile& file)
{
	if(animationController)
	{
		file.Write(true);
		animationController->write(file);
	}
	else
	{
		file.Write(false);
	}
}

void object3d::readAnimationController(gxFile& file)
{
	bool bAnimationController=false;
	file.Read(bAnimationController);
	if(bAnimationController)
	{
		gxAnimation* animationController=getAnimationController();
		animationController->read(file);
	}
}