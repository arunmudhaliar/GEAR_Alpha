#include "gxMesh.h"
#include "../GEAREngine.h"
#include "Timer.h"

gxMesh::gxMesh(int ID):
	object3d(ID)
{
	triangleInfoArrayCount=0;
	triangleInfoArray=NULL;

	vertexBuffer=NULL;
	colorBuffer=NULL;
	normalBuffer=NULL;
	tangentBuffer=NULL;
	uvChannelCount=0;
	uvChannel=NULL;
	noOfTrianglesForInternalUse=0;

	is_VBO=0;
    vboVertexID=0;
	vboColorID=0;
	vboNormalID=0;
	vboTangentID=0;
	setBaseFlag(eObject3dBaseFlag_Static);
}

gxMesh::gxMesh():
	object3d(OBJECT3D_MESH)
{
	triangleInfoArrayCount=0;
	triangleInfoArray=NULL;

	vertexBuffer=NULL;
	colorBuffer=NULL;
	normalBuffer=NULL;
	tangentBuffer=NULL;
	uvChannelCount=0;
	uvChannel=NULL;
	noOfTrianglesForInternalUse=0;

	is_VBO=0;
    vboVertexID=0;
	vboColorID=0;
	vboNormalID=0;
	vboTangentID=0;
	setBaseFlag(eObject3dBaseFlag_Static);
}

gxMesh::~gxMesh()
{
	if(is_VBO)
		deleteVBO();

	triangleInfoArrayCount=0;
	GX_DELETE_ARY(triangleInfoArray);
	GX_DELETE_ARY(vertexBuffer);
	GX_DELETE_ARY(colorBuffer);
	GX_DELETE_ARY(normalBuffer);
	GX_DELETE_ARY(tangentBuffer);

	GX_DELETE_ARY(uvChannel);
}

void gxMesh::buildVBO()
{
	for (int x=0; x<triangleInfoArrayCount; x++)
	{
		gxTriInfo* subTriLst=&triangleInfoArray[x];
		subTriLst->buildVBOTriList();
	}

	// Generate And Bind The Vertex Buffer
	glGenBuffers(1, &vboVertexID);					// Get A Valid Name
	glBindBuffer(GL_ARRAY_BUFFER, vboVertexID);		// Bind The Buffer
	glBufferData(GL_ARRAY_BUFFER, noOfTrianglesForInternalUse*3*3*sizeof(float), vertexBuffer, GL_STATIC_DRAW);

	// Generate And Bind The Color Coordinate Buffer
	if(colorBuffer)
	{
		glGenBuffers(1, &vboColorID);					// Get A Valid Name
		glBindBuffer(GL_ARRAY_BUFFER, vboColorID);		// Bind The Buffer
		glBufferData(GL_ARRAY_BUFFER, noOfTrianglesForInternalUse*3*3*sizeof(float), colorBuffer, GL_STATIC_DRAW);
	}

	// Generate And Bind The Normal Coordinate Buffer
	if(normalBuffer)
	{
		glGenBuffers(1, &vboNormalID);					// Get A Valid Name
		glBindBuffer(GL_ARRAY_BUFFER, vboNormalID);		// Bind The Buffer
		glBufferData(GL_ARRAY_BUFFER, noOfTrianglesForInternalUse*3*3*sizeof(float), normalBuffer, GL_STATIC_DRAW);
	}

	// Generate And Bind The Tangent Coordinate Buffer
	if(tangentBuffer)
	{
		glGenBuffers(1, &vboTangentID);					// Get A Valid Name
		glBindBuffer(GL_ARRAY_BUFFER, vboTangentID);		// Bind The Buffer
		glBufferData(GL_ARRAY_BUFFER, noOfTrianglesForInternalUse*3*4*sizeof(float), tangentBuffer, GL_STATIC_DRAW);
	}

	// Generate And Bind The Texture Coordinate Buffer
    for(int x=0;x<uvChannelCount;x++)
    {
        gxUV* uv=&uvChannel[x];
        if(!uv) continue;

		uv->buildVBOTexCoord(noOfTrianglesForInternalUse);
    }

	glBindBuffer(GL_ARRAY_BUFFER,0);
    is_VBO=true;
}

