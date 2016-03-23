#pragma once

#include "gxWorld.h"
#include "frustum.h"
#include "../util/gxExpandableArray.h"

class gxWorld;

#define MAX_OCTREECHILD	8
class DECLSPEC OctreeNode
{
public:
	OctreeNode();
	~OctreeNode();

	void appendObject(object3d* transformObj);
	int	getLevel();
	int	getNumOfObjects();
	object3d* getObject(int index);
	void setAABB(vector3f min_, vector3f max_);
	void setLevel(int level)							{	treeLevel = level;		}
	void setChild(int index, OctreeNode* child)			{	childs[index]=child;	}
	OctreeNode* getChild(int index)						{	return childs[index];	}
	const std::vector<object3d*>* getObjectList()		{	return &objectList;		}

	gxAABBf& getAABB()          {	return aabb;	}
	float getRadius()			{	return radius;	}

private:
	OctreeNode* childs[MAX_OCTREECHILD];
	int treeLevel;
	std::vector<object3d*> objectList;
	gxAABBf aabb;
	float radius;
};

class DECLSPEC COctree
{
public:
	COctree();
	~COctree();

	bool createOctree(gxWorld* world, int minTransformObj=10, int maxLevel=8);
	void pushToRootNode(object3d* obj3d);
	void reset();
	OctreeNode* getRoot()	{	return rootNode;	}
	void drawOctree(OctreeNode* node, gxHWShader* shader);
	void resetCollidedTransformObjList();
	void checkOverlapWithOctree(OctreeNode* node, object3d* obj3d);
	void checkFrustumOverlapWithOctree(OctreeNode* node, gxFrustumf* frustum, unsigned int cullingmask);
	
	int	getLevelsReached()	{	return noOfLevelsReached;   }
	ExpandableArray<object3d*>* getCollidedObjList()		{	return &collidedObjectList;			}
	ExpandableArray<object3d*>* getCollidedAlphaObjList()	{	return &collidedAlphaObjectList;	}

	object3d* pickBruteForce(vector3f& rayOrig, vector3f& rayDir);

private:
	bool create(OctreeNode* node, object3d* obj3d);
	void overlapWithObject(OctreeNode* node, object3d* obj3d);

	int noOfLevelsReached;
	ExpandableArray<object3d*> collidedObjectList;
	ExpandableArray<object3d*> collidedAlphaObjectList;
	int	minimumTransformObjectToBreak;
	int maximumLevelToBreak;
	OctreeNode*	rootNode;
};