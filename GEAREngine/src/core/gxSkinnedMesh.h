#pragma once

#include "gxMesh.h"

class DECLSPEC gxSkinnedMesh : public gxMesh
{
public:
	gxSkinnedMesh();
	~gxSkinnedMesh();

	void update(float dt);
	void render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);

	int* allocateBoneInfluenceCountBuffer(int nTris);
	int* allocateBoneIndexBuffer(int nCount);
	float* allocateWeightBuffer(int nCount);

	void allocateBoneList(int nBones);
	void populateBoneList(object3d* bone, int index);

	float* allocateAndCopyVertexCopyBuffer();

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);

	void clearPrivateIterator()	{	m_iPrivateIterator=0;	}

	void setRootNode(object3d* rootNode)	{	m_pRootNodePtr = rootNode;	}

private:
	int* m_pszBoneInfluenceCountBuffer;
	int* m_pszBoneIndexBuffer;
	float* m_pszWeightBuffer;
	int m_nBoneInfluencePerVertex;
	float* m_pszVertexCopyBuffer;

	int m_iPrivateIterator;
	int m_nBones;
	int m_nBoneIndexBuffer;
	object3d** m_pszBoneList;
	matrix4x4f* m_pszInvBoneTMList;
	matrix4x4f* m_pszBoneOffsetList;
	object3d* m_pRootNodePtr;
};