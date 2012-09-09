// printDlg.cpp : implementation file
//

#include "stdafx.h"
#include "print.h"
#include "printDlg.h"

#include <sstream>

#include "gfx_printjob.h"
#include "DataTableUnit.h"
#include "BitmapTableUnit.h"
#include "MergableTableUnit.h"
#include "MultiTablesUnit.h"

#include "HeaderFooterTable.h"
////////////////////////////////////
#include "PrintUnitFromDC.h"
////////////////////////////////////

#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg dialog

CPrintDlg::CPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_ROWS, m_editRows);
	DDX_Control(pDX, IDC_EDIT_COLUMNS, m_editColumns);
}

BEGIN_MESSAGE_MAP(CPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPrintDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg message handlers

BOOL CPrintDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPrintDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPrintDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPrintDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class MyPrintJob : public GPrintJob
{
public:
   MyPrintJob() 
   {
   }
   virtual ~MyPrintJob() 
   {
   }
};

///////////////////////////////////////////
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



void CPrintDlg::OnOK() 
{
	// get data
	CString   strRow; 
	m_editRows.GetWindowText(strRow); 
	int rowNum = _ttoi(strRow);
	if (rowNum == 0)
	{
		rowNum = 50;
	}

	CString   strColumns; 
	m_editColumns.GetWindowText(strColumns); 
	int columnNum = _ttoi(strColumns);
	if (columnNum == 0)
	{
		columnNum = 5;
	}

	ASSERT(rowNum != 0 && columnNum != 0);


	// prepare the column
	vector<COLUMNDEFINITIONS> vecColumnDef;
	// define my four columns...
	for (int i = 0; i < columnNum; i++)
	{
		COLUMNDEFINITIONS cd;
		TCHAR buf[200];
		_itow_s(i, buf, 10);
		wstring str = buf;
		str.append(TEXT("����"));
		cd.strName = str.c_str();

		vecColumnDef.push_back(cd);
	}

	// prepare the data
	wstring **strArr = NULL;
	strArr = new wstring* [rowNum];
	for (int i = 0; i < rowNum; i++)
	{
		strArr[i] = new wstring [columnNum];
	}

	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < columnNum; j++)
		{
			srand((unsigned)time(0));
			int randomPart1 = rand() % 10000;
			int randomPart2 = rand() % 1000;

			TCHAR c = 0;
			if (rand() % 2)
			{
				c = TEXT('+');
			}
			else
			{
				c = TEXT('-');
			}
			std::wstringstream ss;
			ss << c << randomPart1 << TEXT(".") << randomPart2 << TEXT('\0');
			strArr[i][j] = ss.str(); 

			wstring temp = strArr[i][j];
		}
	}

	vector<vector<LPCTSTR> > vecParts;

	for (int i = 0; i < rowNum ; i++)
	{
		vector<LPCTSTR> vecTemp;

		for (int j = 0; j < columnNum ; j++)
		{
			vecTemp.push_back(const_cast<LPCTSTR>(strArr[i][j].c_str()));
		}

		vecParts.push_back(vecTemp);
	}

	/////////////////////////////////////////////////////////////////////////////////
	GPrintJob job;
	
	CHeaderFooterTable header(&job);
	header.SetRowColumnNum(3, 5);

	for (int i = 0; i < 3; i++)
	{
		header.MergeRows(i,0,i,1);
		header.MergeRows(i,2,i,4);

		header.SetRowUnitFormat(i, 0, DT_LEFT | DT_SINGLELINE | DT_VCENTER );
		header.SetRowUnitFormat(i, 2, DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	}
	header.SetRowUnitText(0,0,L"abc��һ��:");
	header.SetRowUnitText(0,2,L"Hello world ������磡123 :");
	header.SetRowUnitText(1,0,L"No 2 :");
	header.SetRowUnitText(1,2,L"123 :");
	header.SetRowUnitText(2,0,L"No 3 :");
	header.SetRowUnitText(2,2,L"Hello world 123 :");

	header.SetRowUnitFont(0,0,60,L"����");
	header.SetRowUnitFont(0,2,20,L"����");
	header.SetRowUnitFont(1,0,100,L"����");
	header.SetRowUnitFont(1,2,100,L"����");

	//header.NeedOuterLine(false);

	// TODO : To add footer
	CHeaderFooterTable footer(&job);
	
	CDataTableUnit unitTable1;
	unitTable1.DefineColumns(vecColumnDef);
	unitTable1.SetPrintData(&vecParts);

	// currently it only support for DT_LEFT, DT_CENTER or DT_RIGHT
	unitTable1.SetRowFormat(DT_RIGHT);

	// you do  not need to set the row height. However, if you have set, 
	// the pre-calculation process will be much faster than it does not.
	unitTable1.SetAllRowsHeightInTextLine(1);

	// create a font that is 90�����塱for heading
	unitTable1.SetHeaderFont(100, L"����");
	unitTable1.SetFooterFont(70, L"����");
	unitTable1.SetBodyPrinterFont(90, L"����");

	// draw header
	unitTable1.SetHeader(&header);

	// draw footer
	//unitTable1.SetFooter(&footer);

	unitTable1.SetSeparateLineInterval(10);
	unitTable1.SetSeparateLineWidth(3);

	unitTable1.SetTitle(L"����1");
	unitTable1.SetEnglishVersion(true);
	unitTable1.NeedPrintTitleExcpetFirstPage(true);
	

	CDataTableUnit unitTable2;
	unitTable2.SetTitle(L"����2");
	unitTable2.DefineColumns(vecColumnDef);
	unitTable2.SetPrintData(&vecParts);
	//unitTable2.SetHeader(footer, 3);
	unitTable2.SetAllRowsHeightInTextLine(1);
	

	COLUMNDEFINITIONS cd;
	TCHAR buf[200];
	_itow_s(100, buf, 10);
	wstring str = buf;
	str.append(TEXT("����"));
	cd.strName = str.c_str();
	vecColumnDef.erase(vecColumnDef.end() - 1);
	vecColumnDef.push_back(cd);

	//////////// test 4 : self-define page ////////////////////////////
	CBitmapTableUnit unitBitmapTable;
	unitBitmapTable.SetTitle(L"����");
	// add the margin around the title
	unitBitmapTable.SetTitleMargin(2);
	unitBitmapTable.SetTitlePen(140, L"����");
	unitBitmapTable.NeedPrintTitleExcpetFirstPage(true);

	// ability inherited from the base class
	//unitBitmapTable.SetHeader(&header);
	//unitBitmapTable.SetFooter(footer, 3);

	// set data
	CBitmap bmp;
	CSize mSize;
	LoadPictureFile(L"C:\\Users\\aico\\Desktop\\1.jpg", &bmp, mSize);

	//
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
	unitBitmapTable.SetPrintData(&vecBmp);
	// row in each page does not affect the result	
	unitBitmapTable.SetRowsInEachPage(4);
	


	//////////// test 3 : self-define page ////////////////////////////
	CPrintUnitFromDC userDefinedUnit;
//	userDefinedUnit.SetFooter(footer, 3);
//	userDefinedUnit.SetHeader(&header);
	

	//////////// test 5 : informal table ////////////////////////////
	
	CMergableTableUnit mergeUnit;
	double fPrt = (double)((double)1 / (double)vecColumnDef.size());
	for (int i = 0; i < vecColumnDef.size(); i++)
	{
		vecColumnDef[i].fPct = fPrt;
	}

	mergeUnit.DefineColumns(vecColumnDef);
	rowNum = 15;
	mergeUnit.SetRowNum(rowNum);
	for (int i = 0; i < rowNum + 1; i++)
	{
		mergeUnit.SetCellHeight(i, 2);
	}
	
	mergeUnit.MergeCell(3,3,4,4);
	mergeUnit.MergeCell(7,3,7,4);

	// merge the heading
	for (int i = 0; i < 1; i++)
	{
		mergeUnit.MergeCell(i, 0, i, 1);
		mergeUnit.MergeCell(i, 2, i, columnNum - 1);
	}

	mergeUnit.SetCellText(1,0, L"���");
	mergeUnit.SetCellText(3,3, L"����");
	mergeUnit.SetCellText(2,3, L"���ļ��壬����ʯ");
	mergeUnit.SetCellText(6,8, L"abcde!!!");
	mergeUnit.SetCellText(rowNum - 1, 3, L"asdffdsa");
	mergeUnit.SetCellText(24, 3, L"��Ƥ2");
//	mergeUnit.SetHeader(&header);
//	mergeUnit.SetFooter(footer, 3);
	// set the margin between header and main context by 4 * heightOfLineText
	mergeUnit.SetTopMarginInLineOfText(2);
	
	// to set the row format as "DT_CENTER | DT_WORDBREAK | DT_VCENTER | DT_SINGLELINE"
	// which is also the default value, just to show we can do it.
	mergeUnit.SetAllRowFormat(DT_CENTER | DT_WORDBREAK | DT_VCENTER | DT_SINGLELINE);
	mergeUnit.SetAllRowsFont(40, L"����");
	mergeUnit.SetRowFont(0, 10, L"����");
	mergeUnit.SetRowFont(2, 20, L"����");
	mergeUnit.SetHeadingFont(90, L"����");
//	mergeUnit.SetCellFont(1, 0, 10, L"����");
	mergeUnit.NeedDrawTableOuterline(true);

	// set title
	mergeUnit.SetTitle(L"�������ǲ�����Ů�˵ĸ���");
	// do not to show the column
	mergeUnit.SetNeedDrawHeadings(true);

	//////////// test 4 : print multi-tables in one printing unit ////////////////////////////
	// table 1
	CPrintUnitMergableTable t1;
	double fPrt2 = (double)((double)1 / (double)vecColumnDef.size());
	for (int i = 0; i < vecColumnDef.size(); i++)
	{
		vecColumnDef[i].fPct = fPrt2;
	}

	t1.DefineColumns(vecColumnDef);
	rowNum = 10;
	t1.SetRowNum(rowNum);
	for (int i = 0; i < rowNum + 1/*1 is column*/; i++)
	{
		t1.SetCellHeight(i, 3);
	}
	t1.MergeCell(1,0,2,2);
	t1.SetCellText(1,0, L"����");
	t1.SetRowFormat(DT_CENTER | DT_WORDBREAK | DT_VCENTER | DT_SINGLELINE);
	t1.SetAllRowsFont(30, L"����");
	t1.SetRowFont(3, 30, L"����");
	t1.SetHeadingFont(30, L"����");
	t1.SetCellFont(1, 0, 30, L"����");
	t1.SetTitle(L"����ɽˮ������");
	t1.NeedDrawTableOuterline(true);
	
	// table 2
	CPrintUnitMergableTable t2;
	double fPrt3 = (double)((double)1 / (double)vecColumnDef.size());
	for (int i = 0; i < vecColumnDef.size(); i++)
	{
		vecColumnDef[i].fPct = fPrt3;
		vecColumnDef[i].strName = L"����";
	}

	t2.DefineColumns(vecColumnDef);
	rowNum = 20;
	t2.SetRowNum(rowNum);
	for (int i = 0; i < rowNum + 1/*1 is column*/; i++)
	{
		t2.SetCellHeight(i, 3);
	}
	t2.SetTitle(L"����׳��������");
	for (int i = 1; i < rowNum + 1; i++)
	{
		for (int j = 0; j < vecColumnDef.size(); j++)
		{
			wstring str = L"����-";
			WCHAR t[256];
			wsprintf(t, L"%d-", i);
			str.append(t);
			wsprintf(t, L"%d", j);
			str.append(t);
			t2.SetCellText(i,j, str.c_str());
		}
	}
	
	// table 3
	CPrintUnitMergableTable t3;
	t3.SetTitle(L"�㶫");
	double fPrt4 = (double)((double)1 / (double)vecColumnDef.size());
	for (int i = 0; i < vecColumnDef.size(); i++)
	{
		vecColumnDef[i].fPct = fPrt4;
		vecColumnDef[i].strName = L"�㶫";
	}

	t3.DefineColumns(vecColumnDef);
	rowNum = 20;
	t3.SetRowNum(rowNum);
	for (int i = 0; i < rowNum + 1/*1 is column*/; i++)
	{
		t3.SetCellHeight(i, 3);
	}
	for (int i = 1; i < rowNum + 1; i++)
	{
		for (int j = 0; j < vecColumnDef.size(); j++)
		{
			wstring str = L"����-";
			WCHAR t[256];
			wsprintf(t, L"%d-", i);
			str.append(t);
			wsprintf(t, L"%d", j);
			str.append(t);
			t3.SetCellText(i,j, str.c_str());
		}
	}


	// header for multitableunit
	CHeaderFooterTable header2(&job);
	header2.SetRowColumnNum(3, 5);

	for (int i = 0; i < 3; i++)
	{
		header2.MergeRows(i,0,i,1);
		header2.MergeRows(i,2,i,4);

		header2.SetRowUnitFormat(i, 0, DT_LEFT | DT_SINGLELINE | DT_VCENTER );
		header2.SetRowUnitFormat(i, 2, DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	}
	header2.SetRowUnitText(0,0,L"222��һ��:");
	header2.SetRowUnitText(0,2,L"222Hello world ������磡123 :");
	header2.SetRowUnitText(1,0,L"222No 2 :");
	header2.SetRowUnitText(1,2,L"222123 :");
	header2.SetRowUnitText(2,0,L"222No 3 :");
	header2.SetRowUnitText(2,2,L"222Hello world 123 :");
	/*header2.SetRowUnitText(3,0,L"222No 4 :");
	header2.SetRowUnitText(4,0,L"222No 5 :");*/

	header2.SetRowUnitFont(0,0,60,L"����");
	header2.SetRowUnitFont(0,2,20,L"����");
	header2.SetRowUnitFont(1,0,100,L"����");
	header2.SetRowUnitFont(1,2,100,L"����");

	//header2.NeedOuterLine(false);

	CMultiTablesUnit multiTableUnit;
	multiTableUnit.InsertTables(t1);
	multiTableUnit.InsertTables(t2);
	multiTableUnit.InsertTables(t3);
	multiTableUnit.SetIntervalBetweenFirstTableInLineOfText(1);
	multiTableUnit.SetTableIntervalInLineOfText(2);
	multiTableUnit.NeedPrintTitleExcpetFirstPage(true);
	multiTableUnit.SetHeader(&header2);

	
	//////////// test 5 : print ////////////////////////////
	// actual printing
	// it will use result of the printer dialog's DC
	job.InsertTask(&unitTable1);
//	job.InsertTask(&unitTable2);
//	job.InsertTask(&unitBitmapTable);
//	job.InsertTask(&userDefinedUnit);
	job.InsertTask(&mergeUnit);
	job.InsertTask(&multiTableUnit);

	job.PrintFollowingPrintDialog();
}


void CPrintDlg::OnBnClickedButton1()
{
	// get data
	int rowNum = 100;

	int columnNum = 5;

	ASSERT(rowNum != 0 && columnNum != 0);


	// prepare the column
	vector<COLUMNDEFINITIONS> vecColumnDef;
	// define my four columns...
	for (int i = 0; i < columnNum; i++)
	{
		COLUMNDEFINITIONS cd;
		TCHAR buf[200];
		_itow_s(i, buf, 10);
		wstring str = buf;
		str.append(TEXT("����"));
		cd.strName = str.c_str();

		vecColumnDef.push_back(cd);
	}

	// prepare the data
	wstring **strArr = NULL;
	strArr = new wstring* [rowNum];
	for (int i = 0; i < rowNum; i++)
	{
		strArr[i] = new wstring [columnNum];
	}

	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < columnNum; j++)
		{
			srand((unsigned)time(0));
			int randomPart1 = rand() % 10000;
			int randomPart2 = rand() % 1000;

			TCHAR c = 0;
			if (rand() % 2)
			{
				c = TEXT('+');
			}
			else
			{
				c = TEXT('-');
			}
			std::wstringstream ss;
			ss << c << randomPart1 << TEXT(".") << randomPart2 << TEXT('\0');
			strArr[i][j] = ss.str(); 

			wstring temp = strArr[i][j];
		}
	}

	vector<vector<LPCTSTR> > vecParts;

	for (int i = 0; i < rowNum ; i++)
	{
		vector<LPCTSTR> vecTemp;

		for (int j = 0; j < columnNum ; j++)
		{
			vecTemp.push_back(const_cast<LPCTSTR>(strArr[i][j].c_str()));
		}

		vecParts.push_back(vecTemp);
	}

	/////////////////////////////////////////////////////////////////////////////////
	GPrintJob job;

	CDataTableUnit unitTable1;
	unitTable1.DefineColumns(vecColumnDef);
	unitTable1.SetPrintData(&vecParts);

	// currently it only support for DT_LEFT, DT_CENTER or DT_RIGHT
	unitTable1.SetRowFormat(DT_RIGHT);

	// you do  not need to set the row height. However, if you have set, 
	// the pre-calculation process will be much faster than it does not.
	unitTable1.SetAllRowsHeightInTextLine(1);

	// create a font that is 90�����塱for heading
	unitTable1.SetHeaderFont(90, L"����");
	unitTable1.SetFooterFont(70, L"����");
	unitTable1.SetBodyPrinterFont(90, L"����");

	// draw header
	CHeaderFooterTable header(&job);
	header.SetRowColumnNum(3, 5);

	for (int i = 0; i < 3; i++)
	{
		header.MergeRows(i,0,i,1);
		header.MergeRows(i,2,i,4);

		header.SetRowUnitFormat(i, 0, DT_LEFT | DT_SINGLELINE | DT_VCENTER );
		header.SetRowUnitFormat(i, 2, DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	}
	header.SetRowUnitText(0,0,L"abc��һ��:");
	header.SetRowUnitText(0,2,L"Hello world ������磡123 :");
	header.SetRowUnitText(1,0,L"No 2 :");
	header.SetRowUnitText(1,2,L"123 :");
	header.SetRowUnitText(2,0,L"No 3 :");
	header.SetRowUnitText(2,2,L"Hello world 123 :");

	header.NeedOuterLine(false);
//	unitTable1.SetHeader(&header);

	// draw footer
	// please ensure you have assign for "3" HEADERDEFINITIONS
	

	unitTable1.SetSeparateLineInterval(10);
	unitTable1.SetSeparateLineWidth(3);

	unitTable1.SetTitle(L"����1");
	unitTable1.SetEnglishVersion(true);
	unitTable1.NeedPrintTitleExcpetFirstPage(true);


	job.InsertTask(&unitTable1);

	job.PrintFollowingPrintDialog();
}
