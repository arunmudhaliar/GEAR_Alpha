#include "gxSkinnedMesh.h"

gxSkinnedMesh::gxSkinnedMesh():
gxMesh(OBJECT3D_SKINNED_MESH)
{
	m_pszBoneInfluenceCountBuffer=NULL;
	m_pszBoneIndexBuffer=NULL;
	m_pszWeightBuffer=NULL;
	m_nBoneInfluencePerVertex=4;
	m_pszBoneList=NULL;
	m_nBones=0;
	m_nBoneIndexBuffer=0;
	m_pszVertexCopyBuffer=NULL;
	m_pszInvBoneTMList=NULL;
	m_pszBoneOffsetList=NULL;
	m_pRootNodePtr=NULL;
	reSetBaseFlag(eObject3dBaseFlag_Static);
}

gxSkinnedMesh::~gxSkinnedMesh()
{
	GX_DELETE_ARY(m_pszBoneInfluenceCountBuffer);
	GX_DELETE_ARY(m_pszBoneIndexBuffer);
	GX_DELETE_ARY(m_pszWeightBuffer);

	GX_DELETE_ARY(m_pszBoneList);
	GX_DELETE_ARY(m_pszVertexCopyBuffer);
	GX_DELETE_ARY(m_pszInvBoneTMList);
	GX_DELETE_ARY(m_pszBoneOffsetList);
}

void gxSkinnedMesh::update(float dt)
{
	gxMesh::update(dt);

	int* boneindexbuffer=m_pszBoneIndexBuffer;
	float* weightbuffer=m_pszWeightBuffer;
	int tmp=m_nBoneIndexBuffer;
	matrix4x4f skinTM(*this);
	//skinTM.setPosition(0, 0, 0);
	matrix4x4f rootNodeInverse(getWorldMatrix()->getInverse());	//TODO: try optimize this by transforming the skin in transformchanged()

	for(int x=0;x<m_nTris_For_Internal_Use*3;x++)
	{
		vector3f vertex(&m_pszVertexCopyBuffer[x*3]);
		vector3f finalVertex;
		int nInfluenceBones=m_pszBoneInfluenceCountBuffer[x];
		for(int y=0;y<nInfluenceBones;y++)
		{
			int boneID=boneindexbuffer[y];
			if(boneID<0)
				continue;

			object3d* bone=m_pszBoneList[boneID];
			float weight=weightbuffer[y];

			vector3f transformedVertex(m_pszInvBoneTMList[boneID] * vertex);
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
		m_pszVertexBuffer[x*3+0]=finalVertex.x;
		m_pszVertexBuffer[x*3+1]=finalVertex.y;
		m_pszVertexBuffer[x*3+2]=finalVertex.z;
	}
}

void gxSkinnedMesh::render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/)
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

#if defined (USE_ProgrammablePipeLine)
	//if(renderer->getRenderPassType()==gxRenderer::RENDER_LIGHTING_ONLY)
	//	renderWithLight(renderer, light);
	//else if(renderer->getRenderPassType()==gxRenderer::RENDER_NORMAL)
		renderWithHWShader(renderer, light);
#else
	renderNormal(renderer);
#endif

	object3d::render(renderer, light, renderFlag);
}

int* gxSkinnedMesh::allocateBoneInfluenceCountBuffer(int nTris)
{
	GX_DELETE_ARY(m_pszBoneInfluenceCountBuffer);
	m_pszBoneInfluenceCountBuffer = new int[nTris*3];
	return m_pszBoneInfluenceCountBuffer;
}

int* gxSkinnedMesh::allocateBoneIndexBuffer(int nCount)
{
	//m_nBoneInfluencePerVertex=nBoneInfluencePerVertex;
	m_nBoneIndexBuffer=nCount;
	GX_DELETE_ARY(m_pszBoneIndexBuffer);
	m_pszBoneIndexBuffer = new int[nCount];
	for(int x=0;x<nCount;x++)
		m_pszBoneIndexBuffer[x]=-1;
	return m_pszBoneIndexBuffer;
}

