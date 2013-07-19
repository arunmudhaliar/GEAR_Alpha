//#include "myDLL.h"
//#include <vector>
//
//extern "C" {
//static rendererGL10* g_pRendererGL10 = NULL;
//static std::vector<LineObject*> g_vLineObjects;
//
//extern DllExport void myDLL_init(HWND hWnd)
//{
//	g_pRendererGL10 = new rendererGL10(hWnd);
//
//	g_pRendererGL10->setupRenderer();
//	g_pRendererGL10->loadDefaultRenderState();/**/
//	g_pRendererGL10->swapGLBuffer();
//}
//
//extern DllExport int myDLL_test()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	glTranslatef(0, 0, -1);
//
//	for(std::vector<LineObject*>::iterator it = g_vLineObjects.begin(); it != g_vLineObjects.end(); ++it)
//	{
//		LineObject* obj= *it;
//		glPushMatrix();
//		glMultMatrixf(obj->getMatrix());
//		glBegin(GL_LINES);
//		glVertex3f(obj->m_szLineBuffer[0], obj->m_szLineBuffer[1], 0);
//		glVertex3f(obj->m_szLineBuffer[2], obj->m_szLineBuffer[3], 0);
//		glEnd();
//		glPopMatrix();
//	}
//
//	g_pRendererGL10->swapGLBuffer();
//	return 10;
//}
//
//extern DllExport void myDLL_reset()
//{
//	if(g_pRendererGL10)
//	{
//		delete g_pRendererGL10;
//		g_pRendererGL10=NULL;
//	}
//}
//
//extern DllExport void myDLL_resize(float cx, float cy)
//{
//	if(g_pRendererGL10)
//	{
//		g_pRendererGL10->setViewPort(cx, cy);
//		g_pRendererGL10->setProjectionMatrixToGL(g_pRendererGL10->getOrthoProjectionMatrix());
//	}
//}
//
//extern DllExport LineObject* myDLL_createLine(float x1, float y1, float x2, float y2, float posx, float posy)
//{
//	LineObject* lo = new LineObject();
//	lo->setLine(x1, y1, x2, y2);
//	lo->setPosition(posx, posy, 0);
//	sprintf(lo->getName(), "line%d", g_vLineObjects.size());
//	g_vLineObjects.push_back(lo);
//
//	return lo;
//}
//
//extern DllExport void myDLL_destroyLine(LineObject* lineObject)
//{
//	std::vector<LineObject*>::iterator it;
//
//	for(it = g_vLineObjects.begin(); it != g_vLineObjects.end(); ++it)
//	{
//		if(lineObject == *it)
//		{
//			printf("%s deleted\n", lineObject->getName());
//			g_vLineObjects.erase(it);
//			delete lineObject;
//			break;
//		}
//	}
//}
//
//extern DllExport LineObject* myDLL_findLine(char* name)
//{
//	for(std::vector<LineObject*>::iterator it = g_vLineObjects.begin(); it != g_vLineObjects.end(); ++it)
//	{
//		LineObject* obj= *it;
//		if(strcmp(obj->getName(), name)==0)
//		{
//			return obj;
//		}
//	}
//
//	printf("%s not found\n", name);
//	return NULL;
//}
//
//extern DllExport void myDLL_getLine(LineObject* lineObject, float& x1, float& y1, float& x2, float& y2, float& posx, float& posy)
//{
//	x1=lineObject->m_szLineBuffer[0];
//	y1=lineObject->m_szLineBuffer[1];
//	x2=lineObject->m_szLineBuffer[2];
//	y2=lineObject->m_szLineBuffer[3];
//
//	posx=lineObject->getMatrix()[12];
//	posy=lineObject->getMatrix()[13];
//}
//
//extern DllExport void myDLL_setLine(LineObject* lineObject, float x1, float y1, float x2, float y2, float posx, float posy)
//{
//	lineObject->m_szLineBuffer[0]=x1;
//	lineObject->m_szLineBuffer[1]=y1;
//	lineObject->m_szLineBuffer[2]=x2;
//	lineObject->m_szLineBuffer[3]=y2;
//
//	lineObject->setPosition(posx, posy, 0);
//}
//
//extern DllExport void myDLL_getLineBuffer(LineObject* lineObject, float* buffer)
//{
//	buffer=lineObject->m_szLineBuffer;
//}
//
//
//}