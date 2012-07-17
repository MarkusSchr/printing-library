#include "stdafx.h"
#include "PrintUnitTable.h"

#define SEPATATELINE_WIDTH_MAX 5

CPrintUnitStandardTable::CPrintUnitStandardTable( GPrintJob *pJob )
	: GPrintUnit(pJob)
{
	m_bNeedHeaderSeparateLine = true;
	m_bNeedFooterSeperateLine = true;

	m_separateLineWidth = 2;
	m_separateLineInterval = 15;

	m_pData = NULL;
	m_nRowFormat = DT_CENTER;

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
		m_pum.pumHeadingHeight = tm.tmHeight;
	}

	// left and right margin
	// both the margin is 1/20 of the page
	{
		m_pum.pumLeftMarginWidth = (JRECTDEV.right - JRECTDEV.left)/20;
		m_pum.pumRightMarginWidth = m_pum.pumLeftMarginWidth;
	}
}

void CPrintUnitStandardTable::SetHeadingFont( int nPointSize, LPCTSTR lpszFaceName )
{
	if (m_pUserFontHeading)
	{
		delete m_pUserFontHeading;
	}

	m_pUserFontHeading = new srtFont(nPointSize, lpszFaceName);
}

void CPrintUnitStandardTable::SetBodyPrinterFont( int nPointSize, LPCTSTR lpszFaceName )
{
	if (m_pUserFontPrinter)
	{
		delete m_pUserFontPrinter;
	}

	m_pUserFontPrinter = new srtFont(nPointSize, lpszFaceName);
}

void CPrintUnitStandardTable::SetBodyScreenFont( int nPointSize, LPCTSTR lpszFaceName )
{
	if (m_pUserFontScreen)
	{
		delete m_pUserFontScreen;
	}

	m_pUserFontScreen = new srtFont(nPointSize, lpszFaceName);
}

void CPrintUnitStandardTable::SetHeaderFont( int nPointSize, LPCTSTR lpszFaceName )
{
	if (m_pUserFontHeader)
	{
		delete m_pUserFontHeader;
	}

	m_pUserFontHeader = new srtFont(nPointSize, lpszFaceName);
}

