#include "gxMesh.h"
#include "../GEAREngine.h"
#include "Timer.h"

gxMesh::gxMesh(int ID):
	object3d(ID)
{
	m_nTriInfoArray=0;
	m_pszTriInfoArray=NULL;

	m_pszVertexBuffer=NULL;
	m_pszColorBuffer=NULL;
	m_pszNormalBuffer=NULL;
	m_pszTangentBuffer=NULL;
	m_nUVChannels=0;
	m_pszUVChannels=NULL;
	m_nTris_For_Internal_Use=0;

	m_bVBO=0;
    m_cVBO_vertID=0;
	m_cVBO_clrID=0;
	m_cVBO_nrmlID=0;
	m_cVBO_tangentID=0;
	setBaseFlag(eObject3dBaseFlag_Static);
}

gxMesh::gxMesh():
	object3d(OBJECT3D_MESH)
{
	m_nTriInfoArray=0;
	m_pszTriInfoArray=NULL;

	m_pszVertexBuffer=NULL;
	m_pszColorBuffer=NULL;
	m_pszNormalBuffer=NULL;
	m_pszTangentBuffer=NULL;
	m_nUVChannels=0;
	m_pszUVChannels=NULL;
	m_nTris_For_Internal_Use=0;

	m_bVBO=0;
    m_cVBO_vertID=0;
	m_cVBO_clrID=0;
	m_cVBO_nrmlID=0;
	m_cVBO_tangentID=0;
	setBaseFlag(eObject3dBaseFlag_Static);
}

gxMesh::~gxMesh()
{
	if(m_bVBO)
		deleteVBO();

	m_nTriInfoArray=0;
	GX_DELETE_ARY(m_pszTriInfoArray);
	GX_DELETE_ARY(m_pszVertexBuffer);
	GX_DELETE_ARY(m_pszColorBuffer);
	GX_DELETE_ARY(m_pszNormalBuffer);
	GX_DELETE_ARY(m_pszTangentBuffer);

	GX_DELETE_ARY(m_pszUVChannels);
}

void gxMesh::buildVBO()
{
	for (int x=0; x<m_nTriInfoArray; x++)
	{
		gxTriInfo* subTriLst=&m_pszTriInfoArray[x];
		subTriLst->buildVBOTriList();
	}

	// Generate And Bind The Vertex Buffer
	glGenBuffers(1, &m_cVBO_vertID);					// Get A Valid Name
	glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_vertID);		// Bind The Buffer
	glBufferData(GL_ARRAY_BUFFER, m_nTris_For_Internal_Use*3*3*sizeof(float), m_pszVertexBuffer, GL_STATIC_DRAW);

	// Generate And Bind The Color Coordinate Buffer
	if(m_pszColorBuffer)
	{
		glGenBuffers(1, &m_cVBO_clrID);					// Get A Valid Name
		glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_clrID);		// Bind The Buffer
		glBufferData(GL_ARRAY_BUFFER, m_nTris_For_Internal_Use*3*3*sizeof(float), m_pszColorBuffer, GL_STATIC_DRAW);
	}

	// Generate And Bind The Normal Coordinate Buffer
	if(m_pszNormalBuffer)
	{
		glGenBuffers(1, &m_cVBO_nrmlID);					// Get A Valid Name
		glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_nrmlID);		// Bind The Buffer
		glBufferData(GL_ARRAY_BUFFER, m_nTris_For_Internal_Use*3*3*sizeof(float), m_pszNormalBuffer, GL_STATIC_DRAW);
	}

	// Generate And Bind The Tangent Coordinate Buffer
	if(m_pszTangentBuffer)
	{
		glGenBuffers(1, &m_cVBO_tangentID);					// Get A Valid Name
		glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_tangentID);		// Bind The Buffer
		glBufferData(GL_ARRAY_BUFFER, m_nTris_For_Internal_Use*3*4*sizeof(float), m_pszTangentBuffer, GL_STATIC_DRAW);
	}

	// Generate And Bind The Texture Coordinate Buffer
    for(int x=0;x<m_nUVChannels;x++)
    {
        gxUV* uv=&m_pszUVChannels[x];
        if(!uv) continue;

		uv->buildVBOTexCoord(m_nTris_For_Internal_Use);
    }

	glBindBuffer(GL_ARRAY_BUFFER,0);
    m_bVBO=true;
}

