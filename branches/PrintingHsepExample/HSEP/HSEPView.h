// HSEPView.h : interface of the CHSEPView class
//

#include "..\Print\HSEPrinter.h"

#pragma once


class CHSEPView : public CView
{
protected: // create from serialization only
	CHSEPView();
	DECLARE_DYNCREATE(CHSEPView)

	CHSEPrinter*	m_pPrinter;		



// Attributes
public:
	CHSEPDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CHSEPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in HSEPView.cpp
inline CHSEPDoc* CHSEPView::GetDocument() const
   { return reinterpret_cast<CHSEPDoc*>(m_pDocument); }
#endif

