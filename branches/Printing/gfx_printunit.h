
// gfx_printunit.h


#ifndef _GFX_PRINTUNIT_H_
#define _GFX_PRINTUNIT_H_

#include "afxtempl.h"

#include <vector>
using namespace std;

#include "IPrintable.h"

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

namespace Printing
{
	class GPrintJob;
	class GPrintUnit;

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
}

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

namespace Printing
{
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
}

namespace Printing
{
	typedef struct tagPrintUnitMetrics
	{
		int pumHeadingHeight;
		int pumFooterHeight;
		// the height of header
		int pumHeaderHeight;
		int pumRightMarginWidth;
		int pumLeftMarginWidth;
		// the height of the header and footer's seperate line
		int pumFooterLineHeight;
		int pumHeaderLineHeight;
		int pumLineOfText;
		// margin between the page top and the header
		int pumHeaderMargin;
		int pumFooterMargin;

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
			pumHeaderMargin = 0;
			pumFooterMargin = 0;
		}

		tagPrintUnitMetrics& operator=(tagPrintUnitMetrics& pum)
		{
			if (pum.pumHeadingHeight > -1)
			{
				pumHeadingHeight = pum.pumHeaderHeight; 
			}

			if (pum.pumFooterHeight > -1)
			{
				pumFooterHeight = pum.pumFooterHeight; 
			}
			if (pum.pumHeaderHeight > -1)
			{
				pumHeaderHeight = pum.pumHeaderHeight; 
			}
			if (pum.pumRightMarginWidth > -1)
			{
				pumRightMarginWidth = pum.pumRightMarginWidth; 
			}
			if (pum.pumLeftMarginWidth > -1)
			{
				pumLeftMarginWidth = pum.pumLeftMarginWidth; 
			}
			if (pum.pumFooterLineHeight > -1)
			{
				pumFooterLineHeight = pum.pumFooterLineHeight; 
			}
			if (pum.pumHeaderLineHeight > -1)
			{
				pumHeaderLineHeight = pum.pumHeaderLineHeight; 
			}
			if (pum.pumLineOfText > -1)
			{
				pumLineOfText = pum.pumLineOfText; 
			}
			if (pum.pumHeaderMargin > -1)
			{
				pumHeaderMargin = pum.pumHeaderMargin; 
			}
			if (pum.pumFooterMargin > -1)
			{
				pumFooterMargin = pum.pumFooterMargin; 
			}

			return *this;
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
}


// heading effects
#define HE_DOLAST  0x00000001


// header/footer control characters
#define HFC_CENTER        _T('\x1f')            // I18nOk
#define HFC_RIGHTJUSTIFY  _T('\x1e')            // I18nOk
#define HFC_NEWLINE       _T('\n')              // I18nOk
#define HFC_LINES          _T('\x1c')            // I18nOk


#define PTLF_STARTROW   0x00000001
#define PTLF_ENDROW     0x00000002    

namespace Printing
{
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
}



#define GSELECT_PUFONT(pdc, pfp) \
	GSELECT_OBJECT(pdc, &((pfp)->fontPrinter)); \
	GSelectActivePair xx_activefontpair_xx(this, pfp);


// special flags for the below struct
#define INDEXF_DASHES   0x0001   // print dashes in between name and page
#define INDEXF_PAGENO   0x0002   // print page number

using namespace Printing;

namespace Printing
{
	class GPrintIndexTree;

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
}


// return values from startrow
#define SR_NULL           0   // nothing was done
#define SR_NEEDADVANCEDPAGE   1   // a new page was begun

// return values from endrow
#define ER_NULL           0   // nothing was done
#define ER_OVERFLOW       1   // overflow was printed (implies page advance)



namespace Printing
{
	// array used for printing an index 
	class GPrintIndexTree : public CArray <INDEXITEM, INDEXITEM>
	{
	public:
		GPrintIndexTree();

	public:
		void DeletePrintIndexTree(GPrintIndexTree *pTree);

		virtual ~GPrintIndexTree();
	};
}


typedef CTypedPtrArray <CPtrArray, Printing::LPPRINTCOLUMNDEF> PRINTUNITCOLDEFS, *LPPRINTUNITCOLDEFS;

typedef CTypedPtrArray <CPtrArray, LPPRINTUNITCOLDEFS> PRINTUNITHEADINGS;


namespace Printing
{
	// struct containing dimensions from a combination of print unit and print job
	// often used for saving the current dimensions, then restoring later
	typedef struct tagJobUnitDim
	{
		CRect rectJINFO; // JINFO.m_rectDraw
		CRect rectJRECT;  // JRECT
		CPoint ptJCUR;  // JCUR
		PRINTUNITMETRICS pum;  // m_pum

	} JOBUNITDIM, *LPJOBUNITDIM;
}

///////////////////////////////////////////////////
// heading and footer enum and structure
#define MAX_HEADER_COUNT 3
#define MAX_FOOTER_COUNT MAX_HEADER_COUNT

namespace Printing
{
	enum HEADERFOOTERTYPE
	{
		TYPE_EMPTY,

