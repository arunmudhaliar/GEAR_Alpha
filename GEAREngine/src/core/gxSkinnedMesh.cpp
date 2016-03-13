#include "gxSkinnedMesh.h"

gxSkinnedMesh::gxSkinnedMesh():
gxMesh(OBJECT3D_SKINNED_MESH)
{
	boneInfluenceCountBuffer=NULL;
	boneIndexBuffer=NULL;
	weightBuffer=NULL;
	boneInfluencePerVertex=4;
	boneList=NULL;
	noOfBones=0;
	noOfBoneIndexBuffer=0;
	vertexCopyBuffer=NULL;
	inverseBoneTransformationList=NULL;
	boneOffsetList=NULL;
	rootNode=NULL;
	reSetBaseFlag(eObject3dBaseFlag_Static);
}

gxSkinnedMesh::~gxSkinnedMesh()
{
	GX_DELETE_ARY(boneInfluenceCountBuffer);
	GX_DELETE_ARY(boneIndexBuffer);
	GX_DELETE_ARY(weightBuffer);

	GX_DELETE_ARY(boneList);
	GX_DELETE_ARY(vertexCopyBuffer);
	GX_DELETE_ARY(inverseBoneTransformationList);
	GX_DELETE_ARY(boneOffsetList);
}

void gxSkinnedMesh::update(float dt)
{
	gxMesh::update(dt);

	int* boneindexbuffer=boneIndexBuffer;
	float* weightbuffer=weightBuffer;
	int tmp=noOfBoneIndexBuffer;
	matrix4x4f skinTM(*this);
	//skinTM.setPosition(0, 0, 0);
	matrix4x4f rootNodeInverse(getWorldMatrix()->getInverse());	//TODO: try optimize this by transforming the skin in transformchanged()

	for(int x=0;x<noOfTrianglesForInternalUse*3;x++)
	{
		vector3f vertex(&vertexCopyBuffer[x*3]);
		vector3f finalVertex;
		int nInfluenceBones=boneInfluenceCountBuffer[x];
		for(int y=0;y<nInfluenceBones;y++)
		{
			int boneID=boneindexbuffer[y];
			if(boneID<0)
				continue;

			object3d* bone=boneList[boneID];
			float weight=weightbuffer[y];

			vector3f transformedVertex(inverseBoneTransformationList[boneID] * vertex);
			//vector3f deformVertex(*bone->getWorldMatrix() * transformedVertex);
			vector3f deformVertex((rootNodeInverse * *bone->getWorldMatrix()) * transformedVertex);
			finalVertex.x+=deformVertex.x*weight;
			finalVertex.y+=deformVertex.y*weight;
			finalVertex.z+=deformVertex.z*weight;
		}

		boneindexbuffer+=nInfluenceBones;
		weightbuffer+=nInfluenceBones;

		tmp-=nInfluenceBones;
		assert(tmp>=0);

		//finalVertex=finalVertex-this->getParent()->getWorldMatrix()->getPosition();
		vertexBuffer[x*3+0]=finalVertex.x;
		vertexBuffer[x*3+1]=finalVertex.y;
		vertexBuffer[x*3+2]=finalVertex.z;
	}
}

void gxSkinnedMesh::render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/)
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

#if defined (USE_ProgrammablePipeLine)
	if (renderer->getRenderPassType() == gxRenderer::RENDER_SHADOWMAP)
	{
		renderForShadowMap(renderer);
	}
	else
	{
		renderWithHWShader(renderer, light);
	}
#else
	renderNormal(renderer);
#endif

	object3d::render(renderer, light, renderFlag);
}

int* gxSkinnedMesh::allocateBoneInfluenceCountBuffer(int nTris)
{
	GX_DELETE_ARY(boneInfluenceCountBuffer);
	boneInfluenceCountBuffer = new int[nTris*3];
	return boneInfluenceCountBuffer;
}

int* gxSkinnedMesh::allocateBoneIndexBuffer(int nCount)
{
	//boneInfluencePerVertex=nBoneInfluencePerVertex;
	noOfBoneIndexBuffer=nCount;
	GX_DELETE_ARY(boneIndexBuffer);
	boneIndexBuffer = new int[nCount];
	for(int x=0;x<nCount;x++)
		boneIndexBuffer[x]=-1;
	return boneIndexBuffer;
}

