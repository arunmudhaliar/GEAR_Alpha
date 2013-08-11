#include "gearSceneWorldEditor.h"
#include "../EditorApp.h"
#include "../core/Timer.h"
#include "../gui/geToolBarSeperator.h"

gearSceneWorldEditor::gearSceneWorldEditor():
geWindow("World Editor")
{
	m_pSelectedObj=NULL;
	m_pMainWorldPtr=NULL;

	m_pHorizontalSlider_LightAmbient=NULL;
	m_pLocalOrGlobalAxis=NULL;
	m_pTBGridView=NULL;

	m_pPlayButton = NULL;
	m_pPauseButton = NULL;
}

gearSceneWorldEditor::~gearSceneWorldEditor()
{
	//GE_DELETE(m_pHorizontalSlider_LightAmbient);
}

void gearSceneWorldEditor::onCreate()
{
	monoWrapper::mono_engine_init(2);
	m_pMainWorldPtr = monoWrapper::mono_engine_getWorld(0);
	m_pMainWorldPtr->setWorldObserver(this);

	char metaDataFolder[512];
	memset(metaDataFolder, 0, sizeof(metaDataFolder));
	sprintf(metaDataFolder, "%s/%s", EditorApp::getProjectHomeDirectory(), "MetaData");
	monoWrapper::mono_engine_setMetaFolder(monoWrapper::mono_engine_getWorld(0), metaDataFolder);
	monoWrapper::mono_engine_setMetaFolder(monoWrapper::mono_engine_getWorld(1), metaDataFolder);

	geToolBarButton* tbtn0=new geToolBarButton("t", getToolBar());
	tbtn0->loadImage("res//icons16x16.png", 7, 153);
	getToolBar()->appendToolBarControl(tbtn0);
	geToolBarButton* tbtn1=new geToolBarButton("r", getToolBar());
	tbtn1->loadImage("res//icons16x16.png", 27, 153);
	getToolBar()->appendToolBarControl(tbtn1);
	geToolBarButton* tbtn2=new geToolBarButton("s", getToolBar());
	tbtn2->loadImage("res//icons16x16.png", 49, 153);
	getToolBar()->appendToolBarControl(tbtn2);

	m_pHorizontalSlider_LightAmbient = new geHorizontalSlider();
	m_pHorizontalSlider_LightAmbient->create(getToolBar(), "slider", 0, GE_TOOLBAR_HEIGHT*0.35f, 70);
	m_pHorizontalSlider_LightAmbient->setSliderValue(0.03f);
	getToolBar()->appendToolBarControl(m_pHorizontalSlider_LightAmbient);

	geToolBarSeperator* seperator = new geToolBarSeperator(getToolBar(), 20);
	getToolBar()->appendToolBarControl(seperator);
	geToolBarSeperator* seperator2 = new geToolBarSeperator(getToolBar(), 40);
	getToolBar()->appendToolBarControl(seperator2);

	m_pLocalOrGlobalAxis=new geToolBarButton("Local", getToolBar());
	m_pLocalOrGlobalAxis->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pLocalOrGlobalAxis);
	m_bTransformThroughLocalAxis=true;

	m_pTBGridView=new geToolBarButton("grid", getToolBar());
	m_pTBGridView->loadImage("res//icons16x16.png", 112, 384);
	m_pTBGridView->buttonPressed();
	getToolBar()->appendToolBarControl(m_pTBGridView);

	geToolBarSeperator* seperator3 = new geToolBarSeperator(getToolBar(), 40);
	getToolBar()->appendToolBarControl(seperator3);

	//play-pause-stop
	m_pPlayButton=new geToolBarButton("play", getToolBar());
	m_pPlayButton->loadImage("res//icons16x16.png", 26, 216);
	m_pPlayButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pPlayButton);
	m_pPauseButton=new geToolBarButton("pause", getToolBar());
	m_pPauseButton->loadImage("res//icons16x16.png", 90, 216);
	m_pPauseButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pPauseButton);
	//

	//create grid
	float startX=-500.0f;
	float startY=-500.0f;

	float oneGridSz=abs(2*startX)/10.0f;
	float oneSubGridSz=oneGridSz/10.0f;
	float halfoneSubGridSz=0;//oneSubGridSz*0.5f;

	m_cGridOuterBox[0].set(startX, startY);
	m_cGridOuterBox[1].set(-startX, startY);
	m_cGridOuterBox[2].set(-startX, -startY);
	m_cGridOuterBox[3].set(startX, -startY);

	for(int x=0,cntrY=0, cntrX=0;x<10;x++)
	{
		if(x<9)
		{
			m_cThickGridOnYAxis[x*2+0].x=startX+halfoneSubGridSz+(x+1)*oneGridSz;
			m_cThickGridOnYAxis[x*2+0].y=startY;

			m_cThickGridOnYAxis[x*2+1].x=startX+halfoneSubGridSz+(x+1)*oneGridSz;
			m_cThickGridOnYAxis[x*2+1].y=-startY;

			m_cThickGridOnXAxis[x*2+0].x=startX;
			m_cThickGridOnXAxis[x*2+0].y=startY+halfoneSubGridSz+(x+1)*oneGridSz;

			m_cThickGridOnXAxis[x*2+1].x=-startX;
			m_cThickGridOnXAxis[x*2+1].y=startY+halfoneSubGridSz+(x+1)*oneGridSz;
		}

		for(int y=0;y<9;y++)
		{
			m_cGridOnYAxis[cntrY].x=startX+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
			m_cGridOnYAxis[cntrY++].y=startY;

			m_cGridOnYAxis[cntrY].x=startX+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
			m_cGridOnYAxis[cntrY++].y=-startY;

			m_cGridOnXAxis[cntrX].x=startX;
			m_cGridOnXAxis[cntrX++].y=startY+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;

			m_cGridOnXAxis[cntrX].x=-startX;
			m_cGridOnXAxis[cntrX++].y=startY+oneSubGridSz+x*oneGridSz+y*oneSubGridSz;
		}
	}
	//
}


	vector3f minV;
	vector3f maxV;
			vector3f i1, i2, testSpehere;
