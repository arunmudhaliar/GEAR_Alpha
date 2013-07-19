#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "transform.h"
#include <vector>
#include <algorithm>
#include "basicIncludes.h"
#include "aabb.h"

class object3d : public transform
{
public:

	enum EOBJEC3DTFLAGS
	{
		eObject3dBaseFlag_None		= 0,
		eObject3dBaseFlag_Visible	= (1<<0)
	};

	object3d(int objID):
	  transform(),
		m_iObjectID(objID)
	{
		m_pParentPtr=NULL;
		memset(m_cszName, 0, sizeof(m_cszName));
		m_eBaseFlags=0;
		setBaseFlag(eObject3dBaseFlag_Visible);
	}

	virtual ~object3d()
	{
		for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
		{
			object3d* obj = *it;
			GX_DELETE(obj);
		}
		m_cChilds.clear();
	}

	virtual void update(float dt)
	{
		for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
		{
			object3d* obj = *it;
			obj->update(dt);
		}
	}

	virtual void render()
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

	bool removeChild(object3d* child)
	{
		int old_sz=m_cChilds.size();
		m_cChilds.erase(std::remove(m_cChilds.begin(), m_cChilds.end(), child), m_cChilds.end());

		if((old_sz>m_cChilds.size()))
		{
			child->setParent(NULL);
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

	virtual void transformationChangedf()
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

	virtual void calculateAABB()
	{
		for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
		{
			object3d* obj = *it;
			obj->calculateAABB();
		}
	}

	int getID()				{	return m_iObjectID;	}
	const char* getName()	{	return m_cszName;	}
	void setName(const char* name)	{	GX_STRCPY(m_cszName, name);	}

	object3d* appendChild(object3d* child)	{	child->setParent(this); m_cChilds.push_back(child); child->transformationChangedf(); return child;	}
	std::vector<object3d*>* getChildList()	{	return &m_cChilds;	}

	void setParent(object3d* pParentPtr)	{	m_pParentPtr=pParentPtr;	}
	object3d* getParent()					{	return m_pParentPtr;		}

	void	resetAllBaseFlags()						{	m_eBaseFlags=0;						}
	void	setBaseFlag(EOBJEC3DTFLAGS eFlags)		{	m_eBaseFlags=m_eBaseFlags|eFlags;	};
	void	reSetBaseFlag(EOBJEC3DTFLAGS eFlags)	{	m_eBaseFlags=m_eBaseFlags&~eFlags;	};
	bool	isBaseFlag(EOBJEC3DTFLAGS eFlags)		{	return (m_eBaseFlags&eFlags)?true:false;	};
	int		getBaseFlag()							{	return m_eBaseFlags;				}

	gxAABBf& getAABB()	{	return m_cAABB;	}

protected:
	char m_cszName[64];
	int m_iObjectID;
	object3d* m_pParentPtr;
	unsigned int m_eBaseFlags;
	std::vector<object3d*> m_cChilds;
	gxAABBf m_cAABB;
};

#endif