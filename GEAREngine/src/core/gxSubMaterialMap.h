#ifndef GXSUBMATERIALMAP_H
#define GXSUBMATERIALMAP_H

#include "gxTexture.h"
#include "vector2.h"

class gxSubMaterialMap
{
public:
	enum ESUBMAT_MAP
	{
		EMMAP_NONE	= -1,
		EMMAP_1		= 0,
		EMMAP_2		= 1,
		EMMAP_3		= 2,
		EMMAP_4		= 3,
		EMMAP_5		= 4,
		EMMAP_6		= 5,
		EMMAP_7		= 6,
		EMMAP_8		= 7,
		EMMAP_9		= 8,
		EMMAP_10	= 9,
		EMMAP_11	= 10,
		EMMAP_MAX
	};

	gxSubMaterialMap()
	{
		m_pTextureName=NULL;
	}

	~gxSubMaterialMap()
	{
		GX_DELETE_ARY(m_pTextureName);
	}

	const vector2f &getUV()			{	return m_cUV;			}
	const char* getTextureName()	{	return m_pTextureName;	}
	ESUBMAT_MAP getType()			{	return m_eType;			}
	gxTexture* getTexture()			{	return &m_cTexture;		}

private:
	vector2f m_cUV;
	char* m_pTextureName;
	ESUBMAT_MAP m_eType;
	gxTexture	m_cTexture;
};

#endif