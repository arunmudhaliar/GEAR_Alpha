#ifndef GXSKINNEDMESH_H
#define GXSKINNEDMESH_H

#include "gxMesh.h"

class DllExport gxSkinnedMesh : public gxMesh
{
public:
	gxSkinnedMesh();
	~gxSkinnedMesh();

	void update(float dt);
	void render(gxRenderer* renderer, object3d* light);

	int* allocateBoneIndexBuffer(int nTris, int nBoneInfluencePerVertex);
	float* allocateWeightBuffer(int nTris, int nBoneInfluencePerVertex);

	void allocateBoneList(int nBones);
	void populateBoneList(object3d* bone, int index);

	float* allocateAndCopyVertexCopyBuffer();

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);

	void clearPrivateIterator()	{	m_iPrivateIterator=0;	}

private:
	int* m_pszBoneIndexBuffer;
	float* m_pszWeightBuffer;
	int m_nBoneInfluencePerVertex;
	float* m_pszVertexCopyBuffer;

	int m_iPrivateIterator;
	int m_nBones;
	object3d** m_pszBoneList;
	matrix4x4f* m_pszInvBoneTMList;
	vector3f* m_pszBoneOffsetList;
};

#endif