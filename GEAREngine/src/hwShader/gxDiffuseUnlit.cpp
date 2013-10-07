#include "gxDiffuseUnlit.h"
#include "../core/gxMesh.h"

gxDiffuseUnlit::gxDiffuseUnlit():
gxHWShader()
{

}

gxDiffuseUnlit::~gxDiffuseUnlit()
{
}

void gxDiffuseUnlit::bind()
{
}

void gxDiffuseUnlit::getUniformVars()
{
}

void gxDiffuseUnlit::updateShaderVars(float dt)
{
}

void gxDiffuseUnlit::inputShaderUniformVars(void* ptr)
{
}

void gxDiffuseUnlit::inputShaderAttribVars(void* ptr)
{
}

void gxDiffuseUnlit::renderAsNormalMesh(object3d* obj, const matrix4x4f* parentTM)
{
//    gxMesh* mesh = (gxMesh*)obj;
//    
//	GLboolean bLighting=true;
//	//glGetBooleanv(GL_LIGHTING, &bLighting);
//	//if(m_pEngineObserver) m_pEngineObserver->PreRenderMesh(this);
//    
//	gxHWShader* shader=this;
//    
//	shader->enableProgram();
//	shader->resetAllFlags();
//    
//	glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, mesh->getVertexBuffer());
//    glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
//    
//	//glEnableClientState(GL_VERTEX_ARRAY);
//	//glVertexPointer(3, GL_FLOAT, 0, m_pszfGLVertexList);
//    
////	//normal coordinates
////	if(mesh->isMeshFlag(gxMesh::MESH_USE_NORMAL_ARY))
////	{
////		//glEnableClientState(GL_NORMAL_ARRAY);
////		//glNormalPointer(GL_FLOAT, 0, m_pszfGLNrmlCoordList);
////		glVertexAttribPointer(shader->getAttribLoc("a_normal_coord_v3"), 3, GL_FLOAT, GL_FALSE, 0, mesh->getNormalBuffer());
////		glEnableVertexAttribArray(shader->getAttribLoc("a_normal_coord_v3"));
////	}
//    
//	//glEnable(GL_COLOR_MATERIAL);
//	////color coordinates
//	//if(isMeshFlag(gxMesh::MESH_USE_COLOR_ARY))
//	//{
//	//	glEnableClientState(GL_COLOR_ARRAY);
//	//	glColorPointer(3, GL_FLOAT, 0, m_pszfGLClrCoordList);
//	//}
//    
//	//if(mesh->isBaseFlag(objectBase::OBJF_USE_TM))
//	//{
//        matrix4x4f cRenderMatrix = *objectBase::getRenderer()->getProjectionMatrix() * (*parentTM * *mesh);
//        const float* u_mvp_m4x4=cRenderMatrix.getMatrix();
//		shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);
//        
//        //shader->setFlag(gxHWShader::OBJBASE_FLAG_USE_TM, 1.0f);
//        //		glPushMatrix();
//        //		glMultMatrixf(m);
//	//}
// //   else
// //   {
// //       const float* u_mvp_m4x4=objectBase::getRenderer()->getViewProjectionMatrix()->getMatrix();
//	//	shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);
// //   }
//    
//	//GXbool bUseNormalMapShader=GX_FALSE;
//	//GXbool bUseSimpleLightingShader=GX_FALSE;
//    
////	vector3f eye=objectBase::getRenderer()->getViewMatrix()->getPosition();
////	shader->sendAttrib3fv("a_eyePos_v3", &eye.x);
////    
//	for(int y=0;y<mesh->getSubTriListCount();y++)
//	{
//		gxSubTriList* subTriLst=mesh->getSubTriList(y);
//		int nTexUsed=0;
//        
//		//cUniformInput.u_flags_m4x4[1]=0.0f;
//        
//		if(subTriLst->m_iMatID>=0)
//		{
//			gxMaterial* mat=subTriLst->m_pMaterialPtr;
//			if(mat->isTwoSided())
//				glDisable(GL_CULL_FACE);
//			else
//				glEnable(GL_CULL_FACE);
//            
//            vector4f diffuseClr(mat->getDiffuseClr());
//			shader->sendUniform4fv("u_diffuse_clr", &diffuseClr.x);
////			gxUV* lightmap_uv= mesh->getUV(gxSubMaterialMap::EMMAP_OPACITY);
////			if(lightmap_uv)
////			{
////				gxSubMaterialMap* subMatMap=lightmap_uv->m_pMaterialPtr->getSubMaterialMap(gxSubMaterialMap::EMMAP_DIFFUSE);	
////				if(mesh->applyStageTexture(nTexUsed, NULL, lightmap_uv, subMatMap->getTexture(), GL_TEXTURE_ENV_MODE, GL_REPLACE, 2, 0, shader, "a_uv_coord1_v2"))
////				{
////                    shader->setFlag(gxHWShader::MATERIAL_FLAG_USE_OPACITY_MAP, 1.0f);
////					nTexUsed++;
////				}
////			}
////            
//			//if(mat->getSubMaterialMap(gxSubMaterialMap::EMMAP_DIFFUSE))
//			//{
//			//	gxSubMaterialMap* subMatMap=mat->getSubMaterialMap(gxSubMaterialMap::EMMAP_DIFFUSE);
//			//	gxUV* uv=mesh->getUV(gxSubMaterialMap::EMMAP_DIFFUSE);
//			//	if(	mesh->applyStageTexture(nTexUsed, subMatMap, uv, subMatMap->getTexture(), GL_TEXTURE_ENV_MODE, 
//   //                                   (mesh->isMeshFlag(gxMesh::MESH_USE_LIGHTMAP_ARY) || bLighting)?GL_MODULATE:GL_REPLACE, 2, 0, shader, "a_uv_coord0_v2"))
//			//	{
//   //                 shader->sendUniform1i("u_diffuse_texture", 0);
//   //                 shader->setFlag(gxHWShader::MATERIAL_FLAG_USE_DIFFUSE_MAP, 1.0f);
//			//		nTexUsed++;
//			//	}
//			//}
//            
//			//if(mat->m_eMFlags&gxMaterial::EMAT_USE_NORMAL_TEX)
//			//{
//			//	//ApplyStageTexture(nTexUsed, diff_tex_tm, m_psziGLExtra1CoordList,
//			//	//	mat->m_szShaderAry[Shader::ESH_NORMAL]->m_uGLTexID, GL_TEXTURE_ENV_MODE, GL_MODULATE, 3);
//			//	//nTexUsed++;
//			//}
//            
//			//if(mat->m_eMFlags&gxMaterial::EMAT_USE_SPECULAR_TEX)
//			//{
//			//	//ApplyStageTexture(nTexUsed, diff_tex_tm, m_psziGLTexCoordList,
//			//	//	mat->m_szShaderAry[Shader::ESH_SPECULAR]->m_uGLTexID, GL_TEXTURE_ENV_MODE, GL_MODULATE, 2);
//			//	//nTexUsed++;
//			//}
//            
////			vector4f diffuse(mat->getDiffuseClr());
////			vector4f ambient(mat->getAmbientClr());
////			vector4f specular(mat->getSpecularClr());
////            
////            
////			shader->sendAttrib4fv("a_diffuse_v4", &diffuse.x);
////			shader->sendAttrib4fv("a_ambient_v4", &ambient.x);
////			shader->sendAttrib4fv("a_specular_v4", &specular.x);
////			shader->sendUniform1f("u_shininess", mat->getShininess());
//		}
////		else
////		{
////			float whitecolor[]={1.0f, 1.0f, 1.0f, 1.0f};
////			shader->sendAttrib4fv("a_diffuse_v4", whitecolor);
////			shader->sendAttrib4fv("a_ambient_v4", whitecolor);
////			shader->sendAttrib4fv("a_specular_v4", whitecolor);
////			shader->sendUniform1f("u_shininess", 1.0f);
////		}
////		shader->sendUniformTMfv("u_flags_m4x4", shader->getFlagPtr(), false, 4);
//        
//        
//		glDrawElements(GL_TRIANGLES, subTriLst->m_nTris * 3, GL_UNSIGNED_SHORT, subTriLst->m_pszTri);
//		
//		//Disable all texture ops
//		mesh->disableTextureOperations(nTexUsed, shader, "a_uv_coord0_v2");
//		glDisable(GL_BLEND);
//	}
//    
//	glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
////	glDisableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2"));
////	glDisableVertexAttribArray(shader->getAttribLoc("a_normal_coord_v3"));
//	
//	shader->disableProgram();
//    
////	//render child nodes
////	stLinkNode<objectBase*>* node=m_cChildList.getHead();
////	while(node)
////	{
////		objectBase* obj=node->getData();
////		obj->render();
////		node=node->getNext();
////	}
//    
//	//if((isBaseFlag(OBJF_USE_TM)))
//	//	glPopMatrix();
//    
//	//glDisable(GL_COLOR_MATERIAL);
//	//glDisableClientState(GL_COLOR_ARRAY);
//	//glDisableClientState(GL_NORMAL_ARRAY);
//	//glDisableClientState(GL_VERTEX_ARRAY);
}

