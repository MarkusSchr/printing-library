#include "stdafx.h"
#include "PrintUnitTable.h"

CPrintUnitStandardTable::CPrintUnitStandardTable( GPrintJob *pJob )
	: GPrintUnit(pJob)
{
	m_bNeedHeaderSeparateLine = true;
	m_bNeedFooterSeperateLine = true;

	m_pData = NULL;
	m_nRowFormat = DT_CENTER | DT_WORD_ELLIPSIS;

	m_pUserFontHeader = NULL;
	m_pUserFontFooter = NULL;
	m_pUserFontPrinter = NULL;
	m_pUserFontScreen = NULL;
	m_pUserFontHeading = NULL;
}

void CPrintUnitStandardTable::PrepareDefaultFonts()
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

void CPrintUnitStandardTable::PrepareMetrics()
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
		m_pum.pumHeadingHeight = m_pum.pumLineOfText;
	}

	// left and right margin
	// both the margin is 1/20 of the page
	{
		m_pum.pumLeftMarginWidth = (JRECTDEV.right - JRECTDEV.left)/20;
		m_pum.pumRightMarginWidth = m_pum.pumLeftMarginWidth;
	}
}

void CPrintUnitStandardTable::SetHeadingFont( CFont* font )
{
	m_pUserFontHeading = font;
}

void CPrintUnitStandardTable::SetBodyFont( CFont* printerFont, CFont* screenFont )
{
	m_pUserFontPrinter = printerFont;
	m_pUserFontScreen = screenFont;
}

void CPrintUnitStandardTable::SetHeaderFont( CFont* font )
{
	m_pUserFontHeader = font;
}

void CPrintUnitStandardTable::SetFooterFont( CFont* font )
{
	m_pUserFontFooter = font;
}

void CPrintUnitStandardTable::SetMetrics( PRINTUNITMETRICS pum )
{
	memcpy(&m_pum, &pum, sizeof(PRINTUNITMETRICS));
}

bool CPrintUnitStandardTable::NeedHeaderLine( bool bNeedHeaderLine /*= true*/ )
{
	bool bOld = m_bNeedHeaderSeparateLine;
	m_bNeedHeaderSeparateLine = bNeedHeaderLine;
	return bOld;
}

bool CPrintUnitStandardTable::NeedFooterLine( bool bNeedFooterLine /*= true*/ )
{
	bool bOld = m_bNeedFooterSeperateLine;
	m_bNeedFooterSeperateLine = bNeedFooterLine;
	return bOld;
}

void CPrintUnitStandardTable::DefineColumns( vector<COLUMNDEFINITIONS>& columns )
{
	m_vecColumnDef.clear();
	m_vecColumnDef.resize(columns.size());

	copy(columns.begin(), columns.end(), m_vecColumnDef.begin());
}

void CPrintUnitStandardTable::SetHeader( HEADERDEFINITIONS *header )
{
	for (int i = 0; i < MAX_HEADER_COUNT; i++)
	{
		memcpy(&m_header[i], &header[i], sizeof(HEADERDEFINITIONS));	
	}
}

void CPrintUnitStandardTable::SetFooter( FOOTERDEFINITIONS *footer )
{
	for (int i = 0; i < MAX_FOOTER_COUNT; i++)
	{
		memcpy(&m_footer[i], &footer[i], sizeof(FOOTERDEFINITIONS));	
	}
}

void CPrintUnitStandardTable::InitPrintMetrics()
{
	PrepareMetrics();

	RealizeMetrics();
}

void CPrintUnitStandardTable::CreatePrintFonts()
{
	PrepareDefaultFonts();

	if (m_pUserFontHeader)
	{
		CopyFont(m_fontHeader, *m_pUserFontHeader);
	}
	if (m_pUserFontFooter)
	{
		CopyFont(m_fontFooter, *m_pUserFontFooter);
	}
	if (m_pUserFontPrinter)
	{
		CopyFont(m_fontPairBody.fontPrinter, *m_pUserFontPrinter);
	}
	if (m_pUserFontScreen)
	{
		CopyFont(m_fontPairBody.fontScreen, *m_pUserFontScreen);
	}
	if (m_pUserFontHeading)
	{
		CopyFont(m_fontHeading, *m_pUserFontHeading);
	}
}

void CPrintUnitStandardTable::CompleteAllColHeadingsDefinition()
{
	for (int i = 0; i < (int)m_vecColumnDef.size(); i++)
	{
		InsertPrintCol(i, m_vecColumnDef[i].strName.c_str(), m_vecColumnDef[i].fPct, m_vecColumnDef[i].nFormat);
	}

	// must call base class
	GPrintUnit::CompleteAllColHeadingsDefinition();
}

