#ifndef GXTRIINFO_H
#define GXTRIINFO_H

#include "basicIncludes.h"
#include "gxMaterial.h"

class gxTriInfo
{
public:
	gxTriInfo()
	{
		m_nTris=0;
		m_pTriList=NULL;
		m_pMaterialPtr=NULL;
		m_iMaterialCRC = 0;
	}

	~gxTriInfo()
	{
		m_nTris=0;
		GX_DELETE_ARY(m_pTriList);
	}

	int getNoOfTris()	{	return m_nTris;		}
	int* getTriList()	{	return m_pTriList;	}

	int* allocateMemory(int nTris)
	{
		m_nTris=nTris;
		m_pTriList = new int[m_nTris*3];
		return m_pTriList;
	}

	gxMaterial* getMaterial()				{	return m_pMaterialPtr;		}
	void setMaterial(gxMaterial* material)	{	m_pMaterialPtr	=material;	}

	void write(gxFile& file)
	{
		file.Write(m_nTris);
		if(m_nTris)
			file.WriteBuffer((unsigned char*)m_pTriList, sizeof(int)*m_nTris*3);
		if(m_pMaterialPtr)
			file.Write(m_pMaterialPtr->getFileCRC());
		else
			file.Write((int)0);
	}

	void read(gxFile& file)
	{
		file.Read(m_nTris);
		if(m_nTris)
		{
			int* buffer = allocateMemory(m_nTris);
			file.ReadBuffer((unsigned char*)buffer, sizeof(int)*m_nTris*3);
		}
		file.Read(m_iMaterialCRC);
	}

	int getMaterialCRCFromFileReadInfo()	{	return m_iMaterialCRC;	}

private:
	gxMaterial* m_pMaterialPtr;	//must not delete this pointer

	int m_nTris;
	int* m_pTriList;
	int m_iMaterialCRC;
};

#endif