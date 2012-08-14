
// PrintExampleView.cpp : implementation of the CPrintExampleView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PrintExample.h"
#endif

#include "PrintExampleDoc.h"
#include "PrintExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "..\\printing\\BitmapTableUnit.h"

#pragma comment(lib, "..\\debug\\PrintingDll.lib")

// CPrintExampleView

IMPLEMENT_DYNCREATE(CPrintExampleView, CView)

BEGIN_MESSAGE_MAP(CPrintExampleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPrintExampleView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPrintExampleView construction/destruction

CPrintExampleView::CPrintExampleView()
{
	// TODO: add construction code here

}

CPrintExampleView::~CPrintExampleView()
{
}

BOOL CPrintExampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPrintExampleView drawing

void CPrintExampleView::OnDraw(CDC* /*pDC*/)
{
	CPrintExampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPrintExampleView printing


void CPrintExampleView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

void LoadPictureFile(LPCTSTR szFile, CBitmap* pBitmap, CSize& mSize)
{
	// open file
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	_ASSERTE(INVALID_HANDLE_VALUE != hFile);

	// get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	_ASSERTE(-1 != dwFileSize);

	LPVOID pvData = NULL;
	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	DWORD dwBytesRead = 0;
	// read file and store in global memory
	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
	_ASSERTE(FALSE != bRead);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	LPSTREAM pstm = NULL;
	// create IStream* from global memory
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	_ASSERTE(SUCCEEDED(hr) && pstm);

	// Create IPicture from image file
	LPPICTURE gpPicture;

	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture);
	_ASSERTE(SUCCEEDED(hr) && gpPicture); 
	pstm->Release();

	OLE_HANDLE m_picHandle;
	/*
	long hmWidth, hmHeight;
	gpPicture->get_Width(&hmWidth);
	gpPicture->get_Height(&hmHeight);
	int nWidth = MulDiv(hmWidth, GetDeviceCaps(hdc, LOGPIXELSX), HIMETRIC_INCH);
	int nHeight = MulDiv(hmHeight, GetDeviceCaps(hdc, LOGPIXELSY), HIMETRIC_INCH);
	*/
	gpPicture->get_Handle(&m_picHandle);
	pBitmap->DeleteObject();
	pBitmap->Attach((HGDIOBJ) m_picHandle);

	BITMAP bm;
	GetObject(pBitmap->m_hObject, sizeof(bm), &bm);
	mSize.cx = bm.bmWidth; //nWidth;
	mSize.cy = bm.bmHeight; //nHeight;
}

BOOL CPrintExampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	//////////// test 4 : self-define page ////////////////////////////
	CBitmapTableUnit* unitBitmapTable = new CBitmapTableUnit;
	unitBitmapTable->SetTitle(L"²âÊÔ");
	// add the margin around the title
	unitBitmapTable->SetTitleMargin(10);
	unitBitmapTable->SetTitlePen(140, L"¿¬Ìå");
	unitBitmapTable->NeedPrintTitleExcpetFirstPage(true);

	// set data
	CBitmap bmp;
	CSize mSize;
	LoadPictureFile(L"C:\\Users\\aico\\Desktop\\1.jpg", &bmp, mSize);

	vector<vector<CBitmap* > > vecBmp;
	int row = 10;
	int columns = 3;
	vecBmp.resize(row);
	for (int i = 0; i < row; i++)
	{
		vecBmp[i].resize(columns);
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < vecBmp[i].size(); j++)
		{
			vecBmp[i][j] = &bmp;
		}
	}
	unitBitmapTable->SetPrintData(&vecBmp);
	// row in each page does not affect the result	
	unitBitmapTable->SetRowsInEachPage(4);

	m_printJob.InsertTask(unitBitmapTable);

	m_printJob.PrintFollowingPrintDialog();
	return DoPreparePrinting(pInfo);
}

void CPrintExampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPrintExampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPrintExampleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPrintExampleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPrintExampleView diagnostics

#ifdef _DEBUG
void CPrintExampleView::AssertValid() const
{
	CView::AssertValid();
}

void CPrintExampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPrintExampleDoc* CPrintExampleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPrintExampleDoc)));
	return (CPrintExampleDoc*)m_pDocument;
}
#endif //_DEBUG


// CPrintExampleView message handlers