void gxMesh::deleteVBO()
{
	if(m_cVBO_vertID)
		glDeleteBuffers(1, &m_cVBO_vertID);
 
	if(m_cVBO_clrID)
		glDeleteBuffers(1, &m_cVBO_clrID);

	if(m_cVBO_nrmlID)
		glDeleteBuffers(1, &m_cVBO_nrmlID);

	if(m_cVBO_tangentID)
		glDeleteBuffers(1, &m_cVBO_tangentID);

  //  for(int x=0;x<m_nUVChannels;x++)
  //  {
  //      gxUV* uv=&m_pszUVChannels[x];
  //      if(!uv) continue;
		//uv->deleteVBOTexCoord();
  //  }
}

void gxMesh::update(float dt)
{
	object3d::update(dt);
}

void gxMesh::render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/)
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

#if defined (USE_ProgrammablePipeLine)
	if(renderer->getRenderPassType()==gxRenderer::RENDER_SHADOWMAP)
	{
		renderForShadowMap(renderer);
	}
	else
	{
		renderWithHWShader(renderer, light);
	}
#else
	renderNormal(renderer);
#endif

	object3d::render(renderer, light, renderFlag);
}

#if 0
void gxMesh::renderNormal(gxRenderer* renderer)
{
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

		if(triInfo->getMaterial())
			glColor4fv(&triInfo->getMaterial()->getDiffuseClr().x);

		int nTexUsed=0;
		for(int m=0;m<m_nUVChannels;m++)
		{
			gxUV* uv=&m_pszUVChannels[m];
			if(triInfo->getMaterial() && applyStageTexture(renderer, nTexUsed, triInfo, uv, GL_TEXTURE_ENV_MODE, GL_MODULATE, 2))
				nTexUsed++;
		}
		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());
		renderer->m_nTrisRendered+=triInfo->getTriangleCount();
		renderer->m_nDrawCalls++;

		disableTextureOperations(nTexUsed, NULL, NULL);
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPopMatrix();
}

void gxMesh::renderWithLight(gxRenderer* renderer, object3d* light)
{

	if(light->getID()!=OBJECT3D_LIGHT)
		return;

	gxLight* light_ob=(gxLight*)light;

	matrix4x4f cMV = *renderer->getViewMatrix() * *getWorldMatrix();
	const float* u_modelview_m4x4=cMV.getMatrix();

	matrix4x4f noscaleMV(cMV);
	noscaleMV.noScale();
	matrix4x4f cMVInverse = noscaleMV.getInverse();
	cMVInverse.transpose();
	const float* u_modelview_inverse_m4x4=cMVInverse.getMatrix();

	matrix4x4f cMVP = *renderer->getViewProjectionMatrix() * *getWorldMatrix();
    const float* u_mvp_m4x4=cMVP.getMatrix();

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		gxTriInfo* triInfo=&m_pszTriInfoArray[x];
		if(!triInfo->getTriList()) continue;

		gxMaterial* material=triInfo->getMaterial();
		if(!material) continue;
		gxHWShader* shader=material->getShaderPass(1);
		shader->enableProgram();

		light_ob->renderPass(renderer, shader);

		shader->sendUniformTMfv("GEAR_MODEL_MATRIX", getMatrix(), false, 4);
		matrix4x4f inv_model=*this;
		inv_model.inverse();
		shader->sendUniformTMfv("GEAR_MODEL_INVERSE", inv_model.getMatrix(), false, 4);

		//shader->sendUniformTMfv("GEAR_MODELVIEW", u_modelview_m4x4, false, 4);
		shader->sendUniformTMfv("GEAR_MVP", u_mvp_m4x4, false, 4);
		//shader->sendUniformTMfv("GEAR_NORMAL_MATRIX", u_modelview_inverse_m4x4, false, 4);

		glVertexAttribPointer(shader->getAttribLoc("vIN_Position"), 3, GL_FLOAT, GL_FALSE, 0, getVertexBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("vIN_Position"));

		glVertexAttribPointer(shader->getAttribLoc("vIN_Normal"), 3, GL_FLOAT, GL_FALSE, 0, getNormalBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("vIN_Normal"));

		glVertexAttribPointer(shader->getAttribLoc("Tangent"), 3, GL_FLOAT, GL_FALSE, 0, getTangentBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("Tangent"));

		if(triInfo->getMaterial())
		{
			shader->sendUniform4fv("material.diffuse", &material->getDiffuseClr().x);
			shader->sendUniform4fv("material.ambient", &material->getAmbientClr().x);
			shader->sendUniform4fv("material.specular", &material->getSpecularClr().x);
			shader->sendUniform1f("material.shininess", 10.0f/*material->getShininess()*/);
		}
		else
		{
			shader->sendUniform4f("material.diffuse", 0.5f, 0.5f, 0.5f, 1.0f);
			shader->sendUniform4f("material.ambient", 0.2f, 0.2f, 0.2f, 1.0f);
			shader->sendUniform4f("material.specular", 0.2f, 0.2f, 0.2f, 1.0f);
			shader->sendUniform1f("material.shininess", 10.0f/*material->getShininess()*/);
		}


		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());
		renderer->m_nTrisRendered+=triInfo->getTriangleCount();
		renderer->m_nDrawCalls++;

		glDisableVertexAttribArray(shader->getAttribLoc("vIN_Normal"));
		glDisableVertexAttribArray(shader->getAttribLoc("vIN_Position"));

		shader->disableProgram();
	}

}
#endif

