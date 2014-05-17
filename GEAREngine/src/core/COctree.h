#ifndef OCTREE_H
#define OCTREE_H

#include "gxWorld.h"
#include "frustum.h"
#include "../util/gxExpandableArray.h"

class gxWorld;

#define MAX_OCTREECHILD	8
class DllExport OctreeNode
{
public:
	OctreeNode();
	~OctreeNode();

	void appendObject(object3d* transformObj);
	int	getLevel();
	int	getNumOfObjects();
	object3d* getObject(int index);
	void setAABB(vector3f min_, vector3f max_);
	void setLevel(int level)								{	m_iLevel = level;			}
	void setChild(int index, OctreeNode* child)			{	m_pszChild[index]=child;	}
	OctreeNode* getChild(int index)							{	return m_pszChild[index];	}
	std::vector<object3d*>* getObjectList()				{	return &m_cObjLst;			}

	gxAABBf& getAABB()		{	return m_oAABB;		}
	float getRadius()				{	return m_fRadius;	}

private:
	OctreeNode* m_pszChild[MAX_OCTREECHILD];
	int m_iLevel;
	std::vector<object3d*> m_cObjLst;
	gxAABBf m_oAABB;
	float m_fRadius;
};

class DllExport COctree
{
public:
	COctree();
	~COctree();

	bool createOctree(gxWorld* world, int minTransformObj=10, int maxLevel=8);
	void pushToRootNode(object3d* obj);
	void overlapWithChildNodes(OctreeNode* node);
	void reset();
	OctreeNode* getRoot()	{	return m_pRootNode;	}
	void drawOctree(OctreeNode* node, gxHWShader* shader);
	void resetCollidedTransformObjList();
	void checkOverlapWithOctree(OctreeNode* node, object3d* obj);
	void checkFrustumOverlapWithOctree(OctreeNode* node, gxFrustumf* frustum);
	
	int	getLevelsReached()	{	return m_nLevelReached;}
	ExpandableArray<object3d*>* getCollidedObjList()	{	return &m_cCollidedObjLst;		}
private:
	bool create(OctreeNode* node);

	int m_nLevelReached;
	ExpandableArray<object3d*> m_cCollidedObjLst;
	int	m_nMinTransformObj;
	int m_nMaxLevel;
	OctreeNode*	m_pRootNode;
};
#endif