void gxMesh::deleteVBO()
{
	if(vboVertexID)
		glDeleteBuffers(1, &vboVertexID);
 
	if(vboColorID)
		glDeleteBuffers(1, &vboColorID);

	if(vboNormalID)
		glDeleteBuffers(1, &vboNormalID);

	if(vboTangentID)
		glDeleteBuffers(1, &vboTangentID);

  //  for(int x=0;x<uvChannelCount;x++)
  //  {
  //      gxUV* uv=&uvChannel[x];
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
	glVertexPointer(3, GL_FLOAT, 0, vertexBuffer);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normalBuffer);

	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		gxTriInfo* triInfo=&triangleInfoArray[x];
		if(!triInfo->getTriList()) continue;

		if(triInfo->getMaterial())
			glColor4fv(&triInfo->getMaterial()->getDiffuseClr().x);

		int nTexUsed=0;
		for(int m=0;m<uvChannelCount;m++)
		{
			gxUV* uv=&uvChannel[m];
			if(triInfo->getMaterial() && applyStageTexture(renderer, nTexUsed, triInfo, uv, GL_TEXTURE_ENV_MODE, GL_MODULATE, 2))
				nTexUsed++;
		}
		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());
		renderer->noOfTrianglesRendered+=triInfo->getTriangleCount();
		renderer->noOfDrawCalls++;

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

	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		gxTriInfo* triInfo=&triangleInfoArray[x];
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
		renderer->noOfTrianglesRendered+=triInfo->getTriangleCount();
		renderer->noOfDrawCalls++;

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

	if(is_VBO)
	{
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vboVertexID));
		CHECK_GL_ERROR(glVertexAttribPointer(vIN_Position, 3, GL_FLOAT, GL_FALSE, 0, NULL));
	}
	else
	{
		CHECK_GL_ERROR(glVertexAttribPointer(vIN_Position, 3, GL_FLOAT, GL_FALSE, 0, getVertexBuffer()));
	}
	CHECK_GL_ERROR(glEnableVertexAttribArray(vIN_Position));

	//float diffuse[]={0.7f, 0.0f, 0.0f, 1.0f};
	//CHECK_GL_ERROR(glUniform4fv(shader->getUniformLoc("diffuse"), 1, diffuse));
	CHECK_GL_ERROR(glEnable(GL_CULL_FACE));
	CHECK_GL_ERROR(glCullFace(GL_FRONT));

	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		gxTriInfo* triInfo=&triangleInfoArray[x];
		if(!triInfo->getTriList()) continue;

		if(is_VBO)
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

	if (is_VBO)
	{
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
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
//	const float* u_modelview_inverse_m4x4=cMVInverse.getMatrix();

	matrix4x4f cMVP = *renderer->getViewProjectionMatrix() * *getWorldMatrix();
    const float* u_mvp_m4x4=cMVP.getMatrix();

	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		gxTriInfo* triInfo=&triangleInfoArray[x];
		if(!triInfo->getTriList()) continue;

		gxMaterial* material=triInfo->getMaterial();
		if(!material) continue;
		gxSurfaceShader* surfaceshader=material->getSurfaceShader();
		if(!surfaceshader) continue;

		if(surfaceshader->getShaderPassCount()<=pass) continue;

		gxHWShader* shader=surfaceshader->getShaderPass(pass);
		stPass* pass_struct = surfaceshader->getShaderPassStruct(pass);


		if (pass_struct->cull_face)
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

		if(is_VBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vboVertexID));
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
			if(is_VBO)
			{
				CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vboNormalID));
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
			if(is_VBO)
			{
				CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vboTangentID));
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
		//gxUV* base_uv=(uvChannelCount)?&uvChannel[0]:NULL;
		//stShaderProperty_Texture2D* base_tex_var=NULL;
		//for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it, ++cntr)
		//{
		//	gxSubMap* submap = *it;
		//	stShaderProperty_Texture2D* shader_var=submap->getShaderTextureProperty();
		//	if(!base_tex_var)
		//		base_tex_var=shader_var;
		//	gxUV* uv=&uvChannel[0];	//base uv
		//	if(applyStageTexture(renderer, nTexUsed, triInfo, base_uv, GL_TEXTURE_ENV_MODE, GL_REPLACE, 2, shader, base_tex_var->texture_uv_in_name.c_str()))
		//	{
		//		shader->sendUniform1i(shader_var->texture_sampler2d_name.c_str(), nTexUsed);
		//		nTexUsed++;
		//	}
		//}
		std::vector<stMaterialPass*>* material_pass=material->getShaderPassList();
		stMaterialPass*	mpass=material_pass->at(pass);

		for (std::vector<gxSubMap*>::iterator it = mpass->vUsedSubMap.begin(); it != mpass->vUsedSubMap.end(); ++it, ++cntr)
		{
			gxSubMap* submap = *it;
			stShaderProperty_Texture2D* shader_var = submap->getShaderTextureProperty();
			int texenv1 = 0;
			int texenv2 = 0;
#ifdef GEAR_WINDOWS
			texenv1 = GL_TEXTURE_ENV_MODE;
			texenv2 = GL_MODULATE;
#endif
			gxUV* base_uv = (cntr < uvChannelCount) ? &uvChannel[cntr] : ((uvChannelCount) ? &uvChannel[0] : NULL);
			if (applyStageTexture(renderer, nTexUsed, triInfo, base_uv, submap, texenv1, texenv2, 2, shader, shader_var->texture_uv_in_name.c_str()))
			{
				shader->sendUniform1i(shader_var->texture_sampler2d_name.c_str(), nTexUsed);
				nTexUsed++;
			}
		}

		//shadowmap
		if (pass_struct->shadow == 1 && light_ob)
		{
			float* depth_bias_mvp = (light_ob->getShadowDepthBiasMVP() * *getWorldMatrix()).getOGLMatrix();
			//GEAR_SHADOW_DEPTH_BIAS_MVP
			int GEAR_SHADOW_DEPTH_BIAS_MVP = shader->getUniformLoc("GEAR_SHADOW_DEPTH_BIAS_MVP");
			CHECK_GL_ERROR(glUniformMatrix4fv(GEAR_SHADOW_DEPTH_BIAS_MVP, 1, false, depth_bias_mvp));

			CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0 + nTexUsed));
			CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, light_ob->getShadowMapFBO().getFBODepthBuffer()));
