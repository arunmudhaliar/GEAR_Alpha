#include "COctree.h"

OctreeNode::OctreeNode()
{
	for(int x=0;x<MAX_OCTREECHILD;x++)
		m_pszChild[x]=NULL;

	m_iLevel=0;
	m_fRadius=0.0f;
}

OctreeNode::~OctreeNode()
{
	m_cObjLst.clear();
	for(int x=0;x<MAX_OCTREECHILD;x++)
		GX_DELETE(m_pszChild[x]);
}

int OctreeNode::getLevel()
{
	return m_iLevel;
}

int OctreeNode::getNumOfObjects()
{
	return m_cObjLst.size();
}

object3d* OctreeNode::getObject(int index)
{
	return m_cObjLst.at(index);
}

void OctreeNode::setAABB(vector3f min_, vector3f max_)
{
	m_oAABB.set(min_, max_);
	m_fRadius=(max_-min_).length()*0.5f;
}

void OctreeNode::appendObject(object3d* transformObj)
{
	m_cObjLst.push_back(transformObj);
}

//====================================================================
//============================OCTREE==================================
//====================================================================
COctree::COctree()
{
	m_nMinTransformObj = 0;
	m_nMaxLevel = 0;
	m_pRootNode = NULL;
	m_nLevelReached=0;
}

COctree::~COctree()
{
	reset();
}

void COctree::reset()
{
	m_cCollidedObjLst.Clear();
	GX_DELETE(m_pRootNode);
}

bool COctree::createOctree(gxWorld* world, int minTransformObj/* =4 */, int maxLevel/* =8 */)
{
	m_cCollidedObjLst.Init(30, 10);
	m_pRootNode=new OctreeNode();
	m_nMinTransformObj=minTransformObj;
	m_nMaxLevel=maxLevel;

	//push all mesh in to the root node
	pushToRootNode(world);
	//stLinkNode<object3d*>* objectnode= world->getRootList()->getHead();
	//while(objectnode)
	//{
	//	if(!(objectnode->getData()->isBaseFlag(object3d::OBJF_USE_TM)))
	//		m_pRootNode->appendObject(objectnode->getData());
	//	objectnode=objectnode->getNext();
	//}

	//root level is 0
	m_pRootNode->setLevel(0);

	//set the AABB for the root node
	m_pRootNode->setAABB(world->getAABB().m_min, world->getAABB().m_max);

	return create(m_pRootNode);
}

void COctree::pushToRootNode(object3d* obj)
{
	if(obj && obj->getID()==OBJECT3D_MESH)
		m_pRootNode->appendObject(obj);

	std::vector<object3d*>* list=obj->getChildList();
	for(std::vector<object3d*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		object3d* childobj=*it;
		pushToRootNode(childobj);
	}
}

bool COctree::create(OctreeNode* node)
{
	if(!node || node->getLevel()>=m_nMaxLevel || node->getNumOfObjects()<=m_nMinTransformObj)
		return true;

	//chk the overlapping child nodes
	overlapWithChildNodes(node);

	//here we can clear the objects from the node

	for(int x=0;x<MAX_OCTREECHILD;x++)
		create(node->getChild(x));

	return false;
}

void COctree::overlapWithChildNodes(OctreeNode* node)
{
	std::vector<object3d*>* list=node->getObjectList();
	for(std::vector<object3d*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		object3d* transform=*it;
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
				m_nLevelReached=node->getChild(0)->getLevel();
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
				m_nLevelReached=node->getChild(1)->getLevel();
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
				m_nLevelReached=node->getChild(2)->getLevel();
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
				m_nLevelReached=node->getChild(3)->getLevel();
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
				m_nLevelReached=node->getChild(4)->getLevel();
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
				m_nLevelReached=node->getChild(5)->getLevel();
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
				m_nLevelReached=node->getChild(6)->getLevel();
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
				m_nLevelReached=node->getChild(7)->getLevel();
			}
			node->getChild(7)->appendObject(transform);
		}
	}//while
}

void COctree::resetCollidedTransformObjList()
{
	if(m_cCollidedObjLst.GetCount()==0) return;

	ExpandableArrayNode<object3d*>* collidedtransformObjNode=m_cCollidedObjLst.GetRoot();
	int count=m_cCollidedObjLst.GetCount();
	while(collidedtransformObjNode && count--)
	{
		collidedtransformObjNode->GetData()->setVisited(false);
		collidedtransformObjNode=collidedtransformObjNode->GetNext();
	}
	m_cCollidedObjLst.Clear();
}

void COctree::checkOverlapWithOctree(OctreeNode* node, object3d* obj)
{
	if(!node || !obj) return;

	//sphere overlap test
	float obj_radius=obj->getAABB().getLongestAxis()*0.5f;
	vector3f obj_center(obj->getAABB().getCenter());
	if(!node->getAABB().isOverlapsSphere(obj_radius, obj_center)) return;


	//chk if the curent node collides with 'obj'
	if(!node->getAABB().isOverLap(obj->getAABB())) return;

	//chk if the meshes collide with 'obj'
	std::vector<object3d*>* list=node->getObjectList();
	for(std::vector<object3d*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		object3d* transf=*it;
		if(!transf->isVisited())
		{
			if(obj->getAABB().isOverLap(transf->getAABB()))
			{
				transf->setVisited(true);
				m_cCollidedObjLst.Append(transf);
			}
		}
	}

	for(int x=0;x<MAX_OCTREECHILD;x++)
		checkOverlapWithOctree(node->getChild(x), obj);
}

void COctree::checkFrustumOverlapWithOctree(OctreeNode* node, gxFrustumf* frustum)
{
	if(!node || !frustum) return;

	//chk if the curent node collides with 'obj'
	int result=frustum->isAABBInside(node->getAABB());
	if(result==0) return;

	if(result==1)	//completely inside
	{
		std::vector<object3d*>* list=node->getObjectList();
		for(std::vector<object3d*>::iterator it = list->begin(); it != list->end(); ++it)
		{
			object3d* transf=*it;
			if(!transf->isVisited())
			{
				transf->setVisited(true);
				m_cCollidedObjLst.Append(transf);
			}
		}
		return;
	}

	//chk if the meshes collide with 'obj'
	std::vector<object3d*>* list=node->getObjectList();
	for(std::vector<object3d*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		object3d* transf=*it;

		if(!transf->isVisited())
		{
			if(frustum->isAABBInside(transf->getAABB()))
			{
				transf->setVisited(true);
				m_cCollidedObjLst.Append(transf);
			}
		}
	}
	
	for(int x=0;x<MAX_OCTREECHILD;x++)
		checkFrustumOverlapWithOctree(node->getChild(x), frustum);
}

void COctree::drawOctree(OctreeNode* node, gxHWShader* shader)
{
	if(!node) return;

#if defined (USE_ProgrammablePipeLine)
    float r=(float)node->getLevel()/(float)m_nMaxLevel;
	if(node->getLevel()<m_nMaxLevel)
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
	float r=(float)node->getLevel()/(float)m_nMaxLevel;
	if(node->getLevel()<m_nMaxLevel)
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