float* gxSkinnedMesh::allocateWeightBuffer(int nCount)
{
	//m_nBoneInfluencePerVertex=nBoneInfluencePerVertex;
	m_nBoneIndexBuffer=nCount;
	GX_DELETE_ARY(m_pszWeightBuffer);
	m_pszWeightBuffer = new float[nCount];
	memset(m_pszWeightBuffer, 0, sizeof(float)*nCount);
	return m_pszWeightBuffer;
}

void gxSkinnedMesh::allocateBoneList(int nBones)
{
	m_nBones=nBones;
	GX_DELETE_ARY(m_pszBoneList);
	GX_DELETE_ARY(m_pszInvBoneTMList);
	m_pszBoneList = new object3d*[nBones];
	m_pszInvBoneTMList = new matrix4x4f[nBones];
	m_pszBoneOffsetList = new matrix4x4f[nBones];
}

void gxSkinnedMesh::populateBoneList(object3d* bone, int index)
{
	if(m_iPrivateIterator>=m_nBones)
	{
		index=index;
		return;
	}

	m_pszBoneList[m_iPrivateIterator]=bone;
	matrix4x4f bone_backup(*bone);
	if(bone!=this /*&& bone!=this->getParent()*/)
	{
		m_pszBoneOffsetList[m_iPrivateIterator]=*bone->getWorldMatrix();
		*(matrix4x4f*)bone = *bone * this->getWorldMatrix()->getInverse();
	}
	m_pszInvBoneTMList[m_iPrivateIterator]=bone->getWorldMatrix()->getInverse();
	
	//reassign the original value to bone
	*(matrix4x4f*)bone=bone_backup;

	m_iPrivateIterator++;

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
	GX_DELETE_ARY(m_pszVertexCopyBuffer);
	m_pszVertexCopyBuffer = new float[m_nTris_For_Internal_Use*3*3];
	memcpy(m_pszVertexCopyBuffer, m_pszVertexBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	return m_pszVertexCopyBuffer;
}

void gxSkinnedMesh::write(gxFile& file)
{
	gxMesh::writeMeshData(file);

	//write skin data
	if(m_pszBoneInfluenceCountBuffer)
	{
		file.Write(true);
		file.Write(m_nTris_For_Internal_Use);
		file.WriteBuffer((unsigned char*)m_pszBoneInfluenceCountBuffer, sizeof(int)*m_nTris_For_Internal_Use*3);
	}
	else
	{
		file.Write(false);
	}

	if(m_pszBoneIndexBuffer)
	{
		file.Write(true);
		file.Write(m_nBoneIndexBuffer);
		file.WriteBuffer((unsigned char*)m_pszBoneIndexBuffer, sizeof(int)*m_nBoneIndexBuffer);
	}
	else
	{
		file.Write(false);
	}

	if(m_pszWeightBuffer)
	{
		file.Write(true);
		file.Write(m_nBoneIndexBuffer);
		file.WriteBuffer((unsigned char*)m_pszWeightBuffer, sizeof(float)*m_nBoneIndexBuffer);
	}
	else
	{
		file.Write(false);
	}

	file.Write(m_nBones);
	//

	file.Write((int)m_cChilds.size());

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=m_cChilds.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->write(file);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->write(file);
	}
#endif
}

void gxSkinnedMesh::read(gxFile& file)
{
	gxMesh::read(file);

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
		file.Read(m_nBoneIndexBuffer);
		int* buffer=allocateBoneIndexBuffer(m_nBoneIndexBuffer);
		file.ReadBuffer((unsigned char*)buffer, sizeof(int)*m_nBoneIndexBuffer);
	}

	bool bWeightBuffer=false;
	file.Read(bWeightBuffer);
	if(bWeightBuffer)
	{
		file.Read(m_nBoneIndexBuffer);
		float* buffer=allocateWeightBuffer(m_nBoneIndexBuffer);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nBoneIndexBuffer);
	}
	file.Read(m_nBones);
	allocateBoneList(m_nBones);
	allocateAndCopyVertexCopyBuffer();
	//
}
