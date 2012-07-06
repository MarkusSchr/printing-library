


#include "HSECPrintPageConfig.h"

#include <atlimage.h>
#include <vector>
using namespace std;

struct SPrintTable
{
	CString				title;
	//bool				IsPrtTtitle;
	vector<CString>		vhead;
	vector<CString>		vbody;

	vector<int>			vhdcellwd;
//	int					headheight;
//	int					content_line_height;

	int					tableft;
	int					tabtop;
	int					tabwidth;
	
	bool				isContentCenter;
	bool				isFrstLinePrt;
	float				titleHeightScl;
	float				contentHeightScl;

	CFont*				title_fnt;
	CFont*				content_fnt;
	CPen*				tabline_pen;

public:
	SPrintTable():title(_T(""))
		//,IsPrtTtitle(true)
		, vhead()
		, vbody()
		, vhdcellwd()
		/*,headheight()*/
		/*, content_line_height()*/
		, tableft()
		, tabwidth()
		, title_fnt()
		, content_fnt()
		, tabline_pen()
		, isContentCenter(false)
		, isFrstLinePrt(true)
		, titleHeightScl(1.5f)
		, contentHeightScl(1.5f)
	{		 };

};

struct SDataPage
{
	vector<SPrintTable>		vTabs;
	//int						nSpcTabHD;
public:
	SDataPage():vTabs()
		/*, nSpcTabHD()*/
	{

	}
};





class CHSEPrinter : public CHSECPrintPageConfig
{
	vector<SDataPage>		m_vDataPages;

	int		m_nPrintPage;

	//布局，
//page1
	int		m_nPage1stHeadTD;				// 距上间距 单位 0.1mm,
	int		m_nPage1stHeadLD;
	int		m_nPage1stIconHeight;
	int		m_nPage1stIconWidth;
	int		m_nTestReportTitleTD;
	int		m_nTestReportTitleBD;
	int		m_nPage1stBodyMidLD;
	int		m_nPage1stBodyDownBD;

	int		m_nFootHeight;
//page2
	int		m_nPage2ndHeadHeight;
	int		m_nPage2ndLD;
//page3
	int		m_nHeadHeight;				
	int		m_nPage3rdHeadLD;
	int		m_nPage3rdIconHeight;
	int		m_nPage3rdIconWidth;

	//page4
	int		m_nSpcTabHD;

//字体，格式
//page1
	CImage	m_imgPage1stIcon;
	CFont	m_fntTestDepartmentName;	
	CFont	m_fntTestDepartmentAddress;	
	CFont   m_fntTestReportTitle;
	CFont   m_fntPage1stBodyMid;
	CFont   m_fntPage1stBodyDown;

	CFont   m_fntFoot;
	CPen    m_penHeadFoot;

//page2
	CFont	m_fntPage2ndTabCaption;
	CFont	m_fntPage2ndTabContent;
	CPen	m_penTabLine;

//page3
	CFont	m_fntHeadCaption;	
	CFont	m_fntHeadItems;	
	CFont	m_fntPage3rdTitle;	

//page4
	CFont	m_fntPage4thTabCaption;
	CFont	m_fntPage4thTabContent;

	//////传递字体
	LOGFONT   m_LFntTestDepartmentName;	
	LOGFONT	  m_LFntTestDepartmentAddress;	
	LOGFONT   m_LFntTestReportTitle;
	LOGFONT   m_LFntPage1stBodyMid;
	LOGFONT   m_LFntPage1stBodyDown;
	LOGFONT   m_LFntFoot;
	//page2
	LOGFONT	  m_LFntPage2ndTabCaption;
	LOGFONT   m_LFntPage2ndTabContent;
	//page3
	LOGFONT	  m_LFntHeadCaption;	
	LOGFONT	  m_LFntHeadItems;	
	LOGFONT	  m_LFntPage3rdTitle;	
	//page4
	LOGFONT	  m_LFntPage4thTabCaption;
	LOGFONT	  m_LFntPage4thTabContent;

public:
	CHSEPrinter();
	~CHSEPrinter();

	void PreparePrinting(CPrintInfo* pInfo);
	void BeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/);
	void PrepareDC(CDC* pDC, CPrintInfo* pInfo);
	void Print(CDC* pDC, CPrintInfo* pInfo);
	void EndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/);


	bool SetPrintFont(EPrintPageConfig e, LOGFONT lf);
	bool GetPrintFont(EPrintPageConfig e, LOGFONT& lf);

	bool GetPrintFont(EPrintPageLayout e, int val);
	bool GetPrintFont(EPrintPageLayout e, int& val);


private:

	void InitLayout();
	void InitPrintGDI();


	void PrintPage1st(CDC* pDC, CPrintInfo* pInfo);
	void PrintPage2nd(CDC* pDC, CPrintInfo* pInfo);
	void PrintPage3rd(CDC* pDC, CPrintInfo* pInfo);
	void PrintDataPages(CDC* pDC, CPrintInfo* pInfo, int nCurPage);

	void SetMaxPrintPage(CPrintInfo* pInfo, int num );
	void PrintIcon(CDC* pDC,CPrintInfo* pInfo, CRect& rtIC);
	void PrintPageFoot(CDC* pDC, CPrintInfo* pInfo);

	bool GetTabHeight(CDC* pDC/*,CPrintInfo* pInfo*/,SPrintTable sTab,int& nH,int topheadline=0);
	bool PrintSingleTable(CDC* pDC, CPrintInfo* pInfo, SPrintTable sTab,int& nY);
	void PrintPageHead(CDC* pDC, CPrintInfo* pInfo);
	void PrintPageTitle(CDC* pDC,CString str,CRect& rect);

	void FormatPointPairTableString(int nBpt,int nCnt, SPrintTable& sTable);
	void FormatTableWidth_Col(CDC* pDC,CString mcont,int allcol,int& mcol,SPrintTable& sTable,int cellcnt=1);
	void FormatTableWidth_Col(int curcol,int & tabwidth,vector<int>& vhw);
	void CombineTableCellWidth_Col(int cellcnt,vector<int>& vhw);
	void FormatPairTableFirstHeadString(int cbcellcnt,SPrintTable& sTable);
	void FomatPairdataTableString(int cbcellcnt,SPrintTable& sTable);
	void FormatDataPages(CDC* pDC, CPrintInfo* pInfo);
	
	void FormatTableWidth_Row(CDC* pDC,CString mcont,int allcol,SPrintTable& sTable);
	void GetTRowHeight(CDC* pDC,SPrintTable sTab,int& nH0,int& nH1);
	void FormatPointSingleTableString(int nBpt,int nCnt, SPrintTable& sTable);
	void FormatPointSingleTableMMString(int nBpt,int nCnt, SPrintTable& sTable);


	void PrePareGDI(CDC* pDC, CPrintInfo* /*pInfo*/);



};