#if defined(TEXENV_ISSUE) && defined(GEAR_WINDOWS)
			glTexEnvf(GL_TEXTURE_ENV, aTexEnv1, (float)aTexEnv2);
#endif
			shader->sendUniform1i("sampler2d_ShadowMap", nTexUsed);
		}
		//

		if(is_VBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triInfo->getVBOTriListID()));
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, NULL));
			CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
		else
		{
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList()));
		}

		renderer->noOfTrianglesRendered+=triInfo->getTriangleCount();
		renderer->noOfDrawCalls++;

		cntr=0;

		for (std::vector<gxSubMap*>::iterator it = mpass->vUsedSubMap.begin(); it != mpass->vUsedSubMap.end(); ++it, cntr++)
		{
			gxSubMap* submap = *it;
			stShaderProperty_Texture2D* shader_var = submap->getShaderTextureProperty();
			disableTextureOperations(cntr, shader, shader_var->texture_uv_in_name.c_str());
		}

#if defined (USE_ProgrammablePipeLine)
		if (pass_struct->shadow == 1 && light_ob)
		{
			CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0 + cntr));
			//CHECK_GL_ERROR(glDisableVertexAttribArray(shader->getAttribLoc(texCoordAttribName)));
			//glDisable(GL_TEXTURE_2D);
			CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
			//glDisable(GL_TEXTURE_2D);
		}
