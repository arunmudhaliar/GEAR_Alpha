#ifndef GXTRIINFO_H
#define GXTRIINFO_H

#include "basicIncludes.h"
#include "gxMaterial.h"

class gxTriInfo
{
public:
	gxTriInfo()
	{
		m_nVertices=0;
		m_nTriangles=0;
		m_pTriList=NULL;
		m_pMaterialPtr=NULL;
		m_iMaterialCRC = 0;
		m_uVBO_trilstID=0;
	}

	~gxTriInfo()
	{
		deleteVBOTriList();
		GX_DELETE_ARY(m_pTriList);
	}

	int getVerticesCount()	{	return m_nVertices;		}
	int getTriangleCount()	{   return m_nTriangles;	}

	int* getTriList()	{	return m_pTriList;	}

	int* allocateMemory(int nVertices)
	{
		m_nVertices=nVertices;
		m_pTriList = new int[m_nVertices*3];
		return m_pTriList;
	}

	gxMaterial* getMaterial()				{	return m_pMaterialPtr;		}
	void setMaterial(gxMaterial* material)	{	m_pMaterialPtr	=material;	}

	void write(gxFile& file)
	{
		file.Write(m_nVertices);
		if(m_nVertices)
			file.WriteBuffer((unsigned char*)m_pTriList, sizeof(int)*m_nVertices*3);
		if(m_pMaterialPtr)
			file.Write(m_pMaterialPtr->getAssetFileCRC());
		else
			file.Write((int)0);
	}

	void read(gxFile& file)
	{
		file.Read(m_nVertices);
		m_nTriangles=m_nVertices/3;
		if(m_nVertices)
		{
			int* buffer = allocateMemory(m_nVertices);
			file.ReadBuffer((unsigned char*)buffer, sizeof(int)*m_nVertices*3);
		}
		file.Read(m_iMaterialCRC);
	}

	int getMaterialCRCFromFileReadInfo()	{	return m_iMaterialCRC;	}
	unsigned int getVBOTriListID()			{	return m_uVBO_trilstID;	}

	void deleteVBOTriList()
	{
		if(m_uVBO_trilstID!=0)
		{
			glDeleteBuffers(1, &m_uVBO_trilstID);
			m_uVBO_trilstID=0;
		}
	}

	void buildVBOTriList()
	{
		glGenBuffers(1, &m_uVBO_trilstID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uVBO_trilstID);  
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nVertices * sizeof(int), (char*)m_pTriList, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

private:
	gxMaterial* m_pMaterialPtr;	//must not delete this pointer

	int m_nVertices;
	int m_nTriangles;
	int* m_pTriList;
	int m_iMaterialCRC;
	unsigned int m_uVBO_trilstID;
};

#endif