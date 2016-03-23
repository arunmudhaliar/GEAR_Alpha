#include "COctree.h"

OctreeNode::OctreeNode()
{
	for(int x=0;x<MAX_OCTREECHILD;x++)
		childs[x]=NULL;

	treeLevel=0;
	radius=0.0f;
}

OctreeNode::~OctreeNode()
{
    for(auto obj : objectList)
        REF_RELEASE(obj);
	objectList.clear();
    
	for(int x=0;x<MAX_OCTREECHILD;x++)
		GX_DELETE(childs[x]);
}

int OctreeNode::getLevel()
{
	return treeLevel;
}

int OctreeNode::getNumOfObjects()
{
	return (int)objectList.size();
}

object3d* OctreeNode::getObject(int index)
{
	return objectList.at(index);
}

void OctreeNode::setAABB(vector3f min_, vector3f max_)
{
	aabb.set(min_, max_);
	radius=(max_-min_).length()*0.5f;
}

void OctreeNode::appendObject(object3d* transformObj)
{
	objectList.push_back(transformObj);
    REF_RETAIN(transformObj);
}

//====================================================================
//============================OCTREE==================================
//====================================================================
COctree::COctree()
{
	minimumTransformObjectToBreak = 0;
	maximumLevelToBreak = 0;
	rootNode = NULL;
	noOfLevelsReached=0;
}

COctree::~COctree()
{
	reset();
}

void COctree::reset()
{
	collidedObjectList.Clear();
	collidedAlphaObjectList.Clear();
	GX_DELETE(rootNode);
}

bool COctree::createOctree(gxWorld* world, int minTransformObj/* =4 */, int maxLevel/* =8 */)
{
	collidedObjectList.Init(30, 10);
	collidedAlphaObjectList.Init(30, 10);
	rootNode=new OctreeNode();
	minimumTransformObjectToBreak=minTransformObj;
	maximumLevelToBreak=maxLevel;

	//root level is 0
	rootNode->setLevel(0);

	//set the AABB for the root node
	rootNode->setAABB(world->getAABB().m_min, world->getAABB().m_max);

	//push all mesh in to the root node
	pushToRootNode(world);

	return true;
}

void COctree::pushToRootNode(object3d* obj3d)
{
	if(obj3d && (obj3d->getID()==OBJECT3D_MESH || obj3d->getID()==OBJECT3D_SKINNED_MESH))
	{
		rootNode->appendObject(obj3d);
		create(rootNode, obj3d);
	}

    const std::vector<object3d*>* childList=obj3d->getChildList();
    for (auto childobj : *childList)
	{
		pushToRootNode(childobj);
	}
}

bool COctree::create(OctreeNode* node, object3d* obj3d)
{
	if(!node || node->getLevel()>=maximumLevelToBreak || node->getNumOfObjects()<=minimumTransformObjectToBreak)
		return true;

	//chk the overlapping objects
	overlapWithObject(node, obj3d);

	//here we can clear the objects from the node

	for(int x=0;x<MAX_OCTREECHILD;x++)
		create(node->getChild(x), obj3d);

	return false;
}

