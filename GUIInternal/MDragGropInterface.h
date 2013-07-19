#ifndef MDRAGDROPINTERFACE_H
#define MDRAGDROPINTERFACE_H

#include <Windows.h>
#include "GEAREditor\EditorApp.h"

class MDragDropInterface : public IDropTarget
{
public:
	MDragDropInterface(EditorApp* pEditorAppPtr)
	{
		m_pEditorAppPtr=pEditorAppPtr;
		m_lRefCount=1;
	}

	virtual HRESULT STDMETHODCALLTYPE DragEnter( 
	/* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD *pdwEffect)
	{
		POINT temp;
		temp.x=pt.x;
		temp.y=pt.y;

		ScreenToClient(EditorApp::getMainWindowHandle(), &temp);
		m_pEditorAppPtr->DragEnter(temp.x, temp.y);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE DragOver( 
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD *pdwEffect)
	{
		return S_OK;
	}
        
	virtual HRESULT STDMETHODCALLTYPE DragLeave( void)
	{
		m_pEditorAppPtr->DragLeave();
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Drop( 
	/* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD *pdwEffect)
	{
		POINT temp;
		temp.x=pt.x;
		temp.y=pt.y;
		ScreenToClient(EditorApp::getMainWindowHandle(), &temp);

		m_pEditorAppPtr->DragDrop(temp.x, temp.y, (MDataObject*)pDataObj);
		return S_OK;
	}

	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface(REFIID iid, void ** ppvObject)
	{
		if(iid == IID_IDropTarget || iid == IID_IUnknown)
		{
			AddRef();
			*ppvObject = this;
			return S_OK;
		}
		else
		{
			*ppvObject = 0;
			return E_NOINTERFACE;
		}
	}

	virtual ULONG	STDMETHODCALLTYPE	AddRef(void)
	{
		return InterlockedIncrement(&m_lRefCount);
	}

	virtual ULONG	STDMETHODCALLTYPE	Release(void)
	{
		LONG count = InterlockedDecrement(&m_lRefCount);
		
		if(count == 0)
		{
			delete this;
			return 0;
		}
		else
		{
			return count;
		}
	}

private:
	LONG m_lRefCount;
	EditorApp* m_pEditorAppPtr;		//must not delete this pointer
};
#endif