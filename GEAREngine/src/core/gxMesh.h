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
		if(m_cVBO_texID)
			deleteVBOTexCoord();
		m_pMaterialPtr=NULL;
		GX_DELETE_ARY(m_pszfGLTexCoordList);
	}

	void buildVBOTexCoord(int nTris)
	{
		if(!m_pszfGLTexCoordList) return;
        
		glGenBuffers(1, &m_cVBO_texID);					// Get A Valid Name
		glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_texID);	// Bind The Buffer
		glBufferData(GL_ARRAY_BUFFER, nTris*3*2*sizeof(float), m_pszfGLTexCoordList, GL_STATIC_DRAW);
	}

	void deleteVBOTexCoord()
	{
		if(m_cVBO_texID)
			glDeleteBuffers(1, &m_cVBO_texID);
		m_cVBO_texID=0;
	}

	int			m_iMatIDFromFile;
	gxMaterial* m_pMaterialPtr;		//must not delete this pointer
	float*		m_pszfGLTexCoordList;
    unsigned int m_cVBO_texID;
};


class DECLSPEC gxMesh : public object3d
{
protected:
	gxMesh(int ID);
public:
	gxMesh();
	~gxMesh();

	float* getVertexBuffer()	{	return m_pszVertexBuffer;	}
	float* getColorBuffer()		{	return m_pszColorBuffer;	}
	float* getNormalBuffer()	{	return m_pszNormalBuffer;	}
	float* getTangentBuffer()	{	return m_pszTangentBuffer;	}

	//vbo
    bool isVBO()    {   return m_bVBO;  }
    void buildVBO();
	void deleteVBO();
    
    unsigned int getVBOVertexID()   {   return m_cVBO_vertID;   }
    unsigned int getVBOColorID()    {   return m_cVBO_clrID;    }
    unsigned int getVBONormalID()   {   return m_cVBO_nrmlID;   }
	unsigned int getVBOTangentID()  {   return m_cVBO_tangentID;    }

	virtual void update(float dt);
	virtual void render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);
	//void renderNormal(gxRenderer* renderer);
	//void renderWithLight(gxRenderer* renderer, object3d* light);
	void renderWithHWShader(gxRenderer* renderer, object3d* light);
	void renderForShadowMap(gxRenderer* renderer);

	float* allocateVertexBuffer(int nTris);
	float* allocateColorBuffer(int nTris);
	float* allocateNormalBuffer(int nTris);
	float* allocateTangentBuffer(int nTris);

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
	bool createTBN_Data();

	int getNoOfTriInfo()							{	return m_nTriInfoArray;				}
	gxTriInfo* getTriInfo(int index)				{	return &m_pszTriInfoArray[index];	}

	int getVerticesCount();
	int getTriangleCount()		{    return m_nTris_For_Internal_Use;	}

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);
	void writeMeshData(gxFile& file);

protected:

	//bool applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz);
	bool applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, gxSubMap* submap, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz, gxHWShader* shader, const char* texCoordAttribName);
	
	void disableTextureOperations(int stage, gxHWShader* shader, const char* texCoordAttribName);

	int m_nTriInfoArray;
	gxTriInfo* m_pszTriInfoArray;

	float* m_pszVertexBuffer;
	float* m_pszColorBuffer;
	float* m_pszNormalBuffer;
	float* m_pszTangentBuffer;

	//for VBO
	bool  m_bVBO;
	unsigned int m_cVBO_vertID;
	unsigned int m_cVBO_clrID;
	unsigned int m_cVBO_nrmlID;
	unsigned int m_cVBO_tangentID;


	int m_nUVChannels;
	gxUV* m_pszUVChannels;
	int m_nTris_For_Internal_Use;
};

#endif