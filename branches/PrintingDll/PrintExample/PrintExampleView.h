
// PrintExampleView.h : interface of the CPrintExampleView class
//

#pragma once
#include "..\\Printing\\gfx_printjob.h"

class CPrintExampleView : public CView
{
protected: // create from serialization only
	CPrintExampleView();
	DECLARE_DYNCREATE(CPrintExampleView)

// Attributes
public:
	CPrintExampleDoc* GetDocument() const;

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
	virtual ~CPrintExampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	GPrintJob m_printJob;
};

#ifndef _DEBUG  // debug version in PrintExampleView.cpp
inline CPrintExampleDoc* CPrintExampleView::GetDocument() const
   { return reinterpret_cast<CPrintExampleDoc*>(m_pDocument); }
#endif

