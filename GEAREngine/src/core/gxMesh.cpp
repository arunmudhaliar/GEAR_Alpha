#include "gxMesh.h"

gxMesh::gxMesh():
	object3d(100)
{
	m_nTriInfoArray=0;
	m_pszTriInfoArray=NULL;

	m_pszVertexBuffer=NULL;
	m_pszColorBuffer=NULL;
	m_pszNormalBuffer=NULL;
	m_nUVChannels=0;
	m_pszUVChannels=NULL;
}

gxMesh::~gxMesh()
{
	m_nTriInfoArray=0;
	GX_DELETE_ARY(m_pszTriInfoArray);
	GX_DELETE_ARY(m_pszVertexBuffer);
	GX_DELETE_ARY(m_pszColorBuffer);
	GX_DELETE_ARY(m_pszNormalBuffer);

	GX_DELETE_ARY(m_pszUVChannels);
}

void gxMesh::update(float dt)
{
	object3d::update(dt);
}

void gxMesh::render()
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

	glPushMatrix();
	glMultMatrixf(getWorldMatrix()->getMatrix());

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_pszVertexBuffer);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, m_pszNormalBuffer);

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		gxTriInfo* triInfo=&m_pszTriInfoArray[x];
		if(!triInfo->getTriList()) continue;

		glColor4fv(&triInfo->getMaterial()->getDiffuseClr().x);

		int nTexUsed=0;
		for(int m=0;m<m_nUVChannels;m++)
		{
			gxUV* uv=&m_pszUVChannels[m];
			if(triInfo->getMaterial() && applyStageTexture(nTexUsed, triInfo->getMaterial()->getTexture()->getTextureMatrix(), uv, triInfo->getMaterial()->getTexture(), GL_TEXTURE_ENV_MODE, GL_MODULATE, 2))
				nTexUsed++;
		}
		glDrawElements(GL_TRIANGLES, triInfo->getNoOfTris(), GL_UNSIGNED_INT, triInfo->getTriList());

		disableTextureOperations(nTexUsed);
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPopMatrix();

	object3d::render();
}

bool gxMesh::applyStageTexture(int stage, matrix4x4f* matrix, gxUV* uv, gxTexture* texture, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz)
{
	if(!texture) return false;

	if(texture->getTextureID()==0 || !uv) return false;

	glActiveTexture(GL_TEXTURE0+stage);
	glClientActiveTexture(GL_TEXTURE0+stage);
	glTexCoordPointer(texCoordSz, GL_FLOAT, 0, uv->m_pszfGLTexCoordList);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID() );
	glTexEnvf(GL_TEXTURE_ENV, aTexEnv1, (float)aTexEnv2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(matrix->getMatrix());
	glMatrixMode(GL_MODELVIEW);

	if(texture->getTextureType()==gxTexture::TEX_ALPHA)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	return true;
}

void gxMesh::disableTextureOperations(int nMultiTextureUsed)
{
	//Disabling all texture operations
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	for(int i=0;i<nMultiTextureUsed;i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_TEXTURE_2D);	
	}
}

float* gxMesh::allocateVertexBuffer(int nTris)
{
	GX_DELETE_ARY(m_pszVertexBuffer);
	m_pszVertexBuffer = new float[nTris*3*3];
	return m_pszVertexBuffer;
}

float* gxMesh::allocateColorBuffer(int nTris)
{
	GX_DELETE_ARY(m_pszColorBuffer);
	m_pszColorBuffer = new float[nTris*3*3];
	return m_pszColorBuffer;
}

float* gxMesh::allocateNormalBuffer(int nTris)
{
	GX_DELETE_ARY(m_pszNormalBuffer);
	m_pszNormalBuffer = new float[nTris*3*3];
	return m_pszNormalBuffer;
}

int gxMesh::getTotalNoOfTris()
{
	int nTris=0;
	for(int x=0;x<m_nTriInfoArray;x++)
	{
		nTris+=m_pszTriInfoArray[x].getNoOfTris();
	}

	return nTris;
}