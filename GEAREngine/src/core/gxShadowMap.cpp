#include "gxShadowMap.h"

gxShadowMap::gxShadowMap()
{

}

gxShadowMap::~gxShadowMap()
{

}

void gxShadowMap::createShadowMap()
{
	matrix4x4f biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);
	m_cBiasMatrix = biasMatrix;	//TODO: optimize this.

	//m_cDepthProjectionMatrix.setOrtho(-GX_SHADOW_MAP_SIZE*0.5f, GX_SHADOW_MAP_SIZE*0.5f,
	//	-GX_SHADOW_MAP_SIZE*0.5f, GX_SHADOW_MAP_SIZE*0.5f, 0, 1000);
	m_cDepthProjectionMatrix.setOrtho(-100, 100,
		-100, 100, 0, 1000);

	m_cFBO.ReInitFBO(GX_SHADOW_MAP_SIZE, GX_SHADOW_MAP_SIZE);
	m_cFBO.CreateDepthBuffer();
	m_cFBO.AttachDepthBuffer();
	m_cFBO.UnBindFBO();
}

void gxShadowMap::updateMatrix(vector3f& light_pos)
{
	matrix4x4f depthViewMatrix;
	//light_pos = -light_pos;
	vector3f forward(light_pos);
	forward.normalize();
	vector3f up(0, 0, 1);
	vector3f left(up.cross(forward));
	left.normalize();
	up = forward.cross(left);
	up.normalize();
	depthViewMatrix.setXAxis(left);
	depthViewMatrix.setYAxis(up);
	depthViewMatrix.setZAxis(forward);
	depthViewMatrix.setPosition(light_pos);

	m_cDepthMVP = m_cDepthProjectionMatrix * depthViewMatrix.getInverse();
	m_cDepthBiasMVP = m_cBiasMatrix * m_cDepthMVP;
}