void CPrintUnitStandardTable::PrintHeader()
{
	GSELECT_OBJECT(&JDC, &m_fontHeader);

	wchar_t step[MAX_HEADER_COUNT] = {_T(''), HFC_CENTER, HFC_RIGHTJUSTIFY};

	CString strHeader = TEXT("");
	for (int i = 0; i < MAX_HEADER_COUNT; i++)
	{
		TCHAR szBuf[100];
		GMakeStringFillZero(szBuf);

		strHeader.Append(&step[i]);
		GetContentOnType(m_header[i].type, m_header[i].content.c_str(), strHeader);
	}

	// draw the separate line
	if (m_bNeedHeaderSeparateLine)
	{
		strHeader += HFC_NEWLINE;
		strHeader += HFC_LINES;
	}

	PrintHeaderText(strHeader);
}

void CPrintUnitStandardTable::PrintFooter()
{
	GSELECT_OBJECT(&JDC, &m_fontFooter);

	wchar_t step[MAX_HEADER_COUNT] = {_T(''), HFC_CENTER, HFC_RIGHTJUSTIFY};

	CString strFooter = TEXT("");
	// draw the separate line
	if (m_bNeedFooterSeperateLine)
	{
		strFooter += HFC_LINES;
	}

	for (int i = 0; i < MAX_FOOTER_COUNT; i++)
	{
		TCHAR szBuf[100];
		GMakeStringFillZero(szBuf);

		strFooter.Append(&step[i]);
		GetContentOnType(m_footer[i].type, m_footer[i].content.c_str(), strFooter);
	}

	PrintHeaderText(strFooter);
}

void CPrintUnitStandardTable::GetCurrentTimeAndDate( CString& date, CString& time )
{
	// get the current time
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	// format it...
	TCHAR szBuf[100];
	GMakeStringFillZero(szBuf);
	// get the time
	GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &sysTime, NULL, szBuf, sizeof(szBuf));
	time = szBuf;
	// get the date
	GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &sysTime, NULL, szBuf, sizeof(szBuf));
	date = szBuf;	
}

void CPrintUnitStandardTable::GetContentOnType( int type, CString context, CString& strHeader )
{
	switch (type)
	{
	case TYPE_EMPTY:
		{	
		}
		break;
	case TYPE_PAGE: // print current page, using "content" as prefix.
		{
			CString strPage;
			strPage.Format(TEXT("%d"), JINFO.m_nCurPage);
			strHeader = strHeader + context + strPage;
		}
		break;
	case TYPE_DATE: // the current date, using "content" as prefix.
		{
			CString date, time;
			GetCurrentTimeAndDate(date, time);
			strHeader = strHeader + context + date;
		}
		break;
	case TYPE_TIME: // the current time, using "content" as prefix.
		{
			CString date, time;
			GetCurrentTimeAndDate(date, time);
			strHeader = strHeader +context + time;
		}
		break;
	case TYPE_DATETIME: // the current date and time, using "content" as prefix.
		{
			CString date, time;
			GetCurrentTimeAndDate(date, time);
			strHeader = strHeader + context + date + " " + time;
		}
		break;
	case TYPE_DATA:  // user-defined data, it will use "content"
		{
			strHeader = strHeader + context;
		}
		break;
	}	
}

BOOL CPrintUnitStandardTable::Print( vector<vector<LPCTSTR>>* pPrintData, UINT nRowFormat )
{
	if (m_vecColumnDef.size() == 0 || pPrintData == NULL)
	{
		// not define columns yet
		return FALSE;
	}

	// it is the user's responsibility to ensure the data in each row 
	// equals to the number of the column
	m_pData = pPrintData;
	m_nRowFormat = nRowFormat;

	Print();

	return TRUE;
}

BOOL CPrintUnitStandardTable::Print()
{
	if (m_pData == NULL || m_vecColumnDef.size() == 0)
	{
		return FALSE;
	}

	GPrintUnit::Print();

	PrintTableContents(m_pData, m_nRowFormat);

	return TRUE;
}

void CPrintUnitStandardTable::CopyFont( CFont& fontDes, CFont& fontSource )
{
	// should delete the fontDes first
	fontDes.DeleteObject();
	
	LOGFONT lf;
	fontSource.GetLogFont(&lf);
	fontDes.CreateFontIndirect(&lf);
}

CPrintUnitStandardTable::~CPrintUnitStandardTable()
{
}

void CPrintUnitStandardTable::SetPrintData(vector<vector<LPCTSTR>> *data)
{
	m_pData = data;
}

void CPrintUnitStandardTable::SetRowFormat( UINT nFormat )
{
	m_nRowFormat = nFormat;
}