		TYPE_PAGE, // print current page, using "content" as prefix.
		TYPE_DATE, // the current date, using "content" as prefix.
		TYPE_TIME, // the current time, using "content" as prefix.
		TYPE_DATETIME, // the current time, using "content" as prefix.

		TYPE_DATA  // user-defined data, it will use "content"
	};

	typedef struct tagHeaderDefinitions
	{
		HEADERFOOTERTYPE type;	
		std::wstring content;

		tagHeaderDefinitions()
		{
			type = TYPE_EMPTY;
		}
	}HEADERDEFINITIONS, *LPHEADERDEFINITIONS;

	typedef HEADERDEFINITIONS FOOTERDEFINITIONS;
	typedef LPHEADERDEFINITIONS LPFOOTERDEFINITIONS;
	///////////////////////////////////////////////////
}

namespace Printing
{
	class GPrintUnit : public CObject, IPrintable
	{
		friend GSelectActivePair;

		DECLARE_DYNAMIC(GPrintUnit)


	public:
		// header and footer related methods
		// the sequence in the array is important, which means the "left", "center" and "right" in sequence
		// if not necessary, just leave the corresponding item with "type = EMPTY;"
		void SetHeader(HEADERDEFINITIONS *header, int size);
		void SetFooter(FOOTERDEFINITIONS *footer, int size);
		// return the old value
		bool NeedHeaderLine(bool bNeedHeaderLine = true);
		bool NeedFooterLine(bool bNeedFooterLine = true);
		// set the interval between the header and the table
		UINT  SetSeparateLineInterval(UINT interval);
		UINT  SetSeparateLineWidth(UINT width);	
		// header and footer helpers
		void GetContentOnType( int type, CString context, CString& strHeader );
		// if bHeader is TRUE, means drawing header's line, or drawing footer's line
		void DrawSeparetLine(BOOL bHeader);

		// attributes set and get
		void SetBodyPrinterFont(int nPointSize, LPCTSTR lpszFaceName);
		void SetBodyScreenFont(int nPointSize, LPCTSTR lpszFaceName);
		void SetHeaderFont(int nPointSize, LPCTSTR lpszFaceName);
		void SetFooterFont(int nPointSize, LPCTSTR lpszFaceName);

		// print metrics methods
		PRINTUNITMETRICS GetMetrics();
		// set -1 to use default
		void SetMetrics(PRINTUNITMETRICS pum);

	public:
		GPrintUnit(GPrintJob *pJob=NULL);
		virtual ~GPrintUnit();

		GPrintJob *GetJob() const;
		virtual BOOL EnvSetBeforePrinting();
		virtual BOOL EnvCleanupAfterPrinting();

	protected:
		// called when the unit's print job is ready to begin
		virtual void OnBeginPrinting();
		// called when the unit's print job has ended.
		virtual void OnEndPrinting();

	public:
		// before calling this method, call InsertPrintCol() to insert 
		// the columns' definitions and calculate the start positions of all the columns
		virtual void CompleteAllColHeadingsDefinition();
		virtual void CreatePrintFonts();
		virtual void DeleteDefaultFonts();
		virtual void InitPrintMetrics();

	protected:
		struct srtFont
		{
			int nPointSize;
			wstring name;

			srtFont(int size, wstring name)
			{
				this->nPointSize = size;
				this->name = name;
			}
		};
		virtual void CreateUserDefinedFont(CFont& fontDes, srtFont *fontSource);


		// the nHeading in the following functions means the number of the column set.
		// there can be many column set, within which there are many columns
		virtual void InsertPrintCol(int nPos, LPCTSTR lpszName, double fColPct=0.0, UINT nFormat = 0, int nHeading=0);
		void InsertPrintCol(LPPRINTCOLUMN pCol, int nHeading=0);
		void ClearColumnSet();

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
		// return how many pages have been drawn
		// fill the table with data
		int PrintTableContents(vector<vector<LPCTSTR> >* pContents, UINT nRowFormat, int from, int to, BOOL bPrintHeadingWhenChangePage = TRUE);
		virtual void PreCalculateRowHeight(vector<vector<LPCTSTR> >& contents, UINT nRowFormat, int from, int to, BOOL bPrintHeadingWhenChangePage);
		virtual void PreCalculateRowStartPosition(vector<vector<LPCTSTR> >& contents, UINT nRowFormat, int from, int to, BOOL bPrintHeadingWhenChangePage);

