#include "gxGenericShader.h"
#include "../core/gxMesh.h"

gxGenericShader::gxGenericShader():
gxHWShader()
{

}

gxGenericShader::~gxGenericShader()
{
}

void gxGenericShader::bind()
{
}

void gxGenericShader::getUniformVars()
{
}

void gxGenericShader::updateShaderVars(float dt)
{
}

void gxGenericShader::inputShaderUniformVars(void* ptr)
{
}

void gxGenericShader::inputShaderAttribVars(void* ptr)
{
}

void gxGenericShader::renderAsNormalMesh(object3d* obj, const matrix4x4f* parentTM)
{
}


void gxGenericShader::renderThroughHWShader(object3d* obj, const matrix4x4f* parentTM)
{
}