void gxMesh::renderForShadowMap(gxRenderer* renderer)
{
	HWShaderManager* hwShaderManager = engine_getHWShaderManager();
	gxHWShader* shader = hwShaderManager->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_SHADOWMAP_SHADER);	//no need to enable it, since its been enabled/disable by the caller.
	int vIN_Position = shader->getAttribLoc("a_vertex_coord_v4");

	const float* u_mvp_m4x4 = getWorldMatrix()->getMatrix();
	shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);

	if(m_bVBO)
	{
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_vertID));
		CHECK_GL_ERROR(glVertexAttribPointer(vIN_Position, 3, GL_FLOAT, GL_FALSE, 0, NULL));
	}
	else
	{
		CHECK_GL_ERROR(glVertexAttribPointer(vIN_Position, 3, GL_FLOAT, GL_FALSE, 0, getVertexBuffer()));
	}
	CHECK_GL_ERROR(glEnableVertexAttribArray(vIN_Position));

	float diffuse[]={0.7f, 0.0f, 0.0f, 1.0f};
	CHECK_GL_ERROR(glUniform4fv(shader->getUniformLoc("diffuse"), 1, diffuse));

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		gxTriInfo* triInfo=&m_pszTriInfoArray[x];
		if(!triInfo->getTriList()) continue;

		if(m_bVBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triInfo->getVBOTriListID()));
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, NULL));
			CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
		else
		{
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList()));
		}
	}

	CHECK_GL_ERROR(glDisableVertexAttribArray(vIN_Position));
}

