//#ifndef MYDLL_H
//#define MYDLL_H
//
//#include "rendererGL10.h"
//#include "lineObject.h"
//
//extern "C" {
//DllExport void myDLL_init(HWND hWnd);
//DllExport int myDLL_test();
//DllExport void myDLL_reset();
//DllExport void myDLL_resize(float cx, float cy);
//
//DllExport LineObject* myDLL_createLine(float x1, float y1, float x2, float y2, float posx, float posy);
//DllExport void myDLL_destroyLine(LineObject* lineObject);
//DllExport LineObject* myDLL_findLine(char* name);
//DllExport void myDLL_getLine(LineObject* lineObject, float& x1, float& y1, float& x2, float& y2, float& posx, float& posy);
//DllExport void myDLL_setLine(LineObject* lineObject, float x1, float y1, float x2, float y2, float posx, float posy);
//DllExport void myDLL_getLineBuffer(LineObject* lineObject, float* buffer);
//
//}
//#endif