#if 0
void gxDiffuseUnlit::renderAsVBOMesh(objectBase* obj, const matrix4x4f* parentTM)
{
    gxMesh* mesh = (gxMesh*)obj;
    
	GLboolean bLighting=false;
	//glGetBooleanv(GL_LIGHTING, &bLighting);
	//if(m_pEngineObserver) m_pEngineObserver->PreRenderMesh(this);
    
    gxHWShader* shader=this;
    
	shader->enableProgram();
	shader->resetAllFlags();
    
	//glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVBOVertexID());
    glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
    
    //	//normal coordinates
    //	if(isMeshFlag(gxMesh::MESH_USE_NORMAL_ARY))
    //	{
    //		//glEnableClientState(GL_NORMAL_ARRAY);
    //        glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_nrmlID);
    //        glVertexAttribPointer(shader->getAttribLoc("a_normal_coord_v3"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //		glEnableVertexAttribArray(shader->getAttribLoc("a_normal_coord_v3"));
    //		//glNormalPointer(GL_FLOAT, 0, NULL);
    //	}
    
    //	glEnable(GL_COLOR_MATERIAL);
    //	//color coordinates
    //	if(isMeshFlag(gxMesh::MESH_USE_COLOR_ARY))
    //	{
    //		glEnableClientState(GL_COLOR_ARRAY);
    //        glBindBuffer(GL_ARRAY_BUFFER, m_cVBO_clrID);
    //		glColorPointer(3, GL_FLOAT, 0, NULL);
    //	}
    
	if(mesh->isBaseFlag(objectBase::OBJF_USE_TM))
	{
		matrix4x4f cRenderMatrix = *objectBase::getRenderer()->getProjectionMatrix() * (*parentTM * *mesh);
        const float* u_mvp_m4x4=cRenderMatrix.getMatrix();
		shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);

        //shader->setFlag(gxHWShader::OBJBASE_FLAG_USE_TM, 1.0f);
        //		glPushMatrix();
        //		glMultMatrixf(m);
	}
    else
    {
        const float* u_mvp_m4x4=objectBase::getRenderer()->getViewProjectionMatrix()->getMatrix();
		shader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);
    }
	//GXbool bUseNormalMapShader=GX_FALSE;
	//GXbool bUseSimpleLightingShader=GX_FALSE;
    
    //    vector3f eye=objectBase::getRenderer()->getViewMatrix()->getPosition();
    //	shader->sendAttrib3fv("a_eyePos_v3", &eye.x);
    
	for(int y=0;y<mesh->getSubTriListCount();y++)
	{
		gxSubTriList* subTriLst=mesh->getSubTriList(y);
		int nTexUsed=0;
		if(subTriLst->m_iMatID>=0)
		{
			gxMaterial* mat=subTriLst->m_pMaterialPtr; //m_pvMaterialLst->At(subTriLst->m_iMatID);
			//matrix4x4f* diff_tex_tm=(mat->m_eMFlags&gxMaterial::EMAT_USE_DIFFUSE_TEX)?mat->m_szShaderAry[Shader::ESH_DIFFUSE]->m_TM:NULL;
			if(mat->isTwoSided())
				glDisable(GL_CULL_FACE);
			else
				glEnable(GL_CULL_FACE);
			
			//ApplyMaterial(mat, 1.0f);
			vector4f diffuseClr(mat->getDiffuseClr());
			shader->sendUniform4fv("u_diffuse_v4", &diffuseClr.x);
			//glColor4f(diffuseClr.x, diffuseClr.y, diffuseClr.z, 1.0f);
            
            //			gxUV* lightmap_uv=mesh->getUV(gxSubMaterialMap::EMMAP_OPACITY);
            //			if(lightmap_uv)
            //			{
            //				gxSubMaterialMap* subMatMap=lightmap_uv->m_pMaterialPtr->getSubMaterialMap(gxSubMaterialMap::EMMAP_DIFFUSE);	
            //				if(mesh->applyStageTexture(nTexUsed, NULL, lightmap_uv, subMatMap->getTexture(), GL_TEXTURE_ENV_MODE, GL_REPLACE, 2, 0, shader))
            //                {
            //                    shader->setFlag(gxHWShader::MATERIAL_FLAG_USE_OPACITY_MAP, 1.0f);
            //                    nTexUsed++;
            //                }
            //			}
            
			//if(mat->getSubMaterialMap(gxSubMaterialMap::EMMAP_DIFFUSE))
			//{
			//	gxSubMaterialMap* subMatMap=mat->getSubMaterialMap(gxSubMaterialMap::EMMAP_DIFFUSE);
			//	
			//	gxUV* uv=mesh->getUV(gxSubMaterialMap::EMMAP_DIFFUSE);
			//	if(	mesh->applyStageTexture(nTexUsed, subMatMap, uv, subMatMap->getTexture(), GL_TEXTURE_ENV_MODE, 
   //                                         (mesh->isMeshFlag(gxMesh::MESH_USE_LIGHTMAP_ARY) || bLighting)?GL_MODULATE:GL_REPLACE, 2, 0, shader, "a_uv_coord0_v2"))
   //             {
   //                 shader->sendUniform1i("u_diffuse_texture", 0);
   //                 shader->setFlag(gxHWShader::MATERIAL_FLAG_USE_DIFFUSE_MAP, 1.0f);
   //                 nTexUsed++;
   //             }
			//}
            
			//if(mat->m_eMFlags&gxMaterial::EMAT_USE_NORMAL_TEX)
			//{
			//	//ApplyStageTexture(nTexUsed, diff_tex_tm, m_psziGLExtra1CoordList,
			//	//	mat->m_szShaderAry[Shader::ESH_NORMAL]->m_uGLTexID, GL_TEXTURE_ENV_MODE, GL_MODULATE, 3);
			//	//nTexUsed++;
			//}
            
			//if(mat->m_eMFlags&gxMaterial::EMAT_USE_SPECULAR_TEX)
			//{
			//	//ApplyStageTexture(nTexUsed, diff_tex_tm, m_psziGLTexCoordList,
			//	//	mat->m_szShaderAry[Shader::ESH_SPECULAR]->m_uGLTexID, GL_TEXTURE_ENV_MODE, GL_MODULATE, 2);
			//	//nTexUsed++;
			//}
            
            //            vector4f diffuse(mat->getDiffuseClr());
            //			vector4f ambient(mat->getAmbientClr());
            //			vector4f specular(mat->getSpecularClr());
            //            
            //            
            //			shader->sendAttrib4fv("a_diffuse_v4", &diffuse.x);
            //			shader->sendAttrib4fv("a_ambient_v4", &ambient.x);
            //			shader->sendAttrib4fv("a_specular_v4", &specular.x);
            //			shader->sendUniform1f("u_shininess", mat->getShininess());
		}
		else
        {
            //            float whitecolor[]={1.0f, 1.0f, 1.0f, 1.0f};
            //			shader->sendAttrib4fv("a_diffuse_v4", whitecolor);
            //			shader->sendAttrib4fv("a_ambient_v4", whitecolor);
            //			shader->sendAttrib4fv("a_specular_v4", whitecolor);
            //			shader->sendUniform1f("u_shininess", 1.0f);
		}
        //		shader->sendUniformTMfv("u_flags_m4x4", shader->getFlagPtr(), false, 4);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subTriLst->m_uVBO_trilstID);
		glDrawElements(GL_TRIANGLES, subTriLst->m_nTris * 3, GL_UNSIGNED_SHORT, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
		//Disable all texture ops
		mesh->disableTextureOperations(nTexUsed, shader, "a_uv_coord0_v2");
		glDisable(GL_BLEND);
	}
        
	glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
	//glDisableVertexAttribArray(shader->getAttribLoc("a_uv_coord0_v2"));
    //	glDisableVertexAttribArray(shader->getAttribLoc("a_normal_coord_v3"));
	
    glBindBuffer(GL_ARRAY_BUFFER,0);
//#if !defined (ANDROID)
//    glBindVertexArrayOES(0);
//#endif

    
    shader->disableProgram();

    
    //	//render child nodes
    //	stLinkNode<objectBase*>* node=m_cChildList.getHead();
    //	while(node)
    //	{
    //		objectBase* obj=node->getData();
    //		obj->render();
    //		node=node->getNext();
    //	}
    
    //	if((isBaseFlag(OBJF_USE_TM)))
    //		glPopMatrix();
    //    
    //	glDisable(GL_COLOR_MATERIAL);
    //	glDisableClientState(GL_COLOR_ARRAY);
    //	glDisableClientState(GL_NORMAL_ARRAY);
    //	glDisableClientState(GL_VERTEX_ARRAY);
    
	//if(m_pEngineObserver) m_pEngineObserver->PostRenderMesh(this);
}
#endif

void gxDiffuseUnlit::renderThroughHWShader(object3d* obj, const matrix4x4f* parentTM)
{
	//if(obj->getBaseID()!=OBJ_ID_MESH) return;

	//gxMesh* mesh = (gxMesh*)obj;

 //   //if(mesh->isVBO())
 //   //    renderAsVBOMesh(mesh, parentTM);
 //   //else
 //       renderAsNormalMesh(mesh, parentTM);
}