void gxMesh::renderWithHWShader(gxRenderer* renderer, object3d* light)
{
	int pass=0;
	gxLight* light_ob=NULL;
	if(renderer->getRenderPassType()==gxRenderer::RENDER_LIGHTING_ONLY)
	{
		pass=1;
		light_ob=(gxLight*)light;
	}

	matrix4x4f cMV = *renderer->getViewMatrix() * *getWorldMatrix();
	const float* u_modelview_m4x4=cMV.getMatrix();

	matrix4x4f noscaleMV(cMV);
	noscaleMV.noScale();
	matrix4x4f cMVInverse = noscaleMV.getInverse();
	cMVInverse.transpose();
	const float* u_modelview_inverse_m4x4=cMVInverse.getMatrix();

	matrix4x4f cMVP = *renderer->getViewProjectionMatrix() * *getWorldMatrix();
    const float* u_mvp_m4x4=cMVP.getMatrix();

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		gxTriInfo* triInfo=&m_pszTriInfoArray[x];
		if(!triInfo->getTriList()) continue;

		gxMaterial* material=triInfo->getMaterial();
		if(!material) continue;
		gxSurfaceShader* surfaceshader=material->getSurfaceShader();
		if(!surfaceshader) continue;

		if(surfaceshader->getShaderPassCount()<=pass) continue;

		gxHWShader* shader=surfaceshader->getShaderPass(pass);
		stPass* pass_struct = surfaceshader->getShaderPassStruct(pass);

		if(pass_struct->cull_face)
		{
			CHECK_GL_ERROR(glEnable(GL_CULL_FACE));
			CHECK_GL_ERROR(glCullFace(pass_struct->cull_face));
		}
		else
		{
			CHECK_GL_ERROR(glDisable(GL_CULL_FACE));
		}

		shader->enableProgram();

		//time
		if(pass_struct->GEAR_Time)
		{
			float t=Timer::getElapsedTime()*Timer::getTimeScale();
			float time[]={t, t*0.5f, t*0.25f, t*0.1f};
			shader->sendUniform_GEAR_Time(time);
		}

		//screen params
		if(pass_struct->GEAR_ScreenParams)
		{
			vector2f viewport_sz(renderer->getViewPortSz());
			float screenParams[]={viewport_sz.x, viewport_sz.y, 0, 0};
			shader->sendUniform_GEAR_ScreenParams(screenParams);
		}

		if(light_ob && pass_struct->Light)
			light_ob->renderPass(renderer, shader);
		if(pass_struct->GEAR_MV)
		{
			shader->sendUniform_GEAR_MODELVIEW(u_modelview_m4x4);
		}

		if(pass_struct->GEAR_MVP)
		{
			shader->sendUniform_GEAR_MVP(u_mvp_m4x4);
		}

		if(pass_struct->GEAR_M)
		{
			shader->sendUniform_GEAR_MODEL_MATRIX(getWorldMatrix()->getMatrix());
		}

		if(pass_struct->GEAR_M_INVERSE)
		{
			matrix4x4f inv_model=*getWorldMatrix();
			inv_model.inverse();
			shader->sendUniform_GEAR_MODEL_INVERSE(inv_model.getMatrix());
		}

		int vIN_Position=shader->getAttrib_vIN_Position();
		int vIN_Normal=-1;
		int Tangent=-1;

		if(m_bVBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_vertID));
			CHECK_GL_ERROR(glVertexAttribPointer(vIN_Position, 3, GL_FLOAT, GL_FALSE, 0, NULL));
		}
		else
		{
			CHECK_GL_ERROR(glVertexAttribPointer(vIN_Position, 3, GL_FLOAT, GL_FALSE, 0, getVertexBuffer()));
		}
		CHECK_GL_ERROR(glEnableVertexAttribArray(vIN_Position));

		if(pass_struct->Light)
		{
			vIN_Normal=shader->getAttrib_vIN_Normal();
			if(m_bVBO)
			{
				CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_nrmlID));
				CHECK_GL_ERROR(glVertexAttribPointer(vIN_Normal, 3, GL_FLOAT, GL_FALSE, 0, NULL));
			}
			else
			{
				CHECK_GL_ERROR(glVertexAttribPointer(vIN_Normal, 3, GL_FLOAT, GL_FALSE, 0, getNormalBuffer()));
			}
			CHECK_GL_ERROR(glEnableVertexAttribArray(vIN_Normal));
		}

		if(pass_struct->Tangent)
		{
			Tangent=shader->getAttrib_Tangent();
			if(m_bVBO)
			{
				CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_tangentID));
				CHECK_GL_ERROR(glVertexAttribPointer(Tangent, 4, GL_FLOAT, GL_FALSE, 0, NULL));
			}
			else
			{
				CHECK_GL_ERROR(glVertexAttribPointer(Tangent, 4, GL_FLOAT, GL_FALSE, 0, getTangentBuffer()));
			}
			CHECK_GL_ERROR(glEnableVertexAttribArray(Tangent));
		}

		if(pass_struct->Material)
		{
			if(triInfo->getMaterial())
			{
				shader->sendUniform_material_diffuse(&material->getDiffuseClr().x);
				shader->sendUniform_material_ambient(&material->getAmbientClr().x);
				shader->sendUniform_material_specular(&material->getSpecularClr().x);
				shader->sendUniform_material_shininess(material->getShininess());
			}
			else
			{
				float diffuse[]={0.5f, 0.5f, 0.5f, 1.0f};
				float ambient[]={0.2f, 0.2f, 0.2f, 1.0f};
				float specular[]={0.2f, 0.2f, 0.2f, 1.0f};

				shader->sendUniform_material_diffuse(diffuse);
				shader->sendUniform_material_ambient(ambient);
				shader->sendUniform_material_specular(specular);
				shader->sendUniform_material_shininess(10.0f);
			}
		}

		int nTexUsed=0;
		int cntr=0;
		//std::vector<gxSubMap*>* maplist=material->getSubMapList();
		//gxUV* base_uv=(m_nUVChannels)?&m_pszUVChannels[0]:NULL;
		//stShaderProperty_Texture2D* base_tex_var=NULL;
		//for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it, ++cntr)
		//{
		//	gxSubMap* submap = *it;
		//	stShaderProperty_Texture2D* shader_var=submap->getShaderTextureProperty();
		//	if(!base_tex_var)
		//		base_tex_var=shader_var;
		//	gxUV* uv=&m_pszUVChannels[0];	//base uv
		//	if(applyStageTexture(renderer, nTexUsed, triInfo, base_uv, GL_TEXTURE_ENV_MODE, GL_REPLACE, 2, shader, base_tex_var->texture_uv_in_name.c_str()))
		//	{
		//		shader->sendUniform1i(shader_var->texture_sampler2d_name.c_str(), nTexUsed);
		//		nTexUsed++;
		//	}
		//}
		std::vector<stMaterialPass*>* material_pass=material->getShaderPassList();
		stMaterialPass*	mpass=material_pass->at(pass);

		for(std::vector<gxSubMap*>::iterator it = mpass->vUsedSubMap.begin(); it != mpass->vUsedSubMap.end(); ++it, ++cntr)
		{
			gxSubMap* submap = *it;
			stShaderProperty_Texture2D* shader_var=submap->getShaderTextureProperty();
			int texenv1=0;
			int texenv2=0;
#ifdef _WIN32
			texenv1=GL_TEXTURE_ENV_MODE;
			texenv2=GL_MODULATE;
#endif
			gxUV* base_uv=(cntr<m_nUVChannels)?&m_pszUVChannels[cntr]:((m_nUVChannels)?&m_pszUVChannels[0]:NULL);
			if(applyStageTexture(renderer, nTexUsed, triInfo, base_uv, submap, texenv1, texenv2, 2, shader, shader_var->texture_uv_in_name.c_str()))
			{
				shader->sendUniform1i(shader_var->texture_sampler2d_name.c_str(), nTexUsed);
				nTexUsed++;
			}
		}

		if(m_bVBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triInfo->getVBOTriListID()));
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, NULL));
			CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
		else
		{
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList()));
		}

		renderer->m_nTrisRendered+=triInfo->getTriangleCount();
		renderer->m_nDrawCalls++;

		cntr=0;
		for(std::vector<gxSubMap*>::iterator it = mpass->vUsedSubMap.begin(); it != mpass->vUsedSubMap.end(); ++it, cntr++)
		{
			gxSubMap* submap = *it;
			stShaderProperty_Texture2D* shader_var=submap->getShaderTextureProperty();
			disableTextureOperations(cntr, shader, shader_var->texture_uv_in_name.c_str());
		}

		CHECK_GL_ERROR(glDisableVertexAttribArray(vIN_Position));

		if(pass_struct->Light)
		{
			CHECK_GL_ERROR(glDisableVertexAttribArray(vIN_Normal));
		}

		if(pass_struct->Tangent)
		{
			CHECK_GL_ERROR(glDisableVertexAttribArray(Tangent));
		}

		if(m_bVBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER,0));
		}

		shader->disableProgram();
	}
}