#else
#error "Not Implemented"
#endif

		CHECK_GL_ERROR(glDisableVertexAttribArray(vIN_Position));

		if(pass_struct->Light)
		{
			CHECK_GL_ERROR(glDisableVertexAttribArray(vIN_Normal));
		}

		if(pass_struct->Tangent)
		{
			CHECK_GL_ERROR(glDisableVertexAttribArray(Tangent));
		}

		if(is_VBO)
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

	gxSubMap* subMapList=NULL;
	subMapList=triInfo->getMaterial()->getSubMap(stage);

	if(!subMapList)	return false;
	if(!subMapList->getTexture()) return false;
	if(subMapList->getTexture()->getTextureID()==0 || !uv) return false;

	glActiveTexture(GL_TEXTURE0+stage);
	glClientActiveTexture(GL_TEXTURE0+stage);
	glTexCoordPointer(texCoordSz, GL_FLOAT, 0, uv->textureCoordArray);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, subMapList->getTexture()->getTextureID() );
	glTexEnvf(GL_TEXTURE_ENV, aTexEnv1, (float)aTexEnv2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(subMapList->getTexture()->getTextureMatrix()->getMatrix());
	glMatrixMode(GL_MODELVIEW);

	if(subMapList->getTexture()->getTextureType()==gxTexture::TEX_ALPHA)
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
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, renderer->getGEARTexture1x1()->textureID));	
	else
	{
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, submap->getTexture()->getTextureID()));	
#if defined(TEXENV_ISSUE) && defined(GEAR_WINDOWS)
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
		if(is_VBO)
		{
			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, uv->vboID));
			CHECK_GL_ERROR(glVertexAttribPointer(hwShader->getAttribLoc(texCoordAttribName), 2, GL_FLOAT, GL_FALSE, 0, 0));
		}
		else
		{
			CHECK_GL_ERROR(glVertexAttribPointer(hwShader->getAttribLoc(texCoordAttribName), 2, GL_FLOAT, GL_FALSE, 0, uv->textureCoordArray));
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
	noOfTrianglesForInternalUse=nTris;
	GX_DELETE_ARY(vertexBuffer);
	vertexBuffer = new float[nTris*3*3];
	return vertexBuffer;
}

float* gxMesh::allocateColorBuffer(int nTris)
{
	GX_DELETE_ARY(colorBuffer);
	colorBuffer = new float[nTris*3*3];
	return colorBuffer;
}

float* gxMesh::allocateNormalBuffer(int nTris)
{
	GX_DELETE_ARY(normalBuffer);
	normalBuffer = new float[nTris*3*3];
	return normalBuffer;
}

float* gxMesh::allocateTangentBuffer(int nTris)
{
	GX_DELETE_ARY(tangentBuffer);
	tangentBuffer = new float[nTris*3*4];
	return tangentBuffer;
}

bool gxMesh::createTBN_Data()
{
	//need to use the below mentioned url
	//http://www.terathon.com/code/tangent.html

	float* diffuse_uv_coordPtr=NULL;
	float* tangent_coord=NULL;
	gxUV* base_uv=(uvChannelCount)?&uvChannel[0]:NULL;

	if(!base_uv || !normalBuffer)
		return false;

	diffuse_uv_coordPtr = base_uv->textureCoordArray;
	if(!diffuse_uv_coordPtr)
		return false;

	//Allocate memory for the additional coords
	GX_DELETE_ARY(tangentBuffer);
	tangentBuffer = new float[noOfTrianglesForInternalUse*3*3];
	tangent_coord = tangentBuffer;
	
	float* v=getVertexBuffer();
	float* tc=diffuse_uv_coordPtr;

	float* ac1= NULL;
//	float* ac2= NULL;
//	float* ac3= NULL;

	vector3f dv2v1;
	vector3f dv3v1;
	vector2f dc2c1TB;
	vector2f dc3c1TB;

	int aIterator=0;
	//for(int x=0;x<triangleInfoArrayCount;x++)
	//{
	//	gxTriInfo* triInfo=&triangleInfoArray[x];
	//	if(!triInfo->getTriList()) continue;

		for(int y=0;y<noOfTrianglesForInternalUse;y++)
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
	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		nVerts+=triangleInfoArray[x].getVerticesCount();
	}

	return nVerts;
}