void COctree::overlapWithObject(OctreeNode* node, object3d* obj3d)
{
	object3d* transform=obj3d;
	gxAABBf oAABB(node->getAABB());
	oAABB.scale(0.5f);
	vector3f sz(oAABB.getSize());
	sz=sz*0.5f;

	gxAABBf oAABBScaled(oAABB);
	//chk collision with first quadrant
	oAABBScaled.translate(sz.x, sz.y, sz.z);
	if(transform->getAABB().isOverLap(oAABBScaled))
	{
		if(node->getChild(0)==NULL)
		{
			node->setChild(0, new OctreeNode());
			node->getChild(0)->setLevel(node->getLevel()+1);
			node->getChild(0)->setAABB(oAABBScaled.m_min, oAABBScaled.m_max);
			noOfLevelsReached=node->getChild(0)->getLevel();
		}
		node->getChild(0)->appendObject(transform);
	}

	//chk collision with second quadrant
	oAABBScaled.translate(0, 0, -sz.z*2.0f);
	if(transform->getAABB().isOverLap(oAABBScaled))
	{
		if(node->getChild(1)==NULL)
		{
			node->setChild(1, new OctreeNode());
			node->getChild(1)->setLevel(node->getLevel()+1);
			node->getChild(1)->setAABB(oAABBScaled.m_min, oAABBScaled.m_max);
			noOfLevelsReached=node->getChild(1)->getLevel();
		}
		node->getChild(1)->appendObject(transform);
	}

	//chk collision with third quadrant
	oAABBScaled.translate(-sz.x*2.0f, 0, 0);
	if(transform->getAABB().isOverLap(oAABBScaled))
	{
		if(node->getChild(2)==NULL)
		{
			node->setChild(2, new OctreeNode());
			node->getChild(2)->setLevel(node->getLevel()+1);
			node->getChild(2)->setAABB(oAABBScaled.m_min, oAABBScaled.m_max);
			noOfLevelsReached=node->getChild(2)->getLevel();
		}
		node->getChild(2)->appendObject(transform);
	}

	//chk collision with fourth quadrant
	oAABBScaled.translate(0, 0, sz.z*2.0f);
	if(transform->getAABB().isOverLap(oAABBScaled))
	{
		if(node->getChild(3)==NULL)
		{
			node->setChild(3, new OctreeNode());
			node->getChild(3)->setLevel(node->getLevel()+1);
			node->getChild(3)->setAABB(oAABBScaled.m_min, oAABBScaled.m_max);
			noOfLevelsReached=node->getChild(3)->getLevel();
		}
		node->getChild(3)->appendObject(transform);
	}

	//chk collision with fifth quadrant
	oAABBScaled.translate(sz.x*2.0f, -sz.y*2.0f, 0);
	if(transform->getAABB().isOverLap(oAABBScaled))
	{
		if(node->getChild(4)==NULL)
		{
			node->setChild(4, new OctreeNode());
			node->getChild(4)->setLevel(node->getLevel()+1);
			node->getChild(4)->setAABB(oAABBScaled.m_min, oAABBScaled.m_max);
			noOfLevelsReached=node->getChild(4)->getLevel();
		}
		node->getChild(4)->appendObject(transform);
	}

	//chk collision with sixth quadrant
	oAABBScaled.translate(0, 0, -sz.z*2.0f);
	if(transform->getAABB().isOverLap(oAABBScaled))
	{
		if(node->getChild(5)==NULL)
		{
			node->setChild(5, new OctreeNode());
			node->getChild(5)->setLevel(node->getLevel()+1);
			node->getChild(5)->setAABB(oAABBScaled.m_min, oAABBScaled.m_max);
			noOfLevelsReached=node->getChild(5)->getLevel();
		}
		node->getChild(5)->appendObject(transform);
	}

	//chk collision with seventh quadrant
	oAABBScaled.translate(-sz.x*2.0f, 0, 0);
	if(transform->getAABB().isOverLap(oAABBScaled))
	{
		if(node->getChild(6)==NULL)
		{
			node->setChild(6, new OctreeNode());
			node->getChild(6)->setLevel(node->getLevel()+1);
			node->getChild(6)->setAABB(oAABBScaled.m_min, oAABBScaled.m_max);
			noOfLevelsReached=node->getChild(6)->getLevel();
		}
		node->getChild(6)->appendObject(transform);
	}

	//chk collision with eighth quadrant
	oAABBScaled.translate(0, 0, sz.z*2.0f);
	if(transform->getAABB().isOverLap(oAABBScaled))
	{
		if(node->getChild(7)==NULL)
		{
			node->setChild(7, new OctreeNode());
			node->getChild(7)->setLevel(node->getLevel()+1);
			node->getChild(7)->setAABB(oAABBScaled.m_min, oAABBScaled.m_max);
			noOfLevelsReached=node->getChild(7)->getLevel();
		}
		node->getChild(7)->appendObject(transform);
	}
}

