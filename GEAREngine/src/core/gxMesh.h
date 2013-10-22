#ifndef GXMESH_H
#define GXMESH_H

#include "object3d.h"
#include "gxTriInfo.h"
#include "gxMaterial.h"
#include "../hwShader/gxHWShader.h"

class gxUV
{
public:
	gxUV()
	{
		m_pMaterialPtr=NULL;
		m_pszfGLTexCoordList=NULL;
		m_iMatIDFromFile=-1;
        m_cVBO_texID=0;
	}

	~gxUV()
	{
		m_pMaterialPtr=NULL;
		GX_DELETE_ARY(m_pszfGLTexCoordList);
	}

	int			m_iMatIDFromFile;
	gxMaterial* m_pMaterialPtr;		//must not delete this pointer
	float*		m_pszfGLTexCoordList;
    unsigned int m_cVBO_texID;
};


class DllExport gxMesh : public object3d
{
protected:
	gxMesh(int ID);
public:
	gxMesh();
	~gxMesh();

	float* getVertexBuffer()	{	return m_pszVertexBuffer;	}
	float* getColorBuffer()		{	return m_pszColorBuffer;	}
	float* getNormalBuffer()	{	return m_pszNormalBuffer;	}

	virtual void update(float dt);
	virtual void render(gxRenderer* renderer);
	void renderNormal(gxRenderer* renderer);
	void renderWithHWShader(gxRenderer* renderer);

	float* allocateVertexBuffer(int nTris);
	float* allocateColorBuffer(int nTris);
	float* allocateNormalBuffer(int nTris);

	gxUV* allocateUVChannels(int nChannel, int nTris)
	{
		if(nChannel==0) return NULL;

		m_nUVChannels=nChannel;
		m_pszUVChannels = new gxUV[nChannel];
		for(int x=0;x<nChannel;x++)
		{
			m_pszUVChannels[x].m_pszfGLTexCoordList = new float[nTris*3*2];
		}

		return m_pszUVChannels;
	}

	gxTriInfo* allocateTriInfoArray(int nCount)		{	m_nTriInfoArray=nCount; m_pszTriInfoArray=new gxTriInfo[nCount]; return m_pszTriInfoArray; }


	int getNoOfTriInfo()							{	return m_nTriInfoArray;				}
	gxTriInfo* getTriInfo(int index)				{	return &m_pszTriInfoArray[index];	}

	int getVerticesCount();

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);
	void writeMeshData(gxFile& file);
	virtual void transformationChangedf();

protected:

	bool applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz);
	bool applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz, gxHWShader* shader, const char* texCoordAttribName);
	
	void disableTextureOperations(int nMultiTextureUsed, gxHWShader* shader, const char* texCoordAttribName);

	int m_nTriInfoArray;
	gxTriInfo* m_pszTriInfoArray;

	float* m_pszVertexBuffer;
	float* m_pszColorBuffer;
	float* m_pszNormalBuffer;
	int m_nUVChannels;
	gxUV* m_pszUVChannels;
	int m_nTris_For_Internal_Use;
};

#endif