void gxMesh::writeData(gxFile& file)
{
    file.Write(triangleInfoArrayCount);
    for(int x=0;x<triangleInfoArrayCount;x++)
    {
        triangleInfoArray[x].write(file);
    }
    
    file.Write(noOfTrianglesForInternalUse);
    if(vertexBuffer)
    {
        file.Write(true);
        file.WriteBuffer((unsigned char*)vertexBuffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
    }
    else
    {
        file.Write(false);
    }
    
    if(colorBuffer)
    {
        file.Write(true);
        file.WriteBuffer((unsigned char*)colorBuffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
    }
    else
    {
        file.Write(false);
    }
    
    if(normalBuffer)
    {
        file.Write(true);
        file.WriteBuffer((unsigned char*)normalBuffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
    }
    else
    {
        file.Write(false);
    }
    
    if(tangentBuffer)
    {
        file.Write(true);
        file.WriteBuffer((unsigned char*)tangentBuffer, sizeof(float)*noOfTrianglesForInternalUse*3*4);
    }
    else
    {
        file.Write(false);
    }
    
    file.Write(uvChannelCount);
    for(int x=0;x<uvChannelCount;x++)
    {
        if(uvChannel[x].material)
            file.Write(uvChannel[x].material->getAssetFileCRC());
        else
            file.Write((int)0);
        file.WriteBuffer((unsigned char*)uvChannel[x].textureCoordArray, sizeof(float)*noOfTrianglesForInternalUse*3*2);
    }
}

void gxMesh::readData(gxFile& file)
{
	file.Read(triangleInfoArrayCount);
	if(triangleInfoArrayCount)
		triangleInfoArray = new gxTriInfo[triangleInfoArrayCount];

	for(int x=0;x<triangleInfoArrayCount;x++)
	{
		triangleInfoArray[x].read(file);
	}

	file.Read(noOfTrianglesForInternalUse);

	bool bVertexBuffer=false;
	file.Read(bVertexBuffer);
	if(bVertexBuffer)
	{
		float* buffer=allocateVertexBuffer(noOfTrianglesForInternalUse);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
	}

	bool bColorBuffer=false;
	file.Read(bColorBuffer);
	if(bColorBuffer)
	{
		float* buffer=allocateColorBuffer(noOfTrianglesForInternalUse);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
	}

	bool bNormalBuffer=false;
	file.Read(bNormalBuffer);
	if(bNormalBuffer)
	{
		float* buffer=allocateNormalBuffer(noOfTrianglesForInternalUse);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfTrianglesForInternalUse*3*3);
	}

	bool bTangentBuffer=false;
	file.Read(bTangentBuffer);
	if(bTangentBuffer)
	{
		float* buffer=allocateTangentBuffer(noOfTrianglesForInternalUse);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*noOfTrianglesForInternalUse*3*4);
	}


	file.Read(uvChannelCount);
	if(uvChannelCount)
		uvChannel = new gxUV[uvChannelCount];
	for(int x=0;x<uvChannelCount;x++)
	{
		int materialCRC=0;
		file.Read(materialCRC);
		uvChannel[x].textureCoordArray =new float[noOfTrianglesForInternalUse*3*2];
		file.ReadBuffer((unsigned char*)uvChannel[x].textureCoordArray, sizeof(float)*noOfTrianglesForInternalUse*3*2);
	}

	if(getID()==OBJECT3D_MESH)
		buildVBO();
	//createTBN_Data();
}