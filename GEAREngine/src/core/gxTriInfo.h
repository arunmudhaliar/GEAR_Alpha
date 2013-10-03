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
		m_pTriList=NULL;
		m_pMaterialPtr=NULL;
		m_iMaterialCRC = 0;
	}

	~gxTriInfo()
	{
		m_nVertices=0;
		GX_DELETE_ARY(m_pTriList);
	}

	int getVerticesCount()	{	return m_nVertices;		}
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
			file.Write(m_pMaterialPtr->getFileCRC());
		else
			file.Write((int)0);
	}

	void read(gxFile& file)
	{
		file.Read(m_nVertices);
		if(m_nVertices)
		{
			int* buffer = allocateMemory(m_nVertices);
			file.ReadBuffer((unsigned char*)buffer, sizeof(int)*m_nVertices*3);
		}
		file.Read(m_iMaterialCRC);
	}

	int getMaterialCRCFromFileReadInfo()	{	return m_iMaterialCRC;	}

private:
	gxMaterial* m_pMaterialPtr;	//must not delete this pointer

	int m_nVertices;
	int* m_pTriList;
	int m_iMaterialCRC;
};

#endif