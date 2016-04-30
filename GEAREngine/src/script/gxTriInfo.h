#pragma once

#include "../core/basicIncludes.h"
#include "../core/gxMaterial.h"

class gxTriInfo
{
public:
	gxTriInfo()
	{
		vertexCount=0;
		triangleCount=0;
		triangleList=NULL;
		material=NULL;
		materialCRC = 0;
		triangleListVBOID=0;
	}

	~gxTriInfo()
	{
		deleteVBOTriList();
		GX_DELETE_ARY(triangleList);
        REF_RELEASE(this->material);
	}

	int getVerticesCount()	{	return vertexCount;		}
	int getTriangleCount()	{   return triangleCount;	}

	int* getTriList()	{	return triangleList;	}

	int* allocateMemory(int nVertices)
	{
		vertexCount = nVertices;
		triangleList = new int[vertexCount*3];
		return triangleList;
	}

	gxMaterial* getMaterial()				{	return material;            }
	void setMaterial(gxMaterial* material)
    {
        if(this->material==material)
            return;
        REF_RELEASE(this->material);
        this->material = material;
        REF_RETAIN(this->material);
    }

	void write(gxFile& file)
	{
		file.Write(vertexCount);
		if(vertexCount)
			file.WriteBuffer((unsigned char*)triangleList, sizeof(int)*vertexCount*3);
		if(material)
			file.Write(material->getAssetFileCRC());
		else
			file.Write((int)0);
	}

	void read(gxFile& file)
	{
		file.Read(vertexCount);
		triangleCount=vertexCount/3;
		if(vertexCount)
		{
			int* buffer = allocateMemory(vertexCount);
			file.ReadBuffer((unsigned char*)buffer, sizeof(int)*vertexCount*3);
		}
		file.Read(materialCRC);
	}

	int getMaterialCRCFromFileReadInfo()	{	return materialCRC;	}
	unsigned int getVBOTriListID()			{	return triangleListVBOID;	}

	void deleteVBOTriList()
	{
		if(triangleListVBOID!=0)
		{
			glDeleteBuffers(1, &triangleListVBOID);
			triangleListVBOID=0;
		}
	}

	void buildVBOTriList()
	{
		glGenBuffers(1, &triangleListVBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleListVBOID);  
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(int), (char*)triangleList, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

private:
	gxMaterial* material;	//must not delete this pointer

	int vertexCount;
	int triangleCount;
	int* triangleList;
	int materialCRC;
	unsigned int triangleListVBOID;
};