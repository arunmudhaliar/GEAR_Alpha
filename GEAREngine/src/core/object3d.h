#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "transform.h"
#include <vector>
#include <algorithm>
#include "basicIncludes.h"
#include "aabb.h"
#include "gxAnimation.h"

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

protected:
	char m_cszName[64];
	int m_iObjectID;
	object3d* m_pParentPtr;
	unsigned int m_eBaseFlags;
	std::vector<object3d*> m_cChilds;
	gxAABBf m_cAABB;
	gxAnimation* m_pAnimationController;
	gxAnimationTrack* m_pAnimationTrack;	//must not delete this pointer
};

#endif