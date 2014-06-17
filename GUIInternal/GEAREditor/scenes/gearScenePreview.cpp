#include "gearScenePreview.h"
#include "../EditorApp.h"
//#include "../../../GEAREngine/src/core/Timer.h"

gearScenePreview::gearScenePreview():
geWindow("Preview")
{
	m_pSelectedObj=NULL;
	m_pPreviewWorldPtr=NULL;
	m_bStopFollowCam=false;
	m_pLightPtr=NULL;
}

gearScenePreview::~gearScenePreview()
{
}

void gearScenePreview::onCreate()
{
	reinitPreviewWorld();
}

void gearScenePreview::reinitPreviewWorld()
{
	if(m_pPreviewWorldPtr)
		m_pPreviewWorldPtr->resetWorld();
	m_pPreviewWorldPtr=monoWrapper::mono_engine_getWorld(1);
	m_pPreviewWorldPtr->getActiveCamera()->setNear(1.0f);
	object3d* light=engine_createLight(m_pPreviewWorldPtr, "Light", gxLight::LIGHT_DIRECTIONAL);
	((gxLight*)light)->setDiffuseColor(vector4f(0.5f, 0.5f, 0.5f, 1.0f));
	((gxLight*)light)->setAmbientColor(vector4f(0.1f, 0.1f, 0.1f, 1.0f));
	((gxLight*)light)->setSpecularColor(vector4f(0.25f, 0.25f, 0.25f, 1.0f));
	((gxLight*)light)->setConstantAttenuation(0.5f);
	light->updatePositionf(-1, -10, 1);
	m_pLightPtr=light;
}

void gearScenePreview::draw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	if(!m_pSelectedObj) return;

	monoWrapper::mono_engine_resize(m_pPreviewWorldPtr, m_cPos.x+getIamOnLayout()->getPos().x, (m_pRenderer->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/, 1.0f, 10000.0f);

	onDraw();

	//STATS
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (m_pRenderer->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D((int)0, (int)(m_cSize.x), (int)(m_cSize.y-getTopMarginOffsetHeight()), (int)0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, -1);
	glDisable(GL_DEPTH_TEST);
	CHECK_GL_ERROR(geGUIManager::g_pFontArial10_84Ptr->drawString("1 object selected", 5, 5+geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x));
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	//
}

void gearScenePreview::onDraw()
{
	monoWrapper::mono_engine_update(m_pPreviewWorldPtr, Timer::getDtinSec());
	followObject(Timer::getDtinSec(), m_pSelectedObj);

	monoWrapper::mono_engine_renderSingleObject(m_pPreviewWorldPtr, m_pSelectedObj, NULL);
}

void gearScenePreview::selectedObject3D(object3d* obj)
{
	m_pSelectedObj=obj;
	if(m_pSelectedObj)
		m_pSelectedObj->identity();
	m_bStopFollowCam=false;	
}

void gearScenePreview::followObject(float dt, object3d* chasedObj)
{
	if(dt>0.1f || m_bStopFollowCam) return;
	if(chasedObj==NULL) return;

	Camera* cam=m_pPreviewWorldPtr->getActiveCamera();
	matrix4x4f* chasingObj=(matrix4x4f*)cam;
    //matrix4x4f* chasedObj=(matrix4x4f*)this;
	vector3f	eyeOff;
	float speed=10.0f;
	
	eyeOff = vector3f(0, -(chasedObj->getAABB().getLongestAxis()*0.5f + cam->getNear())*2.5f, 0);

	vector3f    transformedEye((chasedObj->getAABB().getCenter()) + eyeOff);
	vector3f    transformedLookAt(chasedObj->getAABB().getCenter());
	
	vector3f    chasingObjPos(cam->getWorldMatrix()->getPosition());
    vector3f    chasedObjPos(chasedObj->getAABB().getCenter());
    vector3f    lenV(transformedEye-chasingObjPos);
    float       len=lenV.length();
	
    if(len<=0.01f)
	{
		m_bStopFollowCam=true;
		return;
	}

    if(len>4000.0f)
    {
        float factor=4000.0f/len;
        lenV=lenV*factor;
    }
	
    vector3f    updatedPos(chasingObjPos+lenV*(speed*dt));
    vector3f forward(updatedPos-transformedLookAt);
    forward.normalize();
    vector3f up(0, 0, 1);
    vector3f left(up.cross(forward));
    left.normalize();
    up=forward.cross(left);
    up.normalize();
	
	chasingObj->setXAxis(left);
	chasingObj->setYAxis(up);
	chasingObj->setZAxis(forward);
	chasingObj->setPosition(updatedPos);
	
	m_pLightPtr->setPosition(updatedPos);

	cam->updateCamera();
}

bool gearScenePreview::onMouseLButtonDown(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseLButtonDown(m_pPreviewWorldPtr, x, y, nFlag);
	m_cPrevMousePos.set(x, y);
	return true;
}

bool gearScenePreview::onMouseLButtonUp(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseLButtonUp(m_pPreviewWorldPtr, x, y, nFlag);
	m_cPrevMousePos.set(x, y);
	return true;
}

bool gearScenePreview::onMouseRButtonDown(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseRButtonDown(m_pPreviewWorldPtr, x, y, nFlag);
	m_cPrevMousePos.set(x, y);
	return true;
}

void gearScenePreview::onMouseRButtonUp(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseRButtonUp(m_pPreviewWorldPtr, x, y, nFlag);
	m_cPrevMousePos.set(x, y);
}

bool gearScenePreview::onMouseMove(float x, float y, int flag)
{
	//monoWrapper::mono_engine_mouseMove(m_pPreviewWorldPtr, x, y, flag);
	if(!m_pSelectedObj)
		return true;

	int xx=x;
	int yy=y;
	int xPos = xx;
	int yPos = yy;
	int Pos_dx	= abs(xPos-m_cPrevMousePos.x);
	int Pos_dy	= abs(yPos-m_cPrevMousePos.y);

	int aDirX=-1;
	int aDirY=1;
	if(m_cPrevMousePos.x>xPos)		aDirX=1;
	if(m_cPrevMousePos.y>yPos)		aDirY=-1;

#ifdef _WIN32
	/*if(flag&MK_MBUTTON)
	{
		float d=m_pSelectedObj->getPosition().length();
		if(flag&MK_SHIFT)
			m_pSelectedObj->updateLocalPositionf((d/5000.0f)*Pos_dx*aDirX, (d/5000.0f)*Pos_dy*aDirY, 0);
		else
			m_pSelectedObj->updateLocalPositionf((d/500.0f)*Pos_dx*aDirX, (d/500.0f)*Pos_dy*aDirY, 0);
	}
	else*/ if(flag&MK_RBUTTON)
#endif
	{

		vector3f aUP(0, 0, 1);
		//aUP=camera->getYAxis();
		vector3f aVect(m_pSelectedObj->getAABB().getCenter());
		//aVect=m_cPickObjectCenter;	//can modify this later to rotate around mesh center
		Camera* cam=m_pPreviewWorldPtr->getActiveCamera();

		m_pSelectedObj->rotateArb(0.5f*Pos_dx*-aDirX, &aUP.x, aVect);
		vector3f left=cam->getXAxis();
		m_pSelectedObj->rotateArb(0.5f*Pos_dy*aDirY, &left.x, aVect);
	}
	m_cPrevMousePos.set(x, y);
	return true;
}

void gearScenePreview::onMouseWheel(int zDelta, int x, int y, int flag)
{
	monoWrapper::mono_engine_mouseWheel(m_pPreviewWorldPtr, zDelta, x, y, flag);
}