#if 0
bool gxMesh::applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz)
{
	if(!triInfo->getMaterial()) return false;

	gxSubMap* subMap=NULL;
	subMap=triInfo->getMaterial()->getSubMap(stage);

	if(!subMap)	return false;
	if(!subMap->getTexture()) return false;
	if(subMap->getTexture()->getTextureID()==0 || !uv) return false;

	glActiveTexture(GL_TEXTURE0+stage);
	glClientActiveTexture(GL_TEXTURE0+stage);
	glTexCoordPointer(texCoordSz, GL_FLOAT, 0, uv->m_pszfGLTexCoordList);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, subMap->getTexture()->getTextureID() );
	glTexEnvf(GL_TEXTURE_ENV, aTexEnv1, (float)aTexEnv2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(subMap->getTexture()->getTextureMatrix()->getMatrix());
	glMatrixMode(GL_MODELVIEW);

	if(subMap->getTexture()->getTextureType()==gxTexture::TEX_ALPHA)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	return true;
}
#endif

bool gxMesh::applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, gxSubMap* submap, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz, gxHWShader* shader, const char* texCoordAttribName)
{
	if(!shader) return false;

	bool bUse1x1Texture=true;
	if(triInfo->getMaterial())
	{
		if(submap && submap->getTexture())
			bUse1x1Texture=false;
	}

	gxHWShader* hwShader=(gxHWShader*)shader;
    CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0+stage));
	if(bUse1x1Texture)
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, renderer->getGEARTexture1x1()->iTextureID));	
	else
	{
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, submap->getTexture()->getTextureID()));	
#if defined(TEXENV_ISSUE) && defined(_WIN32)
		glTexEnvf(GL_TEXTURE_ENV, aTexEnv1, (float)aTexEnv2);
