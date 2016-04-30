#pragma once

#include "gxMesh.h"

class DECLSPEC gxSkinnedMesh : public gxMesh
{
public:
	~gxSkinnedMesh();

    static gxSkinnedMesh* create(monoClassDef* script, object3d* obj);

	void update(float dt);
	void render(gxRenderer* renderer, monoScriptObjectInstance* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);

	int* allocateBoneInfluenceCountBuffer(int nTris);
	int* allocateBoneIndexBuffer(int nCount);
	float* allocateWeightBuffer(int nCount);

	void allocateBoneList(int nBones);
	void populateBoneList(object3d* bone, int index);

	float* allocateAndCopyVertexCopyBuffer();
	void clearPrivateIterator()             {	privateIterator=0;          }

protected:
    virtual void writeScriptObject(gxFile& file);
    virtual void readScriptObject(gxFile& file);

private:
    gxSkinnedMesh(monoClassDef* script, object3d* obj);
    gxSkinnedMesh(){}
    
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
};