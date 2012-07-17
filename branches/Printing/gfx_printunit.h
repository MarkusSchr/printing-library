
// gfx_printunit.h


#ifndef _GFX_PRINTUNIT_H_
#define _GFX_PRINTUNIT_H_

#include "afxtempl.h"

class GPrintJob;
class GPrintUnit;

#include <vector>
using namespace std;

/////////////////////////////////////////
// misc helpers 
// NUL character
#define GNUL             ((TCHAR)0)
// set a structure to NULs
#define GMakeStructFillZero(st)   ::ZeroMemory(&(st), sizeof((st)))
// find the size of an array
#define GGetArrayElementCount(array)  (sizeof(array)/sizeof(array[0]))
// set a string to NULs
#define GMakeStringFillZero(str)    ::ZeroMemory((str), sizeof((str)))

// returns a percentage 'fPct' of a value 'val'
template <class _Ty> inline _Ty GPERCENT(_Ty val, double fPct)
{
	val = (_Ty)((double)val * fPct);      
	return val;
}

// class which selects a GDI object upon construction, and automatically 
// deselects it upon destruction - takes the burden of remembering to deselect
// a GDI object off the programmer
class GSelectGdiObject 
{
	// Constructor
public:
	GSelectGdiObject(CDC *pDC, CGdiObject *pObject);

	// Implementation
protected:
	// dc in which the object was selected
	CDC *m_pDC;
	// the previously selected object
	HGDIOBJ m_hOldGdiObject;

public:
	virtual ~GSelectGdiObject();
};


inline CRect& GMAKERECT(CRect& r, int nLeft, int nTop, int nWidth, int nHeight)
{
	r.SetRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
	return r;
}

// macro provides safer usage of CDC::SelectObject by providing
// automatic "re-selection" of old gdi object...
#define GSELECT_OBJECT(dc, obj)  GSelectGdiObject xx_SelectGdiObject(dc, obj)

// converts a CFont to a CHARFORMAT structure used by rich edit controls
BOOL GfxFontToCharformat(CFont *pFont, CHARFORMAT& cf, CDC *pDC=NULL);
// converts a logfont height to a point size
int GfxHeightToPointSize(int nHeight, CDC *pDC=NULL);
// count the number of lines in some text, only to count how many '\n' 
// are there in the text
int GfxCountLines(LPCTSTR lpszText);

// turn a bit on or off 
#define GSET_BIT(val, bit, b) \
	if(b) \
	val |= bit; \
				  else \
				  val &= ~bit;


/////////////////////////////////////////


// easy to use macros from within a print unit class
#define JDC       (*(m_pJob->m_pDC))
#define JINFO     (*(m_pJob->m_pInfo))
#define JDLG      (*(m_pJob->m_pDialog))
#define JPD       (*(m_pJob->m_pPD))
#define JCUR      m_pJob->m_ptCursor
#define JRECT     m_pJob->m_rectClient
#define JRECTDEV  m_pJob->m_rectPage



// print column flags
#define PCF_RIGHTMARGIN  0x00000001  // decrease right edge of column rects when printing them
#define PCF_USERICHEDIT  0x00000002  // forces column to print in rich edit control style
#define PCF_TEXTISID     0x00000004  // 'lpszText' is a resource id
#define PCF_STRETCHY     0x00000008  // indicates column should use remainder of available space


typedef struct tagPrintColumnDef
{
	CString strName; // the name of the column
	CString strOverflow; // if one page can not contain all the text, the next page will show it out. This field is the remaining text
	double fPct; // the percentage of the column of the the whole width
	UINT nWidth; // the actual width to be printed in the client rect
	UINT nStart; // the start position of the column in the client rect
	DWORD dwFlags;
	UINT nFormat;

} PRINTCOLUMNDEF, *LPPRINTCOLUMNDEF;



typedef struct tagPrintUnitMetrics
{
	int pumHeadingHeight;
	int pumFooterHeight;
	int pumHeaderHeight;
	int pumRightMarginWidth;
	int pumLeftMarginWidth;
	int pumFooterLineHeight;
	int pumHeaderLineHeight;
	int pumLineOfText;

	tagPrintUnitMetrics()
	{
		pumHeadingHeight = 0;
		pumFooterHeight = 0;
		pumHeaderHeight = 0;
		pumRightMarginWidth = 0;
		pumLeftMarginWidth = 0;
		pumFooterLineHeight = 0;
		pumHeaderLineHeight = 0;
		pumLineOfText = 0;
	}
} PRINTUNITMETRICS, *LPPRINTUNITMETRICS;



