//
//  MDropSource.h
//  Copyright 2017 GEAR3D
//  Gear3D
//
//  Created by Arun Mudaliar.
//
//
#ifndef MDROPSOURCE_H
#define MDROPSOURCE_H

#include<vector>
#if !defined(__APPLE__)     // disable Drag-Drop
#include <Ole2.h>
#include "../gui/geGUIBase.h"

class MDropSource : public IDropSource {
public:
    MDropSource() {
        m_lRefCount = 1;
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */REFIID riid,
    /* [iid_is][out] */__RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject) {
        if (riid == IID_IDropTarget || riid == IID_IUnknown) {
            AddRef();
            *ppvObject = this;
            return S_OK;
        } else {
            *ppvObject = 0;
            return E_NOINTERFACE;
        }
    }

    virtual ULONG STDMETHODCALLTYPE AddRef(void) {
        return InterlockedIncrement(&m_lRefCount);
    }

    virtual ULONG STDMETHODCALLTYPE Release(void) {
        LONG count = InterlockedDecrement(&m_lRefCount);
        if (count == 0) {
            delete this;
            return 0;
        } else {
            return count;
        }
    }

    virtual HRESULT STDMETHODCALLTYPE QueryContinueDrag(__in BOOL fEscapePressed, __in DWORD grfKeyState) {
        // if the Escape key has been pressed since the last call, cancel the drop
        if (fEscapePressed == TRUE)
            return DRAGDROP_S_CANCEL;
        // if the LeftMouse button has been released, then do the drop!
        if ((grfKeyState & MK_LBUTTON) == 0)
            return DRAGDROP_S_DROP;
        // continue with the drag-drop
        return S_OK;
    }
        
    virtual HRESULT STDMETHODCALLTYPE GiveFeedback(
    /* [annotation][in] */
    __in DWORD dwEffect) {
        return DRAGDROP_S_USEDEFAULTCURSORS;
    }

private:
    // any private members and functions
    LONG m_lRefCount;
};

class geGUIBase;
class MDataObject : public IDataObject {
public:
    MDataObject(std::vector<geGUIBase*>* actualDataObjectList, geGUIBase* sourceObject) {
        m_vpActualDataObjectPtrList = actualDataObjectList;
        m_pSourceObjectPtr = sourceObject;
        m_lRefCount = 1;
    }

    ~MDataObject() {
        m_vpActualDataObjectPtrList->clear();
        GE_DELETE(m_vpActualDataObjectPtrList);
    }

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetData(
    /* [unique][in] */FORMATETC *pformatetcIn,
    /* [out] */STGMEDIUM *pmedium) {
        return S_OK;
    }

        
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetDataHere(
    /* [unique][in] */FORMATETC *pformatetc,
    /* [out][in] */STGMEDIUM *pmedium) {
        return DATA_E_FORMATETC;
    }
        
    virtual HRESULT STDMETHODCALLTYPE QueryGetData(
    /* [unique][in] */__RPC__in_opt FORMATETC *pformatetc) {
        return S_OK;
    }
        
    virtual HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(
    /* [unique][in] */ __RPC__in_opt FORMATETC *pformatectIn,
    /* [out] */ __RPC__out FORMATETC *pformatetcOut) {
        // Apparently we have to set this field to NULL even though we don't do anything else
        pformatetcOut->ptd = NULL;
        return E_NOTIMPL;
    }
        
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE SetData(
    /* [unique][in] */ FORMATETC *pformatetc,
    /* [unique][in] */ STGMEDIUM *pmedium,
    /* [in] */ BOOL fRelease) {
        return E_NOTIMPL;
    }
        
    virtual HRESULT STDMETHODCALLTYPE EnumFormatEtc(
    /* [in] */ DWORD dwDirection,
    /* [out] */ __RPC__deref_out_opt IEnumFORMATETC **ppenumFormatEtc) {
        return E_NOTIMPL;
    }
        
    virtual HRESULT STDMETHODCALLTYPE DAdvise(
    /* [in] */ __RPC__in FORMATETC *pformatetc,
    /* [in] */ DWORD advf,
    /* [unique][in] */ __RPC__in_opt IAdviseSink *pAdvSink,
    /* [out] */ __RPC__out DWORD *pdwConnection) {
        return OLE_E_ADVISENOTSUPPORTED;
    }
        
    virtual HRESULT STDMETHODCALLTYPE DUnadvise(
    /* [in] */ DWORD dwConnection) {
        return OLE_E_ADVISENOTSUPPORTED;
    }
        
    virtual HRESULT STDMETHODCALLTYPE EnumDAdvise(
    /* [out] */ __RPC__deref_out_opt IEnumSTATDATA **ppenumAdvise) {
        return OLE_E_ADVISENOTSUPPORTED;
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject) {
        // check to see what interface has been requested
        if (riid == IID_IDataObject || riid == IID_IUnknown) {
            AddRef();
            *ppvObject = this;
            return S_OK;
        } else {
            *ppvObject = 0;
            return E_NOINTERFACE;
        }
    }

    virtual ULONG STDMETHODCALLTYPE AddRef(void) {
        return InterlockedIncrement(&m_lRefCount);
    }

    virtual ULONG STDMETHODCALLTYPE Release(void) {
        LONG count = InterlockedDecrement(&m_lRefCount);
        
        if (count == 0) {
            delete this;
            return 0;
        } else {
            return count;
        }
    }

    std::vector<geGUIBase*>* getActualDataList()    {   return m_vpActualDataObjectPtrList; }
    geGUIBase* getSourcePtr()                       {   return m_pSourceObjectPtr;          }

    private:
    // any private members and functions
    LONG m_lRefCount;
    // geGUIBase* m_pActualDataObjectPtr;
    geGUIBase* m_pSourceObjectPtr;
    std::vector<geGUIBase*>* m_vpActualDataObjectPtrList;
};
#endif
#endif