void gearSceneWorldEditor::draw()
{
	drawTitleAndToolBar();

	monoWrapper::mono_engine_resize(m_pMainWorldPtr, m_cPos.x+getIamOnLayout()->getPos().x, (rendererGL10::g_pRendererGL10->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/);

	//glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (rendererGL10::g_pRendererGL10->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x/*+2.0f*/, m_cSize.y-getTopMarginOffsetHeight()/**//*+2.0f*/);	
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	////gluOrtho2D((int)0, (int)(m_cSize.x/*+2.0f*/), (int)(m_cSize.y-getTopMarginOffsetHeight()/*+2.0f*/), (int)0);
	//gluPerspective(45, m_cSize.x/m_cSize.y, 1.0f, 1000.0f);
	//glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = {-1, 1, 1, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	
	float af=m_pHorizontalSlider_LightAmbient->getSliderValue();//*0.5f;

	float colorWhite[]  = {1.0f,1.0f,1.0f,1.0f};
	float ambient[]   = {af,af,af,1.0f};//{af,af,af,1.0f};
	float diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f};
	//float diffuse[]   = {0.38f, 0.38f, 0.6f,1.0f};
	//float diffuse[]   = {0.6f, 0.6f, 0.9f,1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse );


	glPushMatrix();
	onDraw();
	glPopMatrix();


	//STATS
	glViewport(m_cPos.x+getIamOnLayout()->getPos().x, (rendererGL10::g_pRendererGL10->getViewPortSz().y)-(m_cPos.y+getIamOnLayout()->getPos().y+m_cSize.y), m_cSize.x, m_cSize.y-getTopMarginOffsetHeight());	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D((int)0, (int)(m_cSize.x), (int)(m_cSize.y-getTopMarginOffsetHeight()), (int)0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, -1);
	char buffer[16];
	sprintf(buffer, "FPS : %3.2f", Timer::getFPS());
	glDisable(GL_DEPTH_TEST);
	geGUIManager::g_pFontArial12Ptr->drawString(buffer, 0, 0+geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);

	//m_pHorizontalSlider_LightAmbient->draw();

	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	//
}