void COctree::resetCollidedTransformObjList()
{
	if(collidedObjectList.GetCount()==0 && collidedAlphaObjectList.GetCount()==0) return;

	ExpandableArrayNode<object3d*>* collidedtransformObjNode=collidedObjectList.GetRoot();
	int count=collidedObjectList.GetCount();
	while(collidedtransformObjNode && count--)
	{
		collidedtransformObjNode->GetData()->setVisited(false);
		collidedtransformObjNode=collidedtransformObjNode->GetNext();
	}
	collidedObjectList.Clear();

	//alpha list
	ExpandableArrayNode<object3d*>* collidedtransformAlphaObjNode=collidedAlphaObjectList.GetRoot();
	int alpha_count=collidedAlphaObjectList.GetCount();
	while(collidedtransformAlphaObjNode && alpha_count--)
	{
		collidedtransformAlphaObjNode->GetData()->setVisited(false);
		collidedtransformAlphaObjNode=collidedtransformAlphaObjNode->GetNext();
	}
	collidedAlphaObjectList.Clear();
}

void COctree::checkOverlapWithOctree(OctreeNode* node, object3d* obj3d)
{
	if(!node || !obj3d) return;

	//sphere overlap test
	float obj_radius=obj3d->getAABB().getLongestAxis()*0.5f;
	vector3f obj_center(obj3d->getAABB().getCenter());
	if(!node->getAABB().isOverlapsSphere(obj_radius, obj_center)) return;


	//chk if the curent node collides with 'obj'
	if(!node->getAABB().isOverLap(obj3d->getAABB())) return;

	//chk if the meshes collide with 'obj'
	const std::vector<object3d*>* list=node->getObjectList();
    for(auto transf : *list)
	{
		if(!transf->isVisited())
		{
			if(obj3d->getAABB().isOverLap(transf->getAABB()))
			{
				transf->setVisited(true);
				if(transf->isBaseFlag(object3d::eObject3dBaseFlag_Alpha))
					collidedAlphaObjectList.Append(transf);
				else
					collidedObjectList.Append(transf);
			}
		}
	}

	for(int x=0;x<MAX_OCTREECHILD;x++)
		checkOverlapWithOctree(node->getChild(x), obj3d);
}

void COctree::checkFrustumOverlapWithOctree(OctreeNode* node, gxFrustumf* frustum, unsigned int cullingmask)
{
	if(!node || !frustum) return;

	//chk if the curent node collides with 'obj'
	int result=frustum->isAABBInside(node->getAABB());
	if(result==0) return;

	if(result==1)	//completely inside
	{
		const std::vector<object3d*>* list=node->getObjectList();
        for(auto transf : *list)
		{
			if(!transf->isVisited() && ((1<<transf->getLayer())&cullingmask))
			{
				transf->setVisited(true);
				if(transf->isBaseFlag(object3d::eObject3dBaseFlag_Alpha))
					collidedAlphaObjectList.Append(transf);
				else
					collidedObjectList.Append(transf);
			}
		}
		return;
	}

	//chk if the meshes collide with 'obj'
	const std::vector<object3d*>* list=node->getObjectList();
    for(auto transf : *list)
    {
		if(!transf->isVisited() && ((1<<transf->getLayer())&cullingmask))
		{
			if(frustum->isAABBInside(transf->getAABB()))
			{
				transf->setVisited(true);
				if(transf->isBaseFlag(object3d::eObject3dBaseFlag_Alpha))
					collidedAlphaObjectList.Append(transf);
				else
					collidedObjectList.Append(transf);
			}
		}
	}
	
	for(int x=0;x<MAX_OCTREECHILD;x++)
		checkFrustumOverlapWithOctree(node->getChild(x), frustum, cullingmask);
}

