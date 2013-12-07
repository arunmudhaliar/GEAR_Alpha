#include "gxSkinnedMesh.h"

gxSkinnedMesh::gxSkinnedMesh():
gxMesh(101)
{
	m_pszBoneIndexBuffer=NULL;
	m_pszWeightBuffer=NULL;
	m_nBoneInfluencePerVertex=4;
	m_pszBoneList=NULL;
	m_nBones=0;
	m_pszVertexCopyBuffer=NULL;
	m_pszInvBoneTMList=NULL;
	m_pszBoneOffsetList=NULL;
}

gxSkinnedMesh::~gxSkinnedMesh()
{
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

	for(int x=0;x<m_nTris_For_Internal_Use*3;x++)
	{
		vector3f vertex(&m_pszVertexCopyBuffer[x*3]);
		vector3f finalVertex;
		for(int y=0;y<m_nBoneInfluencePerVertex;y++)
		{
			int boneID=m_pszBoneIndexBuffer[x*m_nBoneInfluencePerVertex+y];
			if(boneID<0)
				continue;

			object3d* bone=m_pszBoneList[boneID];
			float weight=m_pszWeightBuffer[x*m_nBoneInfluencePerVertex+y];

			vector3f transformedVertex(m_pszInvBoneTMList[boneID] * vertex);
			vector3f deformVertex(*bone->getWorldMatrix() * transformedVertex);
			finalVertex.x+=deformVertex.x*weight;
			finalVertex.y+=deformVertex.y*weight;
			finalVertex.z+=deformVertex.z*weight;
		}

		finalVertex=finalVertex-this->getParent()->getWorldMatrix()->getPosition();
		m_pszVertexBuffer[x*3+0]=finalVertex.x;
		m_pszVertexBuffer[x*3+1]=finalVertex.y;
		m_pszVertexBuffer[x*3+2]=finalVertex.z;
	}
}

void gxSkinnedMesh::render(gxRenderer* renderer, object3d* light)
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

	object3d::render(renderer, light);
}

int* gxSkinnedMesh::allocateBoneIndexBuffer(int nTris, int nBoneInfluencePerVertex)
{
	m_nBoneInfluencePerVertex=nBoneInfluencePerVertex;
	GX_DELETE_ARY(m_pszBoneIndexBuffer);
	m_pszBoneIndexBuffer = new int[nTris*3*nBoneInfluencePerVertex];
	for(int x=0;x<nTris*3*nBoneInfluencePerVertex;x++)
		m_pszBoneIndexBuffer[x]=-1;
	return m_pszBoneIndexBuffer;
}

float* gxSkinnedMesh::allocateWeightBuffer(int nTris, int nBoneInfluencePerVertex)
{
	m_nBoneInfluencePerVertex=nBoneInfluencePerVertex;
	GX_DELETE_ARY(m_pszWeightBuffer);
	m_pszWeightBuffer = new float[nTris*3*nBoneInfluencePerVertex];
	memset(m_pszWeightBuffer, 0, sizeof(int)*nTris*3*nBoneInfluencePerVertex);
	return m_pszWeightBuffer;
}

void gxSkinnedMesh::allocateBoneList(int nBones)
{
	m_nBones=nBones;
	GX_DELETE_ARY(m_pszBoneList);
	GX_DELETE_ARY(m_pszInvBoneTMList);
	m_pszBoneList = new object3d*[nBones];
	m_pszInvBoneTMList = new matrix4x4f[nBones];
	m_pszBoneOffsetList = new vector3f[nBones];
}

void gxSkinnedMesh::populateBoneList(object3d* bone, int index)
{
	if(m_iPrivateIterator>=m_nBones)
	{
		index=index;
		return;
	}

	m_pszBoneList[m_iPrivateIterator]=bone;
	if(bone!=this /*&& bone!=this->getParent()*/)
	{
		//*(matrix4x4f*)bone = *bone * this->getWorldMatrix()->getInverse();
	}
	m_pszInvBoneTMList[m_iPrivateIterator]=bone->getWorldMatrix()->getInverse();
	m_pszBoneOffsetList[m_iPrivateIterator]=bone->getWorldMatrix()->getPosition();
	m_iPrivateIterator++;

	std::vector<object3d*>* childList = bone->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		populateBoneList(childobj, index);
	}
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
	file.Write(m_nBoneInfluencePerVertex);
	if(m_pszBoneIndexBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszBoneIndexBuffer, sizeof(int)*m_nTris_For_Internal_Use*3*m_nBoneInfluencePerVertex);
	}
	else
	{
		file.Write(false);
	}

	if(m_pszWeightBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszWeightBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*m_nBoneInfluencePerVertex);
	}
	else
	{
		file.Write(false);
	}

	file.Write(m_nBones);
	//

	file.Write((int)m_cChilds.size());
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->write(file);
	}
}

void gxSkinnedMesh::read(gxFile& file)
{
	gxMesh::read(file);

	//read skin data
	file.Read(m_nBoneInfluencePerVertex);
	bool bBoneIndexBuffer=false;
	file.Read(bBoneIndexBuffer);
	if(bBoneIndexBuffer)
	{
		int* buffer=allocateBoneIndexBuffer(m_nTris_For_Internal_Use, m_nBoneInfluencePerVertex);
		file.ReadBuffer((unsigned char*)buffer, sizeof(int)*m_nTris_For_Internal_Use*3*m_nBoneInfluencePerVertex);
	}

	bool bWeightBuffer=false;
	file.Read(bWeightBuffer);
	if(bWeightBuffer)
	{
		float* buffer=allocateWeightBuffer(m_nTris_For_Internal_Use, m_nBoneInfluencePerVertex);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nTris_For_Internal_Use*3*m_nBoneInfluencePerVertex);
	}
	file.Read(m_nBones);
	allocateBoneList(m_nBones);
	allocateAndCopyVertexCopyBuffer();
	//
}
