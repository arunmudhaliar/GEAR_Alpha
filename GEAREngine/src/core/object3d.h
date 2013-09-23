#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "transform.h"
#include <vector>
#include <algorithm>
#include "basicIncludes.h"
#include "aabb.h"
#include "gxAnimation.h"
#include "../util/gxFile.h"

class DllExport MObject3dObserver
{
public:
	virtual void onObject3dChildAppend(object3d* child)=0;
	virtual void onObject3dChildRemove(object3d* child)=0;
};

class gxAnimation;
class gxAnimationSet;
class DllExport object3d : public transform
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

	void updateAnimationFrameToObject3d();
	virtual void render();

	bool removeChild(object3d* child);

	virtual void transformationChangedf();

	virtual void calculateAABB();

	int getID()				{	return m_iObjectID;	}
	const char* getName()	{	return m_cszName;	}
	void setName(const char* name)	{	GX_STRCPY(m_cszName, name);	}

	object3d* appendChild(object3d* child);
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

	object3d* find(const char* name);

	//animation
	gxAnimation* createAnimationController();
	gxAnimation* getAnimationController()				{	return m_pAnimationController;	}
	gxAnimationSet* applyAnimationSetRecursive(int index);
	void setAnimationTrack(gxAnimationTrack* track);

	void setFileCRC(int crc)	{	m_iFileCRC = crc;	}
	int getFileCRC()			{	return m_iFileCRC;	}

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);

	void writeAnimationController(gxFile& file);
	void readAnimationController(gxFile& file);

	void setEditorUserData(void* userData)	{	m_pEditorUserDataPtr=userData;	}
	void* getEditorUserData()				{	return m_pEditorUserDataPtr;	}

	void setObject3dObserver(MObject3dObserver* observer)	{	m_pObject3dObserver = observer;	}

protected:
	char m_cszName[64];
	int m_iObjectID;
	object3d* m_pParentPtr;
	unsigned int m_eBaseFlags;
	std::vector<object3d*> m_cChilds;
	gxAABBf m_cAABB;
	gxAnimation* m_pAnimationController;
	gxAnimationTrack* m_pAnimationTrack;	//must not delete this pointer
	int m_iFileCRC;
	void* m_pEditorUserDataPtr;				//must not delete this pointer
	MObject3dObserver* m_pObject3dObserver;	//must not delete this pointer
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
}

#endif