void gearSceneWorldEditor::onDraw()
{
	monoWrapper::mono_engine_update(m_pMainWorldPtr, Timer::getDtinSec());
	if(m_pPlayButton->isButtonPressed())
	{
		monoWrapper::mono_game_run(Timer::getDtinSec());
	}
	monoWrapper::mono_engine_render(m_pMainWorldPtr);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	//grid
	if(m_pTBGridView->isButtonPressed())
	{
		glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &m_cGridOnYAxis[0].x);
		glDrawArrays(GL_LINES, 0, 90*2);
		glVertexPointer(2, GL_FLOAT, 0, &m_cGridOnXAxis[0].x);
		glDrawArrays(GL_LINES, 0, 90*2);
	
		glColor4f(0.1f, 0.1f, 0.1f, 1.0f);

		glVertexPointer(2, GL_FLOAT, 0, &m_cThickGridOnYAxis[0].x);
		glDrawArrays(GL_LINES, 0, 9*2);
		glVertexPointer(2, GL_FLOAT, 0, &m_cThickGridOnXAxis[0].x);
		glDrawArrays(GL_LINES, 0, 9*2);

		glVertexPointer(2, GL_FLOAT, 0, &m_cGridOuterBox[0].x);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	//

	if(m_pSelectedObj)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		
		glEnable(GL_COLOR_MATERIAL);
		float distance_frm_cam=(m_pSelectedObj->getWorldMatrix()->getPosition()-m_pMainWorldPtr->getActiveCamera()->getWorldMatrix()->getPosition()).length();
		if(distance_frm_cam<12.0f)
			distance_frm_cam=1.0f;
		else
			distance_frm_cam/=12.0f;

		//matrix4x4f localTM(*m_pSelectedObj->getWorldMatrix());
		//vector3f localScale(localTM.getScale());
		//localScale.normalize();
		//localTM.setScale(localScale);
		glPushMatrix();
		if(m_bTransformThroughLocalAxis)
			glMultMatrixf(m_pSelectedObj->getWorldMatrix()->getMatrix());
		else
			glTranslatef(m_pSelectedObj->getWorldMatrix()->getMatrix()[12], m_pSelectedObj->getWorldMatrix()->getMatrix()[13], m_pSelectedObj->getWorldMatrix()->getMatrix()[14]);
			geUtil::drawGizmo(distance_frm_cam, m_iAxisSelected);
			glColor4f(0.25f, 0.4f, 0.62f, 1);
			m_pSelectedObj->getAABB().draw();
		glPopMatrix();

		//glDisable(GL_COLOR_MATERIAL);

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}


	//geUtil::drawGizmo(3.0f);
	//glDisable(GL_COLOR_MATERIAL);
}

void gearSceneWorldEditor::preWorldRender()
{

}

void gearSceneWorldEditor::postWorldRender()
{
	//glBegin(GL_LINES);
	//glVertex3fv(&minV.x);
	//glVertex3fv(&maxV.x);
	//glEnd();

	//glDisable(GL_DEPTH_TEST);
	//glPushMatrix();
	//glTranslatef(i1.x, i1.y, i1.z);
	////glColor4f(1, 0 , 0, 1);
	//glutWireSphere(0.1f, 3, 3);
	//glPopMatrix();
	//glPushMatrix();
	//glTranslatef(testSpehere.x, testSpehere.y, testSpehere.z);
	//glColor4f(1, 0 , 0, 1);
	//glutWireSphere(0.1f, 3, 3);
	//glPopMatrix();
	//glEnable(GL_DEPTH_TEST);
}

void gearSceneWorldEditor::onSize(float cx, float cy, int flag)
{
	//if(m_pHorizontalSlider_LightAmbient)
	//	m_pHorizontalSlider_LightAmbient->setPos(cx-100, 30);
	geWindow::onSize(cx, cy, flag);
}

static bool intersectionOfLineSegmentAndPlane(vector3f p1, vector3f p2, vector3f planeNormal, vector3f pointOnPlane, float& u)
{
	vector3f p3_p1(pointOnPlane-p1);
	vector3f p2_p1(p2-p1);

	float n = planeNormal.dot(p3_p1);
	float d = planeNormal.dot(p2_p1);

	if(abs(d)<0.00000001f)
		return false;		//line parallel to plane

	u=n/d;
	return true;
}

