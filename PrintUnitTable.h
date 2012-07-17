#pragma once
#include "gfx_printjob.h"
#include "gfx_printunit.h"
#include "string"

typedef struct tagColumnDefinitions
{
	wstring strName;
	double fPct; // percent
	UINT nFormat;

	tagColumnDefinitions()
	{
		nFormat = DT_CENTER | DT_WORD_ELLIPSIS;
		fPct = 0.2;
	}
}COLUMNDEFINITIONS, *LPCOLUMNDEFINITIONS;

///////////////////////////////////////////////////
// heading and footer enum and structure

#define MAX_HEADER_COUNT 3
#define MAX_FOOTER_COUNT MAX_HEADER_COUNT

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


class CPrintUnitStandardTable : public GPrintUnit
{
public:
	CPrintUnitStandardTable(GPrintJob *pJob = NULL);
	virtual ~CPrintUnitStandardTable();
	
public:
	// printing
	virtual BOOL Print();
	BOOL Print(vector<vector<LPCTSTR>>* pPrintData, UINT nRowFormat);

public:
	// define columns
	void DefineColumns(vector<COLUMNDEFINITIONS>& columns);
	
	// heading and footer
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


	BOOL SetPrintData(vector<vector<LPCTSTR>> *data);
	BOOL SetRowFormat(UINT nFormat);


	// font related methods
	void SetHeadingFont(int nPointSize, LPCTSTR lpszFaceName);
	void SetBodyPrinterFont(int nPointSize, LPCTSTR lpszFaceName);
	void SetBodyScreenFont(int nPointSize, LPCTSTR lpszFaceName);
	void SetHeaderFont(int nPointSize, LPCTSTR lpszFaceName);
	void SetFooterFont(int nPointSize, LPCTSTR lpszFaceName);

	// print metrics methods
	PRINTUNITMETRICS GetMetrics();
	void SetMetrics(PRINTUNITMETRICS pum);

protected:
	virtual void CompleteAllColHeadingsDefinition();
	virtual void CreatePrintFonts();
	virtual void InitPrintMetrics();

	// print header and footer
	virtual void PrintHeader();
	virtual void PrintFooter();

private:
	void PrepareDefaultFonts();
	void PrepareMetrics();
	void GetCurrentTimeAndDate(CString& strDate, CString& time);
	void GetContentOnType( int type, CString context, CString& strHeader );
	void DrawSeparetLine(BOOL bHeader);

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

	void CreateUserDefinedFont(CFont& fontDes, srtFont *fontSource);

private:
	srtFont* m_pUserFontHeading;
	vector<vector<LPCTSTR>> *m_pData;
	UINT m_nRowFormat;
	// column information
	vector<COLUMNDEFINITIONS> m_vecColumnDef;


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
};