#endif
		if(submap->getTexture()->getTextureType()==gxTexture::TEX_ALPHA)
		{
			//need to fix this bug
			CHECK_GL_ERROR(glEnable(GL_BLEND));
			CHECK_GL_ERROR(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			setBaseFlag(eObject3dBaseFlag_Alpha);	//check this
			//
		}
	}

	if(uv)
	{
		if(m_bVBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, uv->m_cVBO_texID));
			CHECK_GL_ERROR(glVertexAttribPointer(hwShader->getAttribLoc(texCoordAttribName), 2, GL_FLOAT, GL_FALSE, 0, 0));
		}
		else
		{
			CHECK_GL_ERROR(glVertexAttribPointer(hwShader->getAttribLoc(texCoordAttribName), 2, GL_FLOAT, GL_FALSE, 0, uv->m_pszfGLTexCoordList));
		}
		CHECK_GL_ERROR(glEnableVertexAttribArray(hwShader->getAttribLoc(texCoordAttribName)));
	}
	//glEnable(GL_TEXTURE_2D);


	return true;
}

void gxMesh::disableTextureOperations(int stage, gxHWShader* shader, const char* texCoordAttribName)
{
#if defined (USE_ProgrammablePipeLine)
	if(shader)
	{
		CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0+stage));
		CHECK_GL_ERROR(glDisableVertexAttribArray(shader->getAttribLoc(texCoordAttribName)));
		//glDisable(GL_TEXTURE_2D);
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
		//glDisable(GL_TEXTURE_2D);
	}
#else
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
#endif
	//need to fix this bug
	CHECK_GL_ERROR(glDisable(GL_BLEND));
	//
}

