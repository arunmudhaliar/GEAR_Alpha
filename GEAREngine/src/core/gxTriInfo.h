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

private:
	gxMaterial* m_pMaterialPtr;	//must not delete this pointer

	int m_nTris;
	int* m_pTriList;
};

#endif