typedef enum tagPumType
{
	PT_HEADING = -1,
	PT_LINEOFTEXT = -2,
	PT_FOOTER = -3,
	PT_HEADER = -4,
	PT_LEFTMARGIN = -5,
	PT_RIGHTMARGIN = -6,
	PT_FOOTERLINE = -7,
	PT_HEADERLINE = -8

} PUMTYPE;


// heading effects
#define HE_DOLAST  0x00000001


// header/footer control characters
#define HFC_CENTER        _T('\x1f')            // I18nOk
#define HFC_RIGHTJUSTIFY  _T('\x1e')            // I18nOk
#define HFC_NEWLINE       _T('\n')              // I18nOk
#define HFC_LINES          _T('\x1c')            // I18nOk


#define PTLF_STARTROW   0x00000001
#define PTLF_ENDROW     0x00000002    


typedef struct tagPrintTextLine
{
	LPCTSTR lpszText; // text content
	int nFormat;
	CRect rectText; // rect to contain the context
	int tmHeight; // height to adjust from the rectText.top
	DWORD dwFlags;

} PRINTTEXTLINE, *LPPRINTTEXTLINE;



typedef struct tagPrintUnitFontPair
{
	CFont fontPrinter;
	CFont fontScreen;

} PUFONTPAIR, *LPPUFONTPAIR;



class GSelectActivePair
{
public:
	GSelectActivePair(GPrintUnit *pUnit, LPPUFONTPAIR pPair);
	~GSelectActivePair();

protected:
	LPPUFONTPAIR m_pOldPair;
	GPrintUnit *m_pUnit;
};



#define GSELECT_PUFONT(pdc, pfp) \
	GSELECT_OBJECT(pdc, &((pfp)->fontPrinter)); \
	GSelectActivePair xx_activefontpair_xx(this, pfp);



class GPrintIndexTree;

// special flags for the below struct
#define INDEXF_DASHES   0x0001   // print dashes in between name and page
#define INDEXF_PAGENO   0x0002   // print page number


// structure used for individual index tree items
typedef class GIndexItem
{
public:
	CString strName;
	UINT nFlags;
	GPrintIndexTree *pChildren; 
	WORD wPage;

	GIndexItem();

} INDEXITEM, *LPINDEXITEM;


// structure to be initialized by derived class, info about
// how to print an index tree level
typedef struct tagIndexLevelInfo
{
	CFont *pFont;
	int nIndent;
	int nRowHeight;

} INDEXLEVELINFO;




typedef struct tagPrintColumn
{
	int nPos; // the position for this column in the column set, based on 0
	LPCTSTR lpszName; // name for the column
	double fColPct; // how much percentage does this column cost of the total columns
	DWORD dwFlags; // the combination of PCF_RIGHTMARGIN, PCF_USERICHEDIT, PCF_TEXTISID and PCF_STRETCHY
	UINT nFormat; // format of the column
} PRINTCOLUMN, *LPPRINTCOLUMN;


// return values from startrow
#define SR_NULL           0   // nothing was done
#define SR_NEEDADVANCEDPAGE   1   // a new page was begun

// return values from endrow
#define ER_NULL           0   // nothing was done
#define ER_OVERFLOW       1   // overflow was printed (implies page advance)




// array used for printing an index 
class GPrintIndexTree : public CArray <INDEXITEM, INDEXITEM>
{
public:
	GPrintIndexTree();

public:
	void DeletePrintIndexTree(GPrintIndexTree *pTree);

	virtual ~GPrintIndexTree();
};


typedef CTypedPtrArray <CPtrArray, LPPRINTCOLUMNDEF> PRINTUNITCOLDEFS, *LPPRINTUNITCOLDEFS;

typedef CTypedPtrArray <CPtrArray, LPPRINTUNITCOLDEFS> PRINTUNITHEADINGS;