float* gxSkinnedMesh::allocateWeightBuffer(int nCount)
{
	//boneInfluencePerVertex=nBoneInfluencePerVertex;
	noOfBoneIndexBuffer=nCount;
	GX_DELETE_ARY(weightBuffer);
	weightBuffer = new float[nCount];
	memset(weightBuffer, 0, sizeof(float)*nCount);
	return weightBuffer;
}

void gxSkinnedMesh::allocateBoneList(int nBones)
{
	noOfBones=nBones;
	GX_DELETE_ARY(boneList);
	GX_DELETE_ARY(inverseBoneTransformationList);
	boneList = new object3d*[nBones];
	inverseBoneTransformationList = new matrix4x4f[nBones];
	boneOffsetList = new matrix4x4f[nBones];
}

void gxSkinnedMesh::populateBoneList(object3d* bone, int index)
{
	if(privateIterator>=noOfBones)
	{
		index=index;
		return;
	}

	boneList[privateIterator]=bone;
	matrix4x4f bone_backup(*bone);
	if(bone!=this /*&& bone!=this->getParent()*/)
	{
		//boneOffsetList[privateIterator]=*bone->getWorldMatrix();
		*(matrix4x4f*)bone = *bone * this->getWorldMatrix()->getInverse();
	}
	inverseBoneTransformationList[privateIterator]=bone->getWorldMatrix()->getInverse();
	
//	//reassign the original value to bone
//    if(bone!=this)
//        *(matrix4x4f*)bone=bone_backup;

	privateIterator++;

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=bone->getChildList()->getHead();
    while(node)
    {
		object3d* childobj=node->getData();
		populateBoneList(childobj, index);
        node=node->getNext();
	}
#else
	std::vector<object3d*>* childList = bone->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		populateBoneList(childobj, index);
	}
#endif
}

float* gxSkinnedMesh::allocateAndCopyVertexCopyBuffer()
{
	GX_DELETE_ARY(vertexCopyBuffer);
	vertexCopyBuffer = new float[noOfTrianglesForInternalUse*3*3];
	memcpy(vertexCopyBuffer, vertexBuffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
	return vertexCopyBuffer;
}

void gxSkinnedMesh::writeData(gxFile& file)
{
	gxMesh::writeData(file);

	//write skin data
	if(boneInfluenceCountBuffer)
	{
		file.Write(true);
		file.Write(noOfTrianglesForInternalUse);
		file.WriteBuffer((unsigned char*)boneInfluenceCountBuffer, sizeof(int)*noOfTrianglesForInternalUse*3);
	}
	else
	{
		file.Write(false);
	}

	if(boneIndexBuffer)
	{
		file.Write(true);
		file.Write(noOfBoneIndexBuffer);
		file.WriteBuffer((unsigned char*)boneIndexBuffer, sizeof(int)*noOfBoneIndexBuffer);
	}
	else
	{
		file.Write(false);
	}

	if(weightBuffer)
	{
		file.Write(true);
		file.Write(noOfBoneIndexBuffer);
		file.WriteBuffer((unsigned char*)weightBuffer, sizeof(float)*noOfBoneIndexBuffer);
	}
	else
	{
		file.Write(false);
	}

	file.Write(noOfBones);
	//
}

void gxSkinnedMesh::readData(gxFile& file)
{
    gxMesh::readData(file);
    
	//read skin data
	bool bBoneInfluenceCountBuffer=false;
	file.Read(bBoneInfluenceCountBuffer);
	if(bBoneInfluenceCountBuffer)
	{
		int nBoneInfluenceCountBuffer=0;
		file.Read(nBoneInfluenceCountBuffer);
		int* buffer=allocateBoneInfluenceCountBuffer(nBoneInfluenceCountBuffer);
		file.ReadBuffer((unsigned char*)buffer, sizeof(int)*nBoneInfluenceCountBuffer*3);
	}

	bool bBoneIndexBuffer=false;
	file.Read(bBoneIndexBuffer);
	if(bBoneIndexBuffer)
	{
		file.Read(noOfBoneIndexBuffer);
		int* buffer=allocateBoneIndexBuffer(noOfBoneIndexBuffer);
		file.ReadBuffer((unsigned char*)buffer, sizeof(int)*noOfBoneIndexBuffer);
	}

	bool bWeightBuffer=false;
	file.Read(bWeightBuffer);
	if(bWeightBuffer)
	{
		file.Read(noOfBoneIndexBuffer);
		float* buffer=allocateWeightBuffer(noOfBoneIndexBuffer);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfBoneIndexBuffer);
	}
	file.Read(noOfBones);
	allocateBoneList(noOfBones);
	allocateAndCopyVertexCopyBuffer();
	//
}
