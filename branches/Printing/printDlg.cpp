// printDlg.cpp : implementation file
//

#include "stdafx.h"
#include "print.h"
#include "printDlg.h"
#include "unit_table1.h"
#include "unit_headerpage.h"
#include "unit_indexpage.h"
#include "gfx_printjob.h"
#include <sstream>
#include "PrintUnitTable.h"


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
   virtual ~MyPrintJob() {;}
   virtual void OnPrint();
   
   void InsertTask( GPrintUnit* task )
   {
	   task->SetJob(this);
	   vecTasks.push_back(task);
   }

private:
   vector<GPrintUnit*> vecTasks;
};

void MyPrintJob::OnPrint()
{
	// draw
	for (int i = 0; i < vecTasks.size(); i++)
	{
		vecTasks[i]->Print();
	}
}


///////////////////////////////////////////



void CPrintDlg::OnOK() 
{
	// get data
	CString   strRow; 
	m_editRows.GetWindowText(strRow); 
	int rowNum = _ttoi(strRow);

	CString   strColumns; 
	m_editColumns.GetWindowText(strColumns); 
	int columnNum = _ttoi(strColumns);

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
		str.append(TEXT("个列"));
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


	CPrintUnitStandardTable unitTable1;
	unitTable1.DefineColumns(vecColumnDef);
	unitTable1.SetPrintData(&vecParts);

	// currently it only support for DT_LEFT, DT_CENTER or DT_RIGHT
	unitTable1.SetRowFormat(DT_CENTER);

	// create a font that is 90“宋体”for heading
	unitTable1.SetHeaderFont(90, L"宋体");
	unitTable1.SetFooterFont(70, L"黑体");
	unitTable1.SetBodyPrinterFont(50, L"楷体");

	// draw header
	HEADERDEFINITIONS header[3];
	header[0].type = TYPE_DATETIME;
	header[0].content = L"现在时间 ";
	header[1].type = TYPE_DATA;
	header[1].content = L"赛克力电缆";
	header[2].type = TYPE_PAGE;
	header[2].content = L"页号 ：";
	// the SetHeader will deal at most 3 items currently
	unitTable1.SetHeader(header, 3);

	// draw footer
	// please ensure you have assign for "3" HEADERDEFINITIONS
	FOOTERDEFINITIONS footer[3];
	footer[0].type = TYPE_TIME;
	footer[1].type = TYPE_DATA;
	footer[1].content = L"我是三得利";
	footer[2].type = TYPE_DATE;
	footer[2].content = L"日期 ：";
	// the SetFooter will deal at most 3 items currently
	unitTable1.SetFooter(footer, 3);

	unitTable1.SetSeparateLineInterval(10);
	unitTable1.SetSeparateLineWidth(3);

	MyPrintJob job;
   job.InsertTask(&unitTable1);

   job.Print();
}