	protected:
		virtual void PrintColForOverflow(int row, int nCol, UINT height, UINT nFormat);	
		// this method can only output contents that fit "this" page, and the overflows will be output in the EndRow
		virtual bool PrintColumnContent(int nCol, LPCTSTR lpszText, UINT nFormat, UINT top, UINT height);
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
		// return the height of the text
		int PrintTextLine(LPCTSTR lpszText, UINT nFormat=0, int tmHeight=0, bool bDrawOuterline = false);
		int PrintTextLine(LPPRINTTEXTLINE lpTextLine, bool bDrawOterLine = false);
		// override this if you want to change the behavior of all overloaded
		// versions of PrintTextLine()
		virtual int PrintTextLineEx(LPPRINTTEXTLINE lpTextLine, bool bDrawOterLine);

		void DrawSeperateLine(LPRECT lpRect);
		void DrawRepeatChar(TCHAR ch, LPRECT lpRect);

		int PumTypeToHeight(PUMTYPE pt) const;

		// printing index trees
		virtual void PrintTree(GPrintIndexTree *pObj, int nLevel=0);
		virtual void PrintTreeItem(LPINDEXITEM lpIndex, int nLevel);
		virtual void GetLevelInfo(INDEXLEVELINFO& li, LPINDEXITEM lpIndex, int nLevel);
		void AddIndexItem(INDEXITEM *pII);

		void SetNeedPreprocessSign(bool bNeedPreprocess);
		bool GetNeedPreprocessSign();

		void SetPreprocessValue(bool bPreprocess);
		bool GetPreprocessValue();

	protected:
		void DrawOuterLine();
		PARAFORMAT ConfirmRichEditParaFormat( UINT nFormat );
		int DrawTableContents( vector<vector<LPCTSTR> >& contents, UINT nRowFormat, int from, int to, BOOL bPrintHeadingWhenChangePage = FALSE);
		void ClearColumnOverflow();

		void GetCurrentTimeAndDate( CString& date, CString& time );

		//////////////////////////////////////////////////////////////////////////////////////
		// title related methods
	public:
		void SetTitle(LPCTSTR title, UINT nFormat = DT_CENTER);
		void SetTitlePen(int nPointSize, LPCTSTR lpszFaceName);
		// set the interval between the title , header and the table contents
		// in height of line_of_text
		int SetTitleMargin(int titleMarginInLineOfText);
		void NeedPrintTitleExcpetFirstPage(bool bNeed) {m_bNeedPrintTitleExcpetFirstPage = bNeed;}
	protected:
		int PrintTitleAndMoveCursor(BOOL bShowContinued);
		int PrintTitle(BOOL bShowContinued);
	protected:
		// title related attributes
		wstring m_title;
		UINT m_nTitleFormat;
		CFont m_FontTitle;
		srtFont * m_pFontTileSrt;
		int m_titleMargin;
		bool m_bNeedPrintTitleExcpetFirstPage;
		////////////////////////////////////////////////////////////////////////////////////////

	private:
		// call to set the print job that owns this unit
		void SetJob(GPrintJob *pJob);

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

		bool m_bPreprocessing;
		bool m_bCheckPosition;
		bool m_bPrintThePage;

	public:
		// pointer to owner print job
		GPrintJob *m_pJob;

	private:
		// all the followings are for drawing the table
		// to contain the columns, the index means which page this column is in
		// i.e. m_vecColumnPage[0][1] indicates the second column in page 1
		vector<vector<int>> m_vecColumnPage;


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

		// a sign to indicate whether we have done the preprocessing
		bool m_bNeedPreprocessed;


	protected:
		// header contents
		HEADERDEFINITIONS m_header[MAX_HEADER_COUNT];
		// footer contents
		HEADERDEFINITIONS m_footer[MAX_FOOTER_COUNT];

		bool m_bNeedHeaderSeparateLine;
		bool m_bNeedFooterSeperateLine;


		srtFont* m_pUserFontHeader;
		srtFont* m_pUserFontFooter;
		srtFont* m_pUserFontPrinter;
		srtFont* m_pUserFontScreen;

		UINT m_separateLineWidth;
		UINT m_separateLineInterval;

		// this is the index of m_vecColumnPage, to indicate which Columns page we are
		int m_currentWorkingColums;

		friend class GPrintJob;
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
}




#endif


