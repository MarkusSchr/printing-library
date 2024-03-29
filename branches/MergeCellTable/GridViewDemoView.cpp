// GridViewDemoView.cpp : implementation of the CGridViewDemoView class
//

#include "stdafx.h"
#include "GridViewDemo.h"

#include "GridViewDemoDoc.h"
#include "GridViewDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridViewDemoView

IMPLEMENT_DYNCREATE(CGridViewDemoView, CView)

BEGIN_MESSAGE_MAP(CGridViewDemoView, CView)
	//{{AFX_MSG_MAP(CGridViewDemoView)
	ON_WM_SIZE()
    ON_COMMAND(ID_TOGGLE_READONLY, OnToggleReadonly)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MergeCell, OnMergeCell)
	ON_COMMAND(ID_CancelMergeCell, OnCancelMergeCell)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridViewDemoView construction/destruction

CGridViewDemoView::CGridViewDemoView()
{
	m_pGridCtrl = NULL;
}

CGridViewDemoView::~CGridViewDemoView()
{
	if (m_pGridCtrl)
		delete m_pGridCtrl;
}

BOOL CGridViewDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGridViewDemoView drawing

void CGridViewDemoView::OnDraw(CDC* pDC)
{
	CGridViewDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGridViewDemoView printing

BOOL CGridViewDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGridViewDemoView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnBeginPrinting(pDC, pInfo);
}

void CGridViewDemoView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnPrint(pDC, pInfo);
}

void CGridViewDemoView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CGridViewDemoView diagnostics

#ifdef _DEBUG
void CGridViewDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CGridViewDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGridViewDemoDoc* CGridViewDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGridViewDemoDoc)));
	return (CGridViewDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridViewDemoView message handlers

void CGridViewDemoView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		m_pGridCtrl = new CGridCtrl;
		if (!m_pGridCtrl) return;

		// Create the Gridctrl window
		CRect rect;
		GetClientRect(rect);
		m_pGridCtrl->Create(rect, this, 100);

		// fill it up with stuff
		m_pGridCtrl->SetEditable(TRUE);
		m_pGridCtrl->EnableDragAndDrop(TRUE);

		try {
			m_pGridCtrl->SetFixedRowCount(0);
			m_pGridCtrl->SetFixedColumnCount(0);
			m_pGridCtrl->SetRowCount(6);
			m_pGridCtrl->SetColumnCount(6);
		}
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}

		// fill rows/cols with text
		
		for (int row = 0; row < m_pGridCtrl->GetRowCount(); row++)
		{
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = 0;
				if (row > 0) {
					Item.nFormat = DT_CENTER|DT_WORDBREAK|DT_VCENTER;
					Item.strText.Format(_T("%d"),row);
				}
				m_pGridCtrl->SetRowHeight(row,20);
				m_pGridCtrl->SetItem(&Item);
		}
		
		for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
		{
			if (col > 0) {
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = 0;
				Item.col = col;
				Item.nFormat = DT_CENTER|DT_WORDBREAK|DT_VCENTER;
				Item.strText.Format(_T("%c"),64+col);
				m_pGridCtrl->SetColumnWidth(col,70);
				m_pGridCtrl->SetItem(&Item);
			}
		}
		// both not the first
		m_pGridCtrl->MergeCells(CCellRange(1,1,1,2));
		m_pGridCtrl->SetItemText(1,1,_T(" 合并测试1 "));
		m_pGridCtrl->SetItemFormat(1,1,DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_SINGLELINE );

		// both are the first
		m_pGridCtrl->MergeCells(CCellRange(0,0,1,0));
		m_pGridCtrl->SetItemText(0,0,_T(" 合并测试2 "));
		m_pGridCtrl->SetItemFormat(0,0,DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_SINGLELINE );

		// column is the first
		m_pGridCtrl->MergeCells(CCellRange(0,4,0,5));
		m_pGridCtrl->SetItemText(0,4,_T(" 合并测试3 "));
		m_pGridCtrl->SetItemFormat(0,4,DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_SINGLELINE );

		// row is the first
		m_pGridCtrl->MergeCells(CCellRange(3,0,3,1));
		m_pGridCtrl->SetItemText(3,0,_T(" 合并测试4 "));
		m_pGridCtrl->SetItemFormat(3,0,DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_SINGLELINE );


/*
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 4;
		Item.col = 3;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_WORDBREAK;
		Item.strText.Format(_T(" 合并测试 "));
		m_pGridCtrl->SetItem(&Item);*/
		//m_pGridCtrl->CancelMergeCell(4,3);
/*		for (int row = 0; row < m_pGridCtrl->GetRowCount(); row++)
			for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = col;
				if (row < 1) {
					Item.nFormat = DT_LEFT|DT_WORDBREAK;
					Item.strText.Format(_T("Column %d"),col);
				} else if (col < 1) {
					Item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
					Item.strText.Format(_T("Row %d"),row);
				} else {
					Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
					Item.strText.Format(_T("%d"),row*col);
				}
				m_pGridCtrl->SetItem(&Item);
		}
*/
//		m_pGridCtrl->AutoSize();

  }
}

void CGridViewDemoView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (m_pGridCtrl->GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_pGridCtrl->MoveWindow(rect);
	}
}

BOOL CGridViewDemoView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
        if (m_pGridCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
            return TRUE;

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CGridViewDemoView::OnToggleReadonly()
{
    if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
    {
        CCellID cell = m_pGridCtrl->GetFocusCell();
        if (m_pGridCtrl->IsValid(cell))
        {
            int nState = m_pGridCtrl->GetItemState(cell.row, cell.col);
            if (m_pGridCtrl->IsCellEditable(cell))
                m_pGridCtrl->SetItemState(cell.row, cell.col, nState | GVIS_READONLY);
            else
                m_pGridCtrl->SetItemState(cell.row, cell.col, nState & ~GVIS_READONLY);
        }
    }
}

BOOL CGridViewDemoView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}

void CGridViewDemoView::OnMergeCell() 
{
	//m_pGridCtrl->OnMergeCell();
	// TODO: Add your command handler code here
	
}

void CGridViewDemoView::OnCancelMergeCell() 
{
	// TODO: Add your command handler code here
        CCellID cell = m_pGridCtrl->GetFocusCell();
        if (m_pGridCtrl->IsValid(cell))
        {
			// m_pGridCtrl->SplitCells();
		}	
}