object3d* COctree::pickBruteForce(vector3f& rayOrig, vector3f& rayDir)
{
	if(collidedObjectList.GetCount()==0 && collidedAlphaObjectList.GetCount()==0) return NULL;
	if(rayDir.lengthSquared()==0) return NULL;

	std::vector<object3d*> raystartsfrominideobjects;
	float smallest_distance=1e32f;
	object3d* selectedObj=NULL;
	ExpandableArrayNode<object3d*>* collidedtransformObjNode=collidedObjectList.GetRoot();
	int count=collidedObjectList.GetCount();
	while(collidedtransformObjNode && count--)
	{
		object3d* obj3d=collidedtransformObjNode->GetData();
		float distance=obj3d->getAABB().getRayIntersection(rayOrig, rayDir);
		if(distance<=smallest_distance && distance>=0.0f && obj3d->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
		{
			if(distance==0.0f)
			{
				raystartsfrominideobjects.push_back(obj3d);
			}
			selectedObj=obj3d;
			smallest_distance=distance;
		}
		collidedtransformObjNode=collidedtransformObjNode->GetNext();
	}

	//alpha list
	ExpandableArrayNode<object3d*>* collidedtransformAlphaObjNode=collidedAlphaObjectList.GetRoot();
	int alpha_count=collidedAlphaObjectList.GetCount();
	while(collidedtransformAlphaObjNode && alpha_count--)
	{
		object3d* obj3d=collidedtransformAlphaObjNode->GetData();
		float distance=obj3d->getAABB().getRayIntersection(rayOrig, rayDir);
		if(distance<=smallest_distance && distance>=0.0f && obj3d->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
		{
			if(distance==0.0f)//TODO : getRayIntersection() returns 0.0, so '== 0.0f' is a valid statement here.
			{
				raystartsfrominideobjects.push_back(obj3d);
			}
			selectedObj=obj3d;
			smallest_distance=distance;
		}
		collidedtransformAlphaObjNode=collidedtransformAlphaObjNode->GetNext();
	}

	smallest_distance=1e32f;
	//check if anyobject completely covers over ray origin
	for(std::vector<object3d*>::iterator it = raystartsfrominideobjects.begin(); it != raystartsfrominideobjects.end(); ++it)
	{
		object3d* obj3d = *it;
		vector3f v=obj3d->getAABB().getCenter()-rayOrig;
		float distance=v.length();
		if(distance<=smallest_distance)
		{
			selectedObj=obj3d;
			smallest_distance=distance;
		}
	}
	raystartsfrominideobjects.clear();

	return selectedObj;
}

void COctree::drawOctree(OctreeNode* node, gxHWShader* shader)
{
	if(!node) return;

#if defined (USE_ProgrammablePipeLine)
    float r=(float)node->getLevel()/(float)maximumLevelToBreak;
	if(node->getLevel()<maximumLevelToBreak)
	{
		shader->sendUniform4f("u_diffuse_v4", r, 0.0f, 0.0f, 1.0f);
		node->getAABB().draw(shader);
	}
	else
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		shader->sendUniform4f("u_diffuse_v4", r, 0.0f, 0.0f, 0.8f);
		node->getAABB().draw(shader);
		glDisable(GL_BLEND);
	}
#else
	glDisable(GL_LIGHTING);
	float r=(float)node->getLevel()/(float)maximumLevelToBreak;
	if(node->getLevel()<maximumLevelToBreak)
	{
		glColor4f(r, 0.0f, 0.0f, 1.0f);
		node->getAABB().draw();
	}
	else
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r, 0.0f, 0.0f, 0.8f);
		node->getAABB().draw(true);
		glDisable(GL_BLEND);
	}
#endif
    
	for(int x=0;x<MAX_OCTREECHILD;x++)
		drawOctree(node->getChild(x), shader);
}
