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

template<class T>
class CPrintUnitStandardTable : public GPrintUnit
{
public:
	CPrintUnitStandardTable(GPrintJob *pJob = NULL);
	virtual ~CPrintUnitStandardTable();

public:
	// define columns
	void DefineColumns(vector<COLUMNDEFINITIONS>& columns);

	virtual BOOL SetPrintData(vector<vector<T*>> *data);
	BOOL SetRowFormat(UINT nFormat);

	// font related methods
	void SetHeadingFont(int nPointSize, LPCTSTR lpszFaceName);

protected:
	virtual void CompleteAllColHeadingsDefinition();
	virtual void CreatePrintFonts();
	virtual void DeleteDefaultFonts();
	virtual void InitPrintMetrics();
	BOOL CheckCurrentDCCompatibleWithPrevious();
	void CreateUserDefinedFont(CFont& fontDes, srtFont *fontSource);

private:
	void PrepareDefaultFonts();

	void PrepareMetrics();
	void GetCurrentTimeAndDate(CString& strDate, CString& time);
	


protected:
	srtFont* m_pUserFontHeading;
	vector<vector<T*>> *m_pData;
	UINT m_nRowFormat;
	// column information
	vector<COLUMNDEFINITIONS> m_vecColumnDef;

	bool m_bNeedResetColumns;

	// the enumarate num of pages of this print task.
	// the value will be valis if the user has called GPrintJob::EvaluatePageNum()
	int pages;

	CDC * m_pPreviousDC;
};

template<class T>
CPrintUnitStandardTable<T>::CPrintUnitStandardTable( GPrintJob *pJob )
: GPrintUnit(pJob)
{
	m_bNeedHeaderSeparateLine = true;
	m_bNeedFooterSeperateLine = true;

	m_separateLineWidth = 2;
	m_separateLineInterval = 15;

	m_pData = NULL;
	m_nRowFormat = DT_CENTER;

	m_pUserFontHeading = NULL;

	m_bNeedResetColumns = true;

	pages = 0;

	m_pPreviousDC = NULL;
}

template<class T>
void CPrintUnitStandardTable<T>::PrepareDefaultFonts()
{
	LOGFONT logFont;
	GMakeStructFillZero(logFont);

	LPCTSTR lpszFaceName = _T("Arial");//I18nok

	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = 90;
	lstrcpyn(logFont.lfFaceName, lpszFaceName, GGetArrayElementCount(logFont.lfFaceName));
	logFont.lfWeight = FW_BOLD;

	m_fontHeading.CreatePointFontIndirect(&logFont, &JDC);
	m_fontPairBody.fontPrinter.CreatePointFont(90, lpszFaceName, &JDC);
	m_fontPairBody.fontScreen.CreatePointFont(90, lpszFaceName);
	m_fontHeader.CreatePointFont(110, _T("Garamond"), &JDC);//I18nOK	
	m_fontFooter.CreatePointFont(90, _T("Garamond"), &JDC);//I18nOK
}

template<class T>
void CPrintUnitStandardTable<T>::PrepareMetrics()
{
	TEXTMETRIC tm;

	{
		GSELECT_OBJECT(&JDC, &m_fontHeader);
		JDC.GetTextMetrics(&tm);

		m_pum.pumHeaderHeight = tm.tmHeight * 2;
		m_pum.pumHeaderLineHeight = tm.tmHeight;
	}

	{
		GSELECT_OBJECT(&JDC, &m_fontFooter);
		JDC.GetTextMetrics(&tm);

		m_pum.pumFooterHeight = tm.tmHeight * 2;
		m_pum.pumFooterLineHeight = tm.tmHeight;
	}

	{
		GSELECT_OBJECT(&JDC, &(m_fontPairBody.fontPrinter));
		JDC.GetTextMetrics(&tm);
		m_pum.pumLineOfText = tm.tmHeight;
	}

	{
		GSELECT_OBJECT(&JDC, &(m_fontHeading));
		JDC.GetTextMetrics(&tm);
		m_pum.pumHeadingHeight = tm.tmHeight;
	}

	// left and right margin
	// both the margin is 1/20 of the page
	{
		m_pum.pumLeftMarginWidth = (JRECTDEV.right - JRECTDEV.left)/20;
		m_pum.pumRightMarginWidth = m_pum.pumLeftMarginWidth;
	}
}

