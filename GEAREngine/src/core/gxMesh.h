#pragma once

#include "object3d.h"
#include "gxTriInfo.h"
#include "gxMaterial.h"
#include "../hwShader/gxHWShader.h"

class gxUV
{
public:
	gxUV()
	{
		material=NULL;
		textureCoordArray=NULL;
        vboID=0;
	}

	~gxUV()
	{
		if(vboID)
			deleteVBOTexCoord();
		material=NULL;
		GX_DELETE_ARY(textureCoordArray);
	}

	void buildVBOTexCoord(int nTris)
	{
		if(!textureCoordArray) return;
        
		glGenBuffers(1, &vboID);					// Get A Valid Name
		glBindBuffer(GL_ARRAY_BUFFER, vboID);	// Bind The Buffer
		glBufferData(GL_ARRAY_BUFFER, nTris*3*2*sizeof(float), textureCoordArray, GL_STATIC_DRAW);
	}

	void deleteVBOTexCoord()
	{
		if(vboID)
			glDeleteBuffers(1, &vboID);
		vboID=0;
	}

	gxMaterial*     material;		//must not delete this pointer
	float*          textureCoordArray;
    unsigned int    vboID;
};


class DECLSPEC gxMesh : public object3d
{
protected:
	gxMesh(int ID);
    
public:
	~gxMesh();

    static gxMesh* create();

	float* getVertexBuffer()	{	return vertexBuffer;	}
	float* getColorBuffer()		{	return colorBuffer;	}
	float* getNormalBuffer()	{	return normalBuffer;	}
	float* getTangentBuffer()	{	return tangentBuffer;	}

	//vbo
    bool isVBO()    {   return is_VBO;  }
    void buildVBO();
	void deleteVBO();
    
    unsigned int getVBOVertexID()   {   return vboVertexID;   }
    unsigned int getVBOColorID()    {   return vboColorID;    }
    unsigned int getVBONormalID()   {   return vboNormalID;   }
	unsigned int getVBOTangentID()  {   return vboTangentID;    }

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

		uvChannelCount=nChannel;
		uvChannel = new gxUV[nChannel];
		for(int x=0;x<nChannel;x++)
		{
			uvChannel[x].textureCoordArray = new float[nTris*3*2];
		}

		return uvChannel;
	}

	gxTriInfo* allocateTriInfoArray(int nCount)		{	triangleInfoArrayCount=nCount; triangleInfoArray=new gxTriInfo[nCount]; return triangleInfoArray; }
	bool createTBN_Data();

	int getNoOfTriInfo()							{	return triangleInfoArrayCount;				}
	gxTriInfo* getTriInfo(int index)				{	return &triangleInfoArray[index];	}

	int getVerticesCount();
	int getTriangleCount()		{    return noOfTrianglesForInternalUse;	}

protected:
    virtual void writeData(gxFile& file);
    virtual void readData(gxFile& file);

	bool applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, gxSubMap* submap, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz, gxHWShader* shader, const char* texCoordAttribName);	
	void disableTextureOperations(int stage, gxHWShader* shader, const char* texCoordAttribName);

	int triangleInfoArrayCount;
	gxTriInfo* triangleInfoArray;

	float* vertexBuffer;
	float* colorBuffer;
	float* normalBuffer;
	float* tangentBuffer;

	//for VBO
	bool  is_VBO;
	unsigned int vboVertexID;
	unsigned int vboColorID;
	unsigned int vboNormalID;
	unsigned int vboTangentID;


	int uvChannelCount;
	gxUV* uvChannel;
	int noOfTrianglesForInternalUse;
    
private:
    gxMesh();
};