void CPrintUnitStandardTable::SetFooterFont( int nPointSize, LPCTSTR lpszFaceName )
{
	if (m_pUserFontFooter)
	{
		delete m_pUserFontFooter;
	}

	m_pUserFontFooter = new srtFont(nPointSize, lpszFaceName);
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

void CPrintUnitStandardTable::SetHeader( HEADERDEFINITIONS *header, int size )
{
	int minSize = MAX_HEADER_COUNT >= size? size : MAX_HEADER_COUNT;
	for (int i = 0; i < minSize; i++)
	{
		memcpy(&m_header[i], &header[i], sizeof(HEADERDEFINITIONS));	
	}
}

void CPrintUnitStandardTable::SetFooter( FOOTERDEFINITIONS *footer, int size )
{
	int minSize = MAX_FOOTER_COUNT >= size? size : MAX_FOOTER_COUNT;
	for (int i = 0; i < minSize; i++)
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

	wchar_t step[MAX_HEADER_COUNT] = {HFC_CENTER, HFC_RIGHTJUSTIFY, _T('')};

	CString strHeader;
	for (int i = 0; i < MAX_HEADER_COUNT; i++)
	{
		GetContentOnType(m_header[i].type, m_header[i].content.c_str(), strHeader);
		strHeader += step[i];
	}
	strHeader.Delete(strHeader.GetLength(), 1);

	strHeader += HFC_NEWLINE;

	PrintHeaderText(strHeader);

	// draw the separate line
	if (m_bNeedHeaderSeparateLine)
	{
		DrawSeparetLine(TRUE);
	}
}

void CPrintUnitStandardTable::PrintFooter()
{
	GSELECT_OBJECT(&JDC, &m_fontFooter);

	wchar_t step[MAX_HEADER_COUNT] = {HFC_CENTER, HFC_RIGHTJUSTIFY, _T('')};

	CString strFooter;
	// draw the separate line
	if (m_bNeedFooterSeperateLine)
	{
		DrawSeparetLine(FALSE);
	}

	strFooter += HFC_NEWLINE;
	for (int i = 0; i < MAX_FOOTER_COUNT; i++)
	{
		GetContentOnType(m_footer[i].type, m_footer[i].content.c_str(), strFooter);
		strFooter += step[i];
	}

	PrintFooterText(strFooter);
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
	GetDateFormat(LOCALE_USER_DEFAULT, NULL, &sysTime, NULL, szBuf, sizeof(szBuf));
	date = szBuf;	
}

void CPrintUnitStandardTable::GetContentOnType( int type, CString context, CString& str )
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
			str = str + context + strPage;
		}
		break;
	case TYPE_DATE: // the current date, using "content" as prefix.
		{
			CString date, time;
			GetCurrentTimeAndDate(date, time);
			str = str + context + date;
		}
		break;
	case TYPE_TIME: // the current time, using "content" as prefix.
		{
			CString date, time;
			GetCurrentTimeAndDate(date, time);
			str = str +context + time;
		}
		break;
	case TYPE_DATETIME: // the current date and time, using "content" as prefix.
		{
			CString date, time;
			GetCurrentTimeAndDate(date, time);
			str = str + context + date + " " + time;
		}
		break;
	case TYPE_DATA:  // user-defined data, it will use "content"
		{
			str = str + context;
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
	if (!(SetPrintData(pPrintData) && SetRowFormat(nRowFormat)))
	{
		return FALSE;
	}

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

void CPrintUnitStandardTable::CreateUserDefinedFont( CFont& fontDes, srtFont *fontSource )
{
	fontDes.DeleteObject();	
	fontDes.CreatePointFont(fontSource->nPointSize, fontSource->name.c_str(), &JDC);
}

CPrintUnitStandardTable::~CPrintUnitStandardTable()
{
}

BOOL CPrintUnitStandardTable::SetPrintData( vector<vector<LPCTSTR>> *data )
{
	if (data == NULL)
	{
		return FALSE;
	}
	m_pData = data;
	return TRUE;
}

BOOL CPrintUnitStandardTable::SetRowFormat( UINT nFormat )
{
	if (!(nFormat & DT_CENTER || nFormat & DT_LEFT || nFormat & DT_RIGHT))
	{
		return FALSE;
	}
	m_nRowFormat = nFormat;
	return TRUE;
}

UINT CPrintUnitStandardTable::SetSeparateLineInterval( UINT interval )
{
	UINT old = m_separateLineInterval;
	m_separateLineInterval = interval;
	return old;
}

UINT CPrintUnitStandardTable::SetSeparateLineWidth( UINT width )
{
	if (width > SEPATATELINE_WIDTH_MAX)
	{
		width = SEPATATELINE_WIDTH_MAX;
	}
	UINT old = m_separateLineWidth;
	m_separateLineWidth = width;
	return old;
}

void CPrintUnitStandardTable::DrawSeparetLine( BOOL bHeader )
{
	// adjust the separeteline interval in case the user has entered an invalid value
	if (bHeader == TRUE && m_separateLineInterval >= m_pum.pumHeaderLineHeight)
	{
		m_separateLineInterval = m_pum.pumHeaderLineHeight;
	}
	else if(bHeader == FALSE && m_separateLineInterval >= m_pum.pumFooterLineHeight)
	{
		m_separateLineInterval = m_pum.pumFooterLineHeight;
	}

	CPen pen;
	pen.CreatePen(PS_SOLID, m_separateLineWidth, RGB(0,0,0));
	GSELECT_OBJECT(&JDC, &pen);

	int linePosY;
	if (bHeader == TRUE)
	{
		linePosY = JRECT.top - m_separateLineInterval;
	}
	else
	{
		linePosY = JRECT.bottom + m_separateLineInterval;
	}

	JDC.MoveTo(JCUR.x, linePosY);
	JDC.LineTo(JCUR.x + JRECT.Width(), linePosY);
}