template<class T>
void CPrintUnitStandardTable<T>::SetHeadingFont( int nPointSize, LPCTSTR lpszFaceName )
{
	DELETE_IF_NOT_NULL(m_pUserFontHeading);
	m_pUserFontHeading = new srtFont(nPointSize, lpszFaceName);

	SetNeedPreprocessSign(true);
}

template<class T>
void CPrintUnitStandardTable<T>::DefineColumns( vector<COLUMNDEFINITIONS>& columns )
{
	m_vecColumnDef.clear();
	m_vecColumnDef.resize(columns.size());
	copy(columns.begin(), columns.end(), m_vecColumnDef.begin());

	m_bNeedResetColumns = true;
	SetNeedPreprocessSign(true);
}

template<class T>
void CPrintUnitStandardTable<T>::InitPrintMetrics()
{
	PrepareMetrics();

	RealizeMetrics();
}

template<class T>
void CPrintUnitStandardTable<T>::CreatePrintFonts()
{
	PrepareDefaultFonts();

	if (m_pUserFontHeader)
	{
		CreateUserDefinedFont(m_fontHeader, m_pUserFontHeader);
	}
	if (m_pUserFontFooter)
	{
		CreateUserDefinedFont(m_fontFooter, m_pUserFontFooter);
	}
	if (m_pUserFontPrinter)
	{
		CreateUserDefinedFont(m_fontPairBody.fontPrinter, m_pUserFontPrinter);
	}
	if (m_pUserFontScreen)
	{
		CreateUserDefinedFont(m_fontPairBody.fontScreen, m_pUserFontScreen);
	}
	if (m_pUserFontHeading)
	{
		CreateUserDefinedFont(m_fontHeading, m_pUserFontHeading);
	}
}

template<class T>
void CPrintUnitStandardTable<T>::CompleteAllColHeadingsDefinition()
{
	if (m_bNeedResetColumns)
	{
		ClearColumnSet();
	}

	for (int i = 0; m_bNeedResetColumns && i < (int)m_vecColumnDef.size(); i++)
	{
		InsertPrintCol(i, m_vecColumnDef[i].strName.c_str(), m_vecColumnDef[i].fPct, m_vecColumnDef[i].nFormat);
	}

	m_bNeedResetColumns = false;

	// must call base class
	GPrintUnit::CompleteAllColHeadingsDefinition();
}

template<class T>
void CPrintUnitStandardTable<T>::CreateUserDefinedFont( CFont& fontDes, srtFont *fontSource )
{
	fontDes.DeleteObject();	
	fontDes.CreatePointFont(fontSource->nPointSize, fontSource->name.c_str(), &JDC);
}

template<class T>
CPrintUnitStandardTable<T>::~CPrintUnitStandardTable()
{
	DELETE_IF_NOT_NULL(m_pUserFontHeading);
}

template<class T>
BOOL CPrintUnitStandardTable<T>::SetPrintData( vector<vector<T*>> *data )
{
	if (data == NULL)
	{
		return FALSE;
	}
	m_pData = data;

	SetNeedPreprocessSign(true);

	return TRUE;
}

template<class T>
BOOL CPrintUnitStandardTable<T>::SetRowFormat( UINT nFormat )
{
	if (!(nFormat == DT_CENTER || nFormat == DT_LEFT || nFormat == DT_RIGHT))
	{
		return FALSE;
	}
	m_nRowFormat = nFormat;

	SetNeedPreprocessSign(true);

	return TRUE;
}

template<class T>
void CPrintUnitStandardTable<T>::DeleteDefaultFonts()
{
	m_fontHeading.DeleteObject();
	m_fontPairBody.fontPrinter.DeleteObject();
	m_fontPairBody.fontScreen.DeleteObject();
	m_fontHeader.DeleteObject();
	m_fontFooter.DeleteObject();
}

template<class T>
BOOL CPrintUnitStandardTable<T>::CheckCurrentDCCompatibleWithPrevious()
{
	if (m_pPreviousDC == NULL)
	{
		m_pPreviousDC = &JDC;	
		return false;
	}
	else
	{
		// check if compatible
		// TODO : replace the following code to check whether the m_pDC is compatible with 
		// the input dc. Aicro does not find that function. return the right value

		// return compatible? true:false;
		return false;
	}
}
