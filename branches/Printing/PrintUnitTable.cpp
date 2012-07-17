#include "stdafx.h"
#include "PrintUnitTable.h"

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

	m_totalPages = 0;
	m_bInsertCol = false;
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


	m_fontHeading.DeleteObject();
	m_fontHeading.CreatePointFontIndirect(&logFont, &JDC);
	
	m_fontPairBody.fontPrinter.DeleteObject();
	m_fontPairBody.fontPrinter.CreatePointFont(90, lpszFaceName, &JDC);
	
	m_fontPairBody.fontScreen.DeleteObject();
	m_fontPairBody.fontScreen.CreatePointFont(90, lpszFaceName);

	m_fontHeader.DeleteObject();
	m_fontHeader.CreatePointFont(110, _T("Garamond"), &JDC);//I18nOK
	
	m_fontFooter.DeleteObject();
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

void CPrintUnitStandardTable::DefineColumns( vector<COLUMNDEFINITIONS>& columns )
{
	m_vecColumnDef.clear();
	m_vecColumnDef.resize(columns.size());

	copy(columns.begin(), columns.end(), m_vecColumnDef.begin());
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
	for (int i = 0; !m_bInsertCol && i < (int)m_vecColumnDef.size(); i++)
	{
		InsertPrintCol(i, m_vecColumnDef[i].strName.c_str(), m_vecColumnDef[i].fPct, m_vecColumnDef[i].nFormat);
	}

	m_bInsertCol = true;

	// must call base class
	GPrintUnit::CompleteAllColHeadingsDefinition();
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

int CPrintUnitStandardTable::GetPageNum()
{
	// it is the user's responsible to check whether the column can match the data
	if (m_pData == NULL || m_vecColumnDef.size() == 0)
	{
		return -1;
	}

	Print();
	return JINFO.m_nCurPage - 1;
}