float* gxMesh::allocateVertexBuffer(int nTris)
{
	m_nTris_For_Internal_Use=nTris;
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

float* gxMesh::allocateTangentBuffer(int nTris)
{
	GX_DELETE_ARY(m_pszTangentBuffer);
	m_pszTangentBuffer = new float[nTris*3*4];
	return m_pszTangentBuffer;
}

bool gxMesh::createTBN_Data()
{
	//need to use the below mentioned url
	//http://www.terathon.com/code/tangent.html

	float* diffuse_uv_coordPtr=NULL;
	float* tangent_coord=NULL;
	gxUV* base_uv=(m_nUVChannels)?&m_pszUVChannels[0]:NULL;

	if(!base_uv || !m_pszNormalBuffer)
		return false;

	diffuse_uv_coordPtr = base_uv->m_pszfGLTexCoordList;
	if(!diffuse_uv_coordPtr)
		return false;

	//Allocate memory for the additional coords
	GX_DELETE_ARY(m_pszTangentBuffer);
	m_pszTangentBuffer = new float[m_nTris_For_Internal_Use*3*3];
	tangent_coord = m_pszTangentBuffer;
	
	float* v=getVertexBuffer();
	float* tc=diffuse_uv_coordPtr;

	float* ac1= NULL;
	float* ac2= NULL;
	float* ac3= NULL;

	vector3f dv2v1;
	vector3f dv3v1;
	vector2f dc2c1TB;
	vector2f dc3c1TB;

	int aIterator=0;
	//for(int x=0;x<m_nTriInfoArray;x++)
	//{
	//	gxTriInfo* triInfo=&m_pszTriInfoArray[x];
	//	if(!triInfo->getTriList()) continue;

		for(int y=0;y<m_nTris_For_Internal_Use;y++)
		{
			vector3f v1(v[aIterator*9+0], v[aIterator*9+1], v[aIterator*9+2]);
			vector3f v2(v[aIterator*9+3], v[aIterator*9+4], v[aIterator*9+5]);
			vector3f v3(v[aIterator*9+6], v[aIterator*9+7], v[aIterator*9+8]);

			dv2v1 = v2-v1;
			dv3v1 = v3-v1;

			vector2f st1(tc[aIterator*6+0], tc[aIterator*6+1]);
			vector2f st2(tc[aIterator*6+2], tc[aIterator*6+3]);
			vector2f st3(tc[aIterator*6+4], tc[aIterator*6+5]);

			dc2c1TB = st2-st1;
			dc3c1TB = st3-st1;

			float den = dc2c1TB.x * dc3c1TB.y - dc3c1TB.x * dc2c1TB.y;

			ac1=&tangent_coord[aIterator*9+0];
			//ac2=&aMesh->iGLAdditionalCoordList2[aIterator*9+0];
			//ac3=&aMesh->iGLAdditionalCoordList3[aIterator*9+0];

			if (ROUNDOFF(den)==0.0f)	
			{
				//make the identity matrix
				ac1[0]=/*ac2[0]=ac3[0]=*/1.0f;	ac1[1]=/*ac2[1]=ac3[1]=*/0.0f;	ac1[2]=/*ac2[2]=ac3[2]=*/0.0f;
				ac1[3]=/*ac2[3]=ac3[3]=*/0.0f;	ac1[4]=/*ac2[4]=ac3[4]=*/1.0f;	ac1[5]=/*ac2[5]=ac3[5]=*/0.0f;
				ac1[6]=/*ac2[6]=ac3[6]=*/0.0f;	ac1[7]=/*ac2[7]=ac3[7]=*/0.0f;	ac1[8]=/*ac2[8]=ac3[8]=*/1.0f;
			}
			else
			{
				// Calculate the reciprocal value once and for all (to achieve speed)
				float fScale1 = 1.0f/den;

				// T and B are calculated just as the equation in the article states
				vector3f T, B, N;
				T = vector3f((dc3c1TB.y * dv2v1.x - dc2c1TB.y * dv3v1.x) * fScale1,
					(dc3c1TB.y * dv2v1.y - dc2c1TB.y * dv3v1.y) * fScale1,
					(dc3c1TB.y * dv2v1.z - dc2c1TB.y * dv3v1.z) * fScale1);

				B = vector3f((-dc3c1TB.x * dv2v1.x + dc2c1TB.x * dv3v1.x) * fScale1,
					(-dc3c1TB.x * dv2v1.y + dc2c1TB.x * dv3v1.y) * fScale1,
					(-dc3c1TB.x * dv2v1.z + dc2c1TB.x * dv3v1.z) * fScale1);

				// The normal N is calculated as the cross product between T and B
				N = T.cross(B);

				// Calculate the reciprocal value once and for all (to achieve speed)
				float fScale2 = 1.0f/((T.x * B.y * N.z - T.z * B.y * N.x) + (B.x * N.y * T.z - B.z * N.y * T.x) + (N.x * T.y * B.z - N.z * T.y * B.x));


				//T
				vector3f X(B.cross(N).x*fScale2, -N.cross(T).x*fScale2, T.cross(B).x*fScale2);
				X.normalize();
				ac1[0]=ac1[3]=ac1[6]=X.x;	ac1[1]=ac1[4]=ac1[7]=X.y;	ac1[2]=ac1[5]=ac1[8]=X.z;

				////B
				//gxPoint3f Y(-B.Cross(N).y*fScale2, N.Cross(T).y*fScale2, -T.Cross(B).y*fScale2);
				//Y.Normalize();
				//ac2[0]=ac2[3]=ac2[6]=Y.x;	ac2[1]=ac2[4]=ac2[7]=Y.y;	ac2[2]=ac2[5]=ac2[8]=Y.z;

				////N
				//gxPoint3f Z(B.Cross(N).z*fScale2, -N.Cross(T).z*fScale2, T.Cross(B).z*fScale2);
				//Z.Normalize();
				//ac3[0]=ac3[3]=ac3[6]=Z.x;	ac3[1]=ac3[4]=ac3[7]=Z.y;	ac3[2]=ac3[5]=ac3[8]=Z.z;
			}

			aIterator++;
		}
	//}

	return true;
}

int gxMesh::getVerticesCount()
{
	int nVerts=0;
	for(int x=0;x<m_nTriInfoArray;x++)
	{
		nVerts+=m_pszTriInfoArray[x].getVerticesCount();
	}

	return nVerts;
}

void gxMesh::writeMeshData(gxFile& file)
{
	file.Write(m_iObjectID);
	file.Write(m_eBaseFlags);
	file.Write(m_cszName);
	file.WriteBuffer((unsigned char*)m, sizeof(m));
	file.WriteBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Write(m_iAssetFileCRC);
	writeAnimationController(file);

	file.Write(m_nTriInfoArray);
	for(int x=0;x<m_nTriInfoArray;x++)
	{
		m_pszTriInfoArray[x].write(file);
	}

	file.Write(m_nTris_For_Internal_Use);
	if(m_pszVertexBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszVertexBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}
	else
	{
		file.Write(false);
	}

	if(m_pszColorBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszColorBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}
	else
	{
		file.Write(false);
	}

	if(m_pszNormalBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszNormalBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}
	else
	{
		file.Write(false);
	}

	if(m_pszTangentBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszTangentBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*4);
	}
	else
	{
		file.Write(false);
	}

	file.Write(m_nUVChannels);
	for(int x=0;x<m_nUVChannels;x++)
	{
		if(m_pszUVChannels[x].m_pMaterialPtr)
			file.Write(m_pszUVChannels[x].m_pMaterialPtr->getAssetFileCRC());
		else
			file.Write((int)0);
		file.WriteBuffer((unsigned char*)m_pszUVChannels[x].m_pszfGLTexCoordList, sizeof(float)*m_nTris_For_Internal_Use*3*2);
	}
}