// struct containing dimensions from a combination of print unit and print job
// often used for saving the current dimensions, then restoring later
typedef struct tagJobUnitDim
{
	CRect rectJINFO; // JINFO.m_rectDraw
	CRect rectJRECT;  // JRECT
	CPoint ptJCUR;  // JCUR
	PRINTUNITMETRICS pum;  // m_pum

} JOBUNITDIM, *LPJOBUNITDIM;





class GPrintUnit : public CObject
{
	friend GSelectActivePair;

	DECLARE_DYNAMIC(GPrintUnit)

public:
	GPrintUnit(GPrintJob *pJob=NULL);

	// call to set the print job that owns this unit
	void SetJob(GPrintJob *pJob);
	GPrintJob *GetJob() const;
	// call to print the job, returns FALSE if printing should stop
	virtual BOOL Print();

public:
	// before calling this method, call InsertPrintCol() to insert 
	// the columns' definitions and calculate the start positions of all the columns
	virtual void CompleteAllColHeadingsDefinition();
	virtual void CreatePrintFonts();
	virtual void InitPrintMetrics();
	// the nHeading in the following functions means the number of the column set.
	// there can be many column set, within which there are many columns
	virtual void InsertPrintCol(int nPos, LPCTSTR lpszName, double fColPct=0.0, UINT nFormat = 0, int nHeading=0);
	void InsertPrintCol(LPPRINTCOLUMN pCol, int nHeading=0);

	LPPRINTCOLUMNDEF GetPrintColDef(int nCol, int nHeading=-1);
	// initializes 'pDim' with current job and unit dimensions
	void SaveDim(LPJOBUNITDIM pDim);
	// restores job and unit dimensions with data found in 'pDim'
	void RestoreDim(LPJOBUNITDIM pDim);

protected:
	// starts a new page. it will draw the header(which is the opposite of the footer)
	// heading will be drawn accordingly
	virtual bool StartPage();
	// ends current page. it will draw the footer 
	virtual void EndPage();
	// prints a blank page, set 'bIncPageNo' to FALSE if you don't want
	// the page number affected...
	virtual void AdvancePage(BOOL bIncPageNo=TRUE);

	virtual int StartRow(int nHeight=PT_LINEOFTEXT);
	virtual int EndRow(BOOL bCheckForOverflow=TRUE, BOOL bDrawOutline = TRUE);
	BOOL IsPreviousRowOverflow(); 

	virtual bool OnContinueRow();

	virtual BOOL ContinuePrinting() const;
	// adjusts all relavent unit and job dimensions, returns the previous map mode
	virtual int SetMapMode(int nMapMode);
	
	// draw table
	void PrintTableContents(vector<vector<LPCTSTR> >* pContents, UINT nRowFormat, BOOL bPrintHeadingWhenChangePage = TRUE);

public:
	void PrintColForOverflow(int row, int nCol, UINT height, UINT nFormat);	
	// this method can only output contents that fit "this" page, and the overflows will be output in the EndRow
	bool PrintColumnContent(int nCol, LPCTSTR lpszText, UINT nFormat, UINT top, UINT height);
	virtual int DrawColText(LPCTSTR lpszText, int nLen, CRect r, UINT nFormat, int nCol, LPPRINTCOLUMNDEF lpDef);
	virtual void PrintColHeadings(vector<int>& headings, UINT nEffects=0);
	virtual void PrintColHeading(LPCTSTR lpszName, int nLen, CRect r, UINT nFormat, UINT nEffects);
	virtual void DoHeadingEffect(int nCol, LPCTSTR lpszName, int nLen, CRect r,
		UINT nFormat, UINT nEffects);

	virtual void PrintFooter();
	virtual void PrintHeader();

	virtual void RealizeMetrics();

	void SetActiveHeading(int nHeading);

	void PrintFooterText(LPCTSTR lpszText);
	void PrintHeaderText(LPCTSTR lpszText);
	void PrintTextLine(LPCTSTR lpszText, UINT nFormat=0, int tmHeight=0);
	void PrintTextLine(LPPRINTTEXTLINE lpTextLine);
	// override this if you want to change the behavior of all overloaded
	// versions of PrintTextLine()
	virtual void PrintTextLineEx(LPPRINTTEXTLINE lpTextLine);

	void DrawSeperateLine(LPRECT lpRect);
	void DrawRepeatChar(TCHAR ch, LPRECT lpRect);

