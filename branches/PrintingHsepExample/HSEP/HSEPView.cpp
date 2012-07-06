// HSEPView.cpp : implementation of the CHSEPView class
//

#include "stdafx.h"
#include "HSEP.h"

#include "HSEPDoc.h"
#include "HSEPView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHSEPView

IMPLEMENT_DYNCREATE(CHSEPView, CView)

BEGIN_MESSAGE_MAP(CHSEPView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHSEPView construction/destruction

CHSEPView::CHSEPView():m_pPrinter()
{
	// TODO: add construction code here

}

CHSEPView::~CHSEPView()
{
	if(m_pPrinter)
		delete m_pPrinter;
}

BOOL CHSEPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CHSEPView drawing

void CHSEPView::OnDraw(CDC* /*pDC*/)
{
	CHSEPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CHSEPView diagnostics

#ifdef _DEBUG
void CHSEPView::AssertValid() const
{
	CView::AssertValid();
}

void CHSEPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHSEPDoc* CHSEPView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHSEPDoc)));
	return (CHSEPDoc*)m_pDocument;
}
#endif //_DEBUG


// CHSEPView message handlers

void CHSEPView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if(!m_pPrinter)
		m_pPrinter = new CHSEPrinter();
}


// CHSEPView printing

BOOL CHSEPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	if( m_pPrinter)
		m_pPrinter->PreparePrinting(pInfo);
	return DoPreparePrinting(pInfo);
}

void CHSEPView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if(m_pPrinter)
		m_pPrinter->BeginPrinting(pDC,pInfo);
}

void CHSEPView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	if(NULL != m_pPrinter && NULL != pInfo)
		m_pPrinter->PrepareDC(pDC,pInfo);

	CView::OnPrepareDC(pDC, pInfo);
}

void CHSEPView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if(m_pPrinter)
		m_pPrinter->Print(pDC,pInfo);

	CView::OnPrint(pDC, pInfo);
}

void CHSEPView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if(m_pPrinter)
		m_pPrinter->EndPrinting(pDC,pInfo);
}