void gxMesh::write(gxFile& file)
{
	writeMeshData(file);

	file.Write((int)m_cChilds.size());

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=m_cChilds.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->write(file);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->write(file);
	}
#endif
}

void gxMesh::read(gxFile& file)
{
	file.Read(m_eBaseFlags);
	char* temp=file.ReadString();
	GX_STRCPY(m_cszName, temp);
	GX_DELETE_ARY(temp);
	file.ReadBuffer((unsigned char*)m, sizeof(m));
	file.ReadBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Read(m_iAssetFileCRC);
	readAnimationController(file);

	file.Read(m_nTriInfoArray);
	if(m_nTriInfoArray)
		m_pszTriInfoArray = new gxTriInfo[m_nTriInfoArray];

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		m_pszTriInfoArray[x].read(file);
	}

	file.Read(m_nTris_For_Internal_Use);

	bool bVertexBuffer=false;
	file.Read(bVertexBuffer);
	if(bVertexBuffer)
	{
		float* buffer=allocateVertexBuffer(m_nTris_For_Internal_Use);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}

	bool bColorBuffer=false;
	file.Read(bColorBuffer);
	if(bColorBuffer)
	{
		float* buffer=allocateColorBuffer(m_nTris_For_Internal_Use);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}

	bool bNormalBuffer=false;
	file.Read(bNormalBuffer);
	if(bNormalBuffer)
	{
		float* buffer=allocateNormalBuffer(m_nTris_For_Internal_Use);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}

	bool bTangentBuffer=false;
	file.Read(bTangentBuffer);
	if(bTangentBuffer)
	{
		float* buffer=allocateTangentBuffer(m_nTris_For_Internal_Use);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nTris_For_Internal_Use*3*4);
	}


	file.Read(m_nUVChannels);
	if(m_nUVChannels)
		m_pszUVChannels = new gxUV[m_nUVChannels];
	for(int x=0;x<m_nUVChannels;x++)
	{
		int materialCRC=0;
		file.Read(materialCRC);
		m_pszUVChannels[x].m_pszfGLTexCoordList =new float[m_nTris_For_Internal_Use*3*2];
		file.ReadBuffer((unsigned char*)m_pszUVChannels[x].m_pszfGLTexCoordList, sizeof(float)*m_nTris_For_Internal_Use*3*2);
	}

	if(getID()==OBJECT3D_MESH)
		buildVBO();
	//createTBN_Data();
}