	int PumTypeToHeight(PUMTYPE pt) const;

	// printing index trees
	virtual void PrintTree(GPrintIndexTree *pObj, int nLevel=0);
	virtual void PrintTreeItem(LPINDEXITEM lpIndex, int nLevel);
	virtual void GetLevelInfo(INDEXLEVELINFO& li, LPINDEXITEM lpIndex, int nLevel);
	void AddIndexItem(INDEXITEM *pII);

private:
	void DrawOuterLine();

	void PreCalculateRowHeight(vector<vector<LPCTSTR> >& contents, UINT nRowFormat);
	void PreCalculateRowStartPosition(vector<vector<LPCTSTR> >& contents, UINT nRowFormat);
	void DrawTableContents( vector<vector<LPCTSTR> >& contents, UINT nRowFormat, BOOL bPrintHeadingWhenChangePage = FALSE);

protected:
	CTypedPtrArray <CPtrArray, LPPRINTCOLUMNDEF> m_colDefs;
	// the extents of the current row
	CSize m_sizeCurrentRow;
	// metrics used for printing
	PRINTUNITMETRICS m_pum;
	// the current heading
	LPPRINTUNITCOLDEFS m_lpActiveColDefs;
	// headings, containing all sets of the columns
	PRINTUNITHEADINGS m_headings;
	// pointer to the currently active font pair
	LPPUFONTPAIR m_pActiveFontPair;
	// TRUE if we're currently printing a heading
	BOOL m_bPrintingHeading;
	//rich edit control used to print multiple lines in same column
	CRichEditCtrl m_richEdit;

	////////////////////////////////////////
	// fonts 
	PUFONTPAIR m_fontPairBody;
	// heading is the column heads
	CFont m_fontHeading;

	// header is the content of the top of the page
	CFont m_fontHeader;
	CFont m_fontFooter;

public:
	// pointer to owner print job
	GPrintJob *m_pJob;

public:
	virtual ~GPrintUnit();

private:
	// all the followings are for drawing the table
	// to contain the columns, the index means which page this column is in
	// i.e. m_vecColumnPage[0][1] indicates the second column in page 1
	vector<vector<int>> m_vecColumnPage;

	// this is the index of m_vecColumnPage, to indicate which Columns page we are
	int m_currentWorkingColums;

	struct srtPreprocessData
	{
		// the final height of the row after scanning all the columns 
		int rowHeight;
		// the overflow height of this row
		int overflowHeight;

		srtPreprocessData()
		{
			rowHeight = 0;
			overflowHeight = 0;
		}
	};
	// pre-process the row starting position
	vector<srtPreprocessData> m_preprocessRowStartPosY;

	// pre-process row height data
	vector<srtPreprocessData> m_preprocessRowHeight;

	// pre-process heading height data
	int m_preprocessHeadingHeight;

	bool m_bPreprocessing;
	bool m_bCheckPosition;
};



////////////////////////////////////////////////////////////////////////////////
// text line parser


struct GPTLTOKEN
{
	CString strToken; // text to draw
	UINT nFormat; // DrawText() Format Flags like DT_RIGHT
	BOOL bDots;
	int bNewLine;
};

// return value from GetNextToken()
enum GNTRESULT
{
	GNTR_ERROR,
	GNTR_ENDOFLINE,
	GNTR_TOKEN,
	GNTR_TOKENNEWLINE,
};


// text line profile
struct GPTLPROFILE
{
	BOOL bDots;
};



class GPrintTextLineParser
{
public:
	GPrintTextLineParser();

	BOOL GetProfile(LPCTSTR lpszText, GPTLPROFILE& profile);
	// Get next Sub-string and its format according to the text input.
	// the lpszText will only be used at the first time of the call of one 
	// instance of GPrintTextLineParser. Later it will make no use.
	GNTRESULT GetNextToken(LPCTSTR lpszText, GPTLTOKEN& token);

protected:
	LPCTSTR m_lpszCurChar;
	int m_nSkipChars;
	TCHAR m_chCur;
	UINT m_nNewFormat;
	BOOL m_bNewSeperateLine;
	BOOL m_bNewLine;

public:
	virtual ~GPrintTextLineParser();
};




#endif


