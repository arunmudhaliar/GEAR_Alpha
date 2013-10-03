#ifndef GXONLYDIFFUSEWITHCOLOR_H
#define GXONLYDIFFUSEWITHCOLOR_H

#include "gxHWShader.h"

class __declspec( dllexport ) gxOnlyDiffuseWithColor : public gxHWShader
{
public:
	
	gxOnlyDiffuseWithColor();
	~gxOnlyDiffuseWithColor();
	
	virtual void bind();
	virtual void getUniformVars();
	virtual void updateShaderVars(float dt);
	virtual void inputShaderUniformVars(void* ptr);
	virtual void inputShaderAttribVars(void* ptr);

	GLint getUserDefinedUniform(int index)		{	return 0;	}
	
	virtual void renderThroughHWShader(object3d* obj, const matrix4x4f* parentTM);
    
    void renderAsNormalMesh(object3d* obj, const matrix4x4f* parentTM);
#if 0
    void renderAsVBOMesh(objectBase* obj, const matrix4x4f* parentTM);
#endif
};
#endif