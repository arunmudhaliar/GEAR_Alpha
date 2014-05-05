#include "Sprite2Dx.h"

Sprite2Dx::Sprite2Dx():
Sprite()
{
}

Sprite2Dx::Sprite2Dx(Sprite2Dx& sprite2dx):
Sprite(sprite2dx)
{
}

Sprite2Dx::~Sprite2Dx()
{
	
}

void Sprite2Dx::draw(gxHWShader* shader)
{
	glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 2, GL_FLOAT, GL_FALSE, 0, m_cszVertLst);
    glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));

    glVertexAttribPointer(shader->getAttribLoc("a_uv_coord0_v2"), 2, GL_FLOAT, GL_FALSE, 0, m_cszTexCoord);
	glEnableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2"));
	
	//arun:texissue glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_cTexture.getTextureID());
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (isRenderFlag(RENDER_FORCE_NO_MODULATION)&&m_fAlpha==1.0f)?GL_REPLACE:GL_MODULATE);

	if(m_cTexture.getTextureType()==geTexture::TEX_ALPHA && !isRenderFlag(RENDER_FORCE_NO_ALPHA))
	{
 		glEnable(GL_BLEND);
		if(m_eBlendFlag==BLEND_MULTIPLY)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else if(m_eBlendFlag==BLEND_ADDITIVE)
		{
			glBlendFunc(GL_ONE, GL_ONE);
		}
	}

	shader->sendUniform1i("u_diffuse_texture", 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (m_cTexture.getTextureType() != geTexture::TEX_UNDEFINED)
	{	
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2"));
		//glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		//arun:texissue glDisable(GL_TEXTURE_2D);
	}
	
	glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
}

void Sprite2Dx::draw(/*const matrix4x4f& parentTM, */geVector2f* pos)
{	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, m_cszVertLst);
    
	if (m_cTexture.getTextureType() != geTexture::TEX_UNDEFINED)
	{
		glActiveTexture(GL_TEXTURE0);
		//glClientActiveTexture(GL_TEXTURE0);	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_cTexture.getTextureID());

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (isRenderFlag(RENDER_FORCE_NO_MODULATION)&&m_fAlpha==1.0f)?GL_REPLACE:GL_MODULATE);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, m_cszTexCoord);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		if(m_cTexture.getTextureType()==geTexture::TEX_ALPHA && !isRenderFlag(RENDER_FORCE_NO_ALPHA))
		{
 			glEnable(GL_BLEND);
			if(m_eBlendFlag==BLEND_MULTIPLY)
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			else if(m_eBlendFlag==BLEND_ADDITIVE)
			{
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glBlendFunc(GL_ONE, GL_ONE);
			}
		}
	}

    if(!isRenderFlag(RENDER_FORCE_NO_ALPHA | RENDER_FORCE_NO_BLEND))
    {
        glColor4f(m_fRGB[0], m_fRGB[1], m_fRGB[2], m_fAlpha);
    }
    
	glPushMatrix();
    if(!isRenderFlag(RENDER_FORCE_NO_TRANSFROM))
    {
		if(pos)
			glTranslatef(pos->x, pos->y, 0.0f);
		else
			glTranslatef(m[12], m[13], m[14]);
    }
    
    onSpritePreRender();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    onSpritePostRender();
    glPopMatrix();
	
	if (m_cTexture.getTextureType() != geTexture::TEX_UNDEFINED)
	{	
		glDisable(GL_BLEND);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Sprite2Dx::drawVerextBufferOnly(geVector2f* pos)
{
#if defined (USE_ProgrammablePipeLine_test)
    
#else
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, m_cszVertLst);

    glPushMatrix();
	if(pos)
		glTranslatef(pos->x, pos->y, 0.0f);
	else
		glTranslatef(m[12], m[13], m[14]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();
    
    glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

