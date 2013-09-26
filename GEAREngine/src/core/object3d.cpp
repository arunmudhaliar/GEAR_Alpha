#include "object3d.h"

extern "C" {
extern DllExport const char* object3d_getName(object3d* obj)
{
	return obj->getName();
}

extern DllExport int object3d_getID(object3d* obj)
{
	return obj->getID();
}
extern DllExport object3d* object3d_find(object3d* obj, const char* name)
{
	return obj->find(name);
}
extern DllExport int object3d_getChildCount(object3d* obj)
{
	return obj->getChildCount();
}
extern DllExport object3d* object3d_getChild(object3d* obj, int index)
{
	return obj->getChild(index);
}

extern DllExport gxAnimation* object3d_createAnimationController(object3d* obj)
{
	return obj->createAnimationController();
}

extern DllExport gxAnimation* object3d_getAnimationController(object3d* obj)
{
	return obj->getAnimationController();
}

extern DllExport gxAnimationSet* object3d_applyAnimationSetRecursive(object3d* obj, int index)
{
	return obj->applyAnimationSetRecursive(index);
}

extern DllExport object3d* object3d_appendChild(object3d* obj, object3d* child)
{
	return obj->appendChild(child);
}

extern DllExport bool object3d_removeChild(object3d* obj, object3d* child)
{
	return obj->removeChild(child);
}

extern DllExport object3d* object3d_getParent(object3d* obj)
{
	return obj->getParent();
}
}

object3d::object3d(int objID):
	transform(),
	m_iObjectID(objID)
{
	setObject3dObserver(NULL);
	setEditorUserData(NULL);
	m_pParentPtr=NULL;
	memset(m_cszName, 0, sizeof(m_cszName));
	m_eBaseFlags=0;
	setBaseFlag(eObject3dBaseFlag_Visible);
	m_pAnimationController=NULL;
	m_pAnimationTrack=NULL;
	m_iFileCRC=0;
}

object3d::~object3d()
{
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		GX_DELETE(obj);
	}
	m_cChilds.clear();

	//if(m_pAnimationTrack)
	//	m_pAnimationTrack->setObject3d(NULL);
	m_iFileCRC=0;
	m_pParentPtr=NULL;
	m_pAnimationTrack=NULL;
	GX_DELETE(m_pAnimationController);
}

void object3d::update(float dt)
{
	if(m_pAnimationController)
	{
		m_pAnimationController->update(dt);
	}

	if(m_pAnimationTrack)
	{
		updateAnimationFrameToObject3d();
	}

	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->update(dt);
	}
}

void object3d::updateAnimationFrameToObject3d()
{
	matrix4x4f* trackInfo=m_pAnimationTrack->getTrack();
	int currentFrame = m_pAnimationTrack->getCurrentFrame();
	float* local_tm = m;

	*(matrix4x4f*)this = trackInfo[currentFrame];
}

void object3d::render()
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

	//glPushMatrix();
	//glMultMatrixf(getWorldMatrix()->getMatrix());
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->render();
	}
	//glPopMatrix();
}

bool object3d::removeChild(object3d* child)
{
	int old_sz=m_cChilds.size();
	if(old_sz==0) return false;

	m_cChilds.erase(std::remove(m_cChilds.begin(), m_cChilds.end(), child), m_cChilds.end());

	if((old_sz>m_cChilds.size()))
	{
		if(m_pObject3dObserver)
			m_pObject3dObserver->onObject3dChildRemove(child);
		child->setParent(NULL);
		//child->setAnimationTrack(NULL);	nned to test fully
		return true;
	}
	else
	{
		for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
		{
			object3d* obj = *it;
			if(obj->removeChild(child))
				return true;
		}
	}
	return false;
}

void object3d::transformationChangedf()
{
	object3d* parent=getParent();
	if(parent)
		m_cWorldMatrix = *(parent->getWorldMatrix()) * *this;
	else
		m_cWorldMatrix = *this;

	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->transformationChangedf();
	}
}

void object3d::calculateAABB()
{
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->calculateAABB();
	}
}

object3d* object3d::appendChild(object3d* child)
{
	child->setParent(this);
	if(m_pObject3dObserver)
		m_pObject3dObserver->onObject3dChildAppend(child);
	m_cChilds.push_back(child);
	child->transformationChangedf();
	return child;
}

gxAnimation* object3d::createAnimationController()
{
	if(m_pAnimationController==NULL)
		m_pAnimationController = new gxAnimation();

	return m_pAnimationController;
}

void object3d::setAnimationTrack(gxAnimationTrack* track)
{
	m_pAnimationTrack=track;
	//if(m_pAnimationTrack)
	//	m_pAnimationTrack->setObject3d(this);
}

object3d* object3d::find(const char* name)
{
	if(strcmp(m_cszName, name)==0)
	{
		return this;
	}

	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		object3d* return_obj = obj->find(name);
		if(return_obj)
			return return_obj;
	}

	return NULL;
}

gxAnimationSet* object3d::applyAnimationSetRecursive(int index)
{
	if(m_pAnimationController==NULL)
		return NULL;

	gxAnimationSet* animSet=m_pAnimationController->getAnimationSetList()->at(index);
	std::vector<gxAnimationTrack*>* trackList=animSet->getTrackList();
	for(std::vector<gxAnimationTrack*>::iterator it = trackList->begin(); it != trackList->end(); ++it)
	{
		gxAnimationTrack* animationTrack = *it;
		object3d* obj_found=find(animationTrack->getName());
		if(obj_found)
		{
			obj_found->setAnimationTrack(animationTrack);
		}
	}


	return animSet;
}

void object3d::write(gxFile& file)
{
	file.Write(m_iObjectID);
	file.Write(m_eBaseFlags);
	file.Write(m_cszName);
	file.WriteBuffer((unsigned char*)m, sizeof(m));
	file.WriteBuffer((unsigned char*)&m_cAABB, sizeof(m_cAABB));
	file.Write(m_iFileCRC);
	writeAnimationController(file);
	file.Write((int)m_cChilds.size());
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->write(file);
	}
}

void object3d::read(gxFile& file)
{
	file.Read(m_eBaseFlags);
	char* temp=file.ReadString();
	strcpy(m_cszName, temp);
	GX_DELETE_ARY(temp);
	file.ReadBuffer((unsigned char*)m, sizeof(m));
	file.ReadBuffer((unsigned char*)&m_cAABB, sizeof(m_cAABB));
	file.Read(m_iFileCRC);
	readAnimationController(file);
}

void object3d::writeAnimationController(gxFile& file)
{
	if(m_pAnimationController)
	{
		file.Write(true);
		m_pAnimationController->write(file);
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
		gxAnimation* animationController=createAnimationController();
		animationController->read(file);
	}
}