bool gearSceneWorldEditor::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		m_iAxisSelected=-1;
		return true;
	}
	//monoWrapper::mono_engine_mouseLButtonDown(m_pMainWorldPtr, x, y, nFlag);
	if(!m_pMainWorldPtr) return true;

	GLdouble viewTM[16];
	GLdouble projectionTM[16];
	for(int m=0;m<16;m++)
	{
		viewTM[m]=m_pMainWorldPtr->getActiveCamera()->getViewMatrix()->getMatrix()[m];
		projectionTM[m]=m_pMainWorldPtr->getActiveCamera()->getProjectionMatrix()->getMatrix()[m];
	}

	GLdouble x1, y1, z1, depth;
	GLdouble x2, y2, z2;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &depth);
	z1=z2=depth;

	gxRectf viewPortRect=m_pMainWorldPtr->getRenderer()->getViewPortRect();
	GLint viewport[4]={viewPortRect.m_pos.x, viewPortRect.m_pos.y, viewPortRect.m_size.x, viewPortRect.m_size.y}; 
	//float ptY=Scene::getCommonData()->getScreenHeight();
	gluUnProject(x, viewPortRect.m_size.y - y, 0, viewTM, projectionTM, viewport, &x1, &y1, &z1);
	gluUnProject(x, viewPortRect.m_size.y - y, 1, viewTM, projectionTM, viewport, &x2, &y2, &z2);
	
	minV.set(x1, y1, z1);
	maxV.set(x2, y2, z2);

	m_iAxisSelected=0;
	if(m_pSelectedObj)
	{
		float distance_frm_cam=(m_pSelectedObj->getWorldMatrix()->getPosition()-m_pMainWorldPtr->getActiveCamera()->getWorldMatrix()->getPosition()).length();
		if(distance_frm_cam<12.0f)
			distance_frm_cam=1.0f;
		else
			distance_frm_cam/=12.0f;

		//x-gizmo
		vector3f gizmo_origin(m_pSelectedObj->getWorldMatrix()->getPosition());
		vector3f xgizmo_max(distance_frm_cam, 0, 0);
		vector3f ygizmo_max(0, distance_frm_cam, 0);
		vector3f zgizmo_max(0, 0, distance_frm_cam);

		if(m_bTransformThroughLocalAxis)
		{
			xgizmo_max=*m_pSelectedObj->getWorldMatrix() * xgizmo_max ;
			ygizmo_max=*m_pSelectedObj->getWorldMatrix() * ygizmo_max ;
			zgizmo_max=*m_pSelectedObj->getWorldMatrix() * zgizmo_max ;
		}
		else
		{
			xgizmo_max=xgizmo_max+gizmo_origin;
			ygizmo_max=ygizmo_max+gizmo_origin;
			zgizmo_max=zgizmo_max+gizmo_origin;
		}

		float ua, ub;
		//x-axis
		if(gxUtil::twoLineSegmentIntersection(minV, maxV, gizmo_origin, xgizmo_max, i1, i2, ua, ub))
		{
			if(ub>=0.0f && ub<=1.0f)
			{
				if((i1-i2).length()<=distance_frm_cam*0.06f)
				{
					testSpehere=i2;
					m_iAxisSelected=1;
					float pu;
					vector3f plane_normal((m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getZAxis():vector3f(0, 0, 1));
					if(intersectionOfLineSegmentAndPlane(minV, maxV, plane_normal, m_pSelectedObj->getWorldMatrix()->getPosition(), pu))
					{
						//vector3f 
						m_cMousePrevPosInWorld = minV+(maxV-minV)*pu;
					}
				}
			}
		}
		if(gxUtil::twoLineSegmentIntersection(minV, maxV, gizmo_origin, ygizmo_max, i1, i2, ua, ub))
		{
			if(ub>=0.0f && ub<=1.0f)
			{
				if((i1-i2).length()<=distance_frm_cam*0.06f)
				{
					testSpehere=i2;
					m_iAxisSelected=2;
					float pu;
					vector3f plane_normal((m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getZAxis():vector3f(0, 0, 1));
					if(intersectionOfLineSegmentAndPlane(minV, maxV, plane_normal, m_pSelectedObj->getWorldMatrix()->getPosition(), pu))
					{
						//vector3f 
						m_cMousePrevPosInWorld = minV+(maxV-minV)*pu;
					}
				}
			}
		}
		if(gxUtil::twoLineSegmentIntersection(minV, maxV, gizmo_origin, zgizmo_max, i1, i2, ua, ub))
		{
			if(ub>=0.0f && ub<=1.0f)
			{
				if((i1-i2).length()<=distance_frm_cam*0.06f)
				{
					testSpehere=i2;
					m_iAxisSelected=3;
					float pu;
					vector3f plane_normal((m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getYAxis():vector3f(0, 1, 0));
					if(intersectionOfLineSegmentAndPlane(minV, maxV, plane_normal, m_pSelectedObj->getWorldMatrix()->getPosition(), pu))
					{
						//vector3f 
						m_cMousePrevPosInWorld = minV+(maxV-minV)*pu;
					}
				}
			}
		}
	}

	m_cMousePrevPos.set(x, y);
	return true;
}

bool gearSceneWorldEditor::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		return true;
	}

	m_iAxisSelected=-1;
	monoWrapper::mono_engine_mouseLButtonUp(m_pMainWorldPtr, x, y, nFlag);
	m_cMousePrevPos.set(x, y);
	return true;
}

bool gearSceneWorldEditor::onMouseRButtonDown(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		m_iAxisSelected=-1;
		return true;
	}
	monoWrapper::mono_engine_mouseRButtonDown(m_pMainWorldPtr, x, y, nFlag);
	m_cMousePrevPos.set(x, y);
	return true;
}

void gearSceneWorldEditor::onMouseRButtonUp(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
	{
		m_iAxisSelected=-1;
		return;
	}
	monoWrapper::mono_engine_mouseRButtonUp(m_pMainWorldPtr, x, y, nFlag);
	m_cMousePrevPos.set(x, y);
}

bool gearSceneWorldEditor::onMouseMove(float x, float y, int flag)
{
	if(!isPointInsideClientArea(x, y)) return false;
	//monoWrapper::mono_engine_mouseMove(m_pMainWorldPtr, x, y, flag);
	Camera* camera=m_pMainWorldPtr->getActiveCamera();
	
	float dx	= (x-m_cMousePrevPos.x);
	float dy	= (y-m_cMousePrevPos.y);

	if(flag&MK_MBUTTON)
	{
		float d=camera->getPosition().length();
		if(flag&MK_SHIFT)
			camera->updateLocalPositionf(-(d/5000.0f)*dx, (d/5000.0f)*dy, 0);
		else
			camera->updateLocalPositionf(-(d/500.0f)*dx, (d/500.0f)*dy, 0);
	}
	else if(flag&MK_RBUTTON /*&& flag&MK_CONTROL*/)
	{

		vector3f aUP(0, 0, 1);
		//aUP=camera->getYAxis();
		vector3f aVect(0, 0, 0);
		if(m_pSelectedObj)
			aVect=m_pSelectedObj->getWorldMatrix()->getPosition();	//can modify this later to rotate around mesh center

		camera->rotateArb(-0.5f*dx, &aUP.x, aVect);
		vector3f left=camera->getXAxis();
		camera->rotateArb(-0.5f*dy, &left.x, aVect);
	}
	else if(flag&MK_LBUTTON)
	{
		if(m_pSelectedObj && m_iAxisSelected>0)
		{
			float d=camera->getPosition().length();
					GLdouble viewTM[16];
					GLdouble projectionTM[16];
					for(int m=0;m<16;m++)
					{
						viewTM[m]=m_pMainWorldPtr->getActiveCamera()->getViewMatrix()->getMatrix()[m];
						projectionTM[m]=m_pMainWorldPtr->getActiveCamera()->getProjectionMatrix()->getMatrix()[m];
					}

					GLdouble x1, y1, z1, depth;
					GLdouble x2, y2, z2;
					glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &depth);
					z1=z2=depth;

					gxRectf viewPortRect=m_pMainWorldPtr->getRenderer()->getViewPortRect();
					GLint viewport[4]={viewPortRect.m_pos.x, viewPortRect.m_pos.y, viewPortRect.m_size.x, viewPortRect.m_size.y}; 
					//float ptY=Scene::getCommonData()->getScreenHeight();
					gluUnProject(x, viewPortRect.m_size.y - y, 0, viewTM, projectionTM, viewport, &x1, &y1, &z1);
					gluUnProject(x, viewPortRect.m_size.y - y, 1, viewTM, projectionTM, viewport, &x2, &y2, &z2);

					minV.set(x1, y1, z1);
					maxV.set(x2, y2, z2);

					vector3f planeNormalArray[3]={

						(m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getZAxis():vector3f(0, 0, 1),
						(m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getZAxis():vector3f(0, 0, 1),
						(m_bTransformThroughLocalAxis)?m_pSelectedObj->getWorldMatrix()->getYAxis():vector3f(0, 1, 0)
					};

					float pu;
					if(intersectionOfLineSegmentAndPlane(minV, maxV, planeNormalArray[m_iAxisSelected-1], m_pSelectedObj->getWorldMatrix()->getPosition(), pu))
					{
						vector3f current_pos_in_world(minV+(maxV-minV)*pu);
						vector3f diff_in_world(current_pos_in_world-m_cMousePrevPosInWorld);
						vector3f transformed;
						transformed=(m_bTransformThroughLocalAxis)?(m_pSelectedObj->getWorldMatrix()->getInverse() % diff_in_world):m_pSelectedObj->getWorldMatrix()->getPosition() + diff_in_world;
						//transformed=m_pSelectedObj->getWorldMatrix()->getInverse() % diff_in_world;

						if(m_bTransformThroughLocalAxis)
						{
							if(m_iAxisSelected==1)
								m_pSelectedObj->updateLocalPositionf(transformed.x, 0, 0);
							else if(m_iAxisSelected==2)
								m_pSelectedObj->updateLocalPositionf(0, transformed.y, 0);
							else if(m_iAxisSelected==3)
								m_pSelectedObj->updateLocalPositionf(0, 0, transformed.z);
						}
						else
						{
							vector3f right(1, 0, 0);
							right=right*diff_in_world.x;
							if(m_iAxisSelected==1)
								m_pSelectedObj->updatePositionf(right.x, 0, 0);
							else if(m_iAxisSelected==2)
								m_pSelectedObj->updatePositionf(0, diff_in_world.y, 0);
							else if(m_iAxisSelected==3)
								m_pSelectedObj->updatePositionf(0, 0, diff_in_world.z);
						}

						m_cMousePrevPosInWorld = current_pos_in_world;
					}
		}
	}

	//DEBUG_PRINT("%f, %f\n", delta.x, delta.y);
	m_cMousePrevPos.set(x, y);

	return true;
}

void gearSceneWorldEditor::onMouseWheel(int zDelta, int x, int y, int flag)
{
	if(!isPointInsideClientArea(x, y)) return;
	//monoWrapper::mono_engine_mouseWheel(m_pMainWorldPtr, zDelta, x, y, flag);
	Camera* camera=m_pMainWorldPtr->getActiveCamera();
	int dir = (zDelta<0)?1:-1;
	vector3f aCamForwardDir(camera->getZAxis());
	float d=camera->getPosition().length();
	float factor=20.0f;
	//if(nFlags&MK_SHIFT)
	//	factor=500.0f;
	aCamForwardDir.x=aCamForwardDir.x*(d/factor)*(dir);
	aCamForwardDir.y=aCamForwardDir.y*(d/factor)*(dir);
	aCamForwardDir.z=aCamForwardDir.z*(d/factor)*(dir);
	camera->updatePositionf(aCamForwardDir.x, aCamForwardDir.y, aCamForwardDir.z);
}

void gearSceneWorldEditor::onButtonClicked(geGUIBase* btn)
{
	if(m_pLocalOrGlobalAxis==btn)
	{
		if(strcmp(m_pLocalOrGlobalAxis->getName(), "Local")==0)
		{
			m_pLocalOrGlobalAxis->setName("Global");
			m_bTransformThroughLocalAxis=false;
		}
		else
		{
			m_pLocalOrGlobalAxis->setName("Local");
			m_bTransformThroughLocalAxis=true;
		}
		m_pLocalOrGlobalAxis->buttonNormal();
	}
	else if(m_pPlayButton==btn)
	{
		monoWrapper::mono_game_start();
	}
}

void gearSceneWorldEditor::onSliderChange(geGUIBase* slider)
{

}