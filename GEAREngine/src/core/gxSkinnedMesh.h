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
	void clearPrivateIterator()             {	privateIterator=0;          }

	void setRootNode(object3d* rootNode)	{	this->rootNode = rootNode;	}

protected:
    virtual void writeData(gxFile& file);
    virtual void readData(gxFile& file);

private:
	int* boneInfluenceCountBuffer;
	int* boneIndexBuffer;
	float* weightBuffer;
	int boneInfluencePerVertex;
	float* vertexCopyBuffer;

	int privateIterator;
	int noOfBones;
	int noOfBoneIndexBuffer;
	object3d** boneList;
	matrix4x4f* inverseBoneTransformationList;
	matrix4x4f* boneOffsetList;
	object3d* rootNode;
};