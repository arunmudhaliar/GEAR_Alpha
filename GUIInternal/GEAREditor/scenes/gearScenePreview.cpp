#include "gearScenePreview.h"
#include "../EditorApp.h"
//#include "../../../GEAREngine/src/core/Timer.h"

gearScenePreview::gearScenePreview(geFontManager* fontmanager):
geWindow("Preview", fontmanager)
{
	selectedObject=NULL;
	previewWorld=NULL;
	stopFollowCam=false;
	previewLight=NULL;
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
	if(previewWorld)
		previewWorld->resetWorld();
	previewWorld=monoWrapper::mono_engine_getWorld(1);
	previewWorld->getActiveCamera()->setNear(1.0f);
	object3d* light=engine_createLight(previewWorld, "Light", gxLight::LIGHT_DIRECTIONAL);
	((gxLight*)light)->setDiffuseColor(vector4f(0.75f, 0.75f, 0.75f, 1.0f));
	((gxLight*)light)->setAmbientColor(vector4f(0.2f, 0.2f, 0.2f, 1.0f));
	((gxLight*)light)->setSpecularColor(vector4f(0.5f, 0.5f, 0.5f, 1.0f));
	((gxLight*)light)->setConstantAttenuation(0.5f);
	light->updatePositionf(-1, -10, 1);
	previewLight=light;
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

	if(!selectedObject) return;

	monoWrapper::mono_engine_resize(previewWorld, m_cPos.x+getIamOnLayout()->getPos().x, (rendererGUI->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/, 1.0f, 10000.0f);

	onDraw();

	//STATS
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (rendererGUI->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D((int)0, (int)(m_cSize.x), (int)(m_cSize.y-getTopMarginOffsetHeight()), (int)0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, -1);
	glDisable(GL_DEPTH_TEST);
	CHECK_GL_ERROR(geFontManager::g_pFontArial10_84Ptr->drawString("1 object selected", 5, 5+geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x));
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	//
}

void gearScenePreview::onDraw()
{
	monoWrapper::mono_engine_update(previewWorld, Timer::getDtinSec());
	followObject(Timer::getDtinSec(), selectedObject);

	monoWrapper::mono_engine_renderSingleObject(previewWorld, selectedObject, NULL, object3d::eObject3dBase_RenderFlag_NormalRenderPass);
}

void gearScenePreview::selectedObject3D(object3d* obj)
{
	selectedObject=obj;
	if(selectedObject)
		selectedObject->identity();
	stopFollowCam=false;	
}

void gearScenePreview::followObject(float dt, object3d* chasedObj)
{
	if(dt>0.1f || stopFollowCam) return;
	if(chasedObj==NULL) return;

	Camera* cam=previewWorld->getActiveCamera();
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
		stopFollowCam=true;
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
	
	previewLight->setPosition(updatedPos);

	cam->updateCamera();
}

bool gearScenePreview::onMouseLButtonDown(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseLButtonDown(previewWorld, x, y, nFlag);
	previousMousePosition.set(x, y);
	return true;
}

bool gearScenePreview::onMouseLButtonUp(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseLButtonUp(previewWorld, x, y, nFlag);
	previousMousePosition.set(x, y);
	return true;
}

bool gearScenePreview::onMouseRButtonDown(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseRButtonDown(previewWorld, x, y, nFlag);
	previousMousePosition.set(x, y);
	return true;
}

void gearScenePreview::onMouseRButtonUp(float x, float y, int nFlag)
{
	monoWrapper::mono_engine_mouseRButtonUp(previewWorld, x, y, nFlag);
	previousMousePosition.set(x, y);
}

bool gearScenePreview::onMouseMove(float x, float y, int flag)
{
	//monoWrapper::mono_engine_mouseMove(previewWorld, x, y, flag);
	if(!selectedObject)
		return true;

	int xx=x;
	int yy=y;
	int xPos = xx;
	int yPos = yy;
	int Pos_dx	= abs(xPos-previousMousePosition.x);
	int Pos_dy	= abs(yPos-previousMousePosition.y);

	int aDirX=-1;
	int aDirY=1;
	if(previousMousePosition.x>xPos)		aDirX=1;
	if(previousMousePosition.y>yPos)		aDirY=-1;

#ifdef _WIN32
	/*if(flag&MK_MBUTTON)
	{
		float d=selectedObject->getPosition().length();
		if(flag&MK_SHIFT)
			selectedObject->updateLocalPositionf((d/5000.0f)*Pos_dx*aDirX, (d/5000.0f)*Pos_dy*aDirY, 0);
		else
			selectedObject->updateLocalPositionf((d/500.0f)*Pos_dx*aDirX, (d/500.0f)*Pos_dy*aDirY, 0);
	}
	else*/
#endif
    if(flag&MK_RBUTTON)
	{
		vector3f aUP(0, 0, 1);
		//aUP=camera->getYAxis();
		vector3f aVect(selectedObject->getAABB().getCenter());
		//aVect=m_cPickObjectCenter;	//can modify this later to rotate around mesh center
		Camera* cam=previewWorld->getActiveCamera();

		selectedObject->rotateArb(0.5f*Pos_dx*-aDirX, &aUP.x, aVect);
		vector3f left=cam->getXAxis();
		selectedObject->rotateArb(0.5f*Pos_dy*aDirY, &left.x, aVect);
	}
	previousMousePosition.set(x, y);
	return true;
}

void gearScenePreview::onMouseWheel(int zDelta, int x, int y, int flag)
{
	monoWrapper::mono_engine_mouseWheel(previewWorld, zDelta, x, y, flag);
}