#include "stdafx.h"

#include "PrintUnitTable.h"

Printing::CPrintUnitStandardTable::CPrintUnitStandardTable( Printing::GPrintJob *pJob )
	: Printing::GPrintUnit(pJob)
{
	m_bNeedHeaderSeparateLine = true;
	m_bNeedFooterSeperateLine = true;

	m_separateLineWidth = 2;
	m_separateLineInterval = 15;

	m_nRowFormat = DT_CENTER;

	m_pUserFontHeading = NULL;

	m_bNeedResetColumns = true;

	pages = 0;

	m_pPreviousDC = NULL;
}

void Printing::CPrintUnitStandardTable::SetHeadingFont( int nPointSize, LPCTSTR lpszFaceName )
{
	DELETE_IF_NOT_NULL(m_pUserFontHeading);
	m_pUserFontHeading = new srtFont(nPointSize, lpszFaceName);

	SetNeedPreprocessSign(true);
}

void Printing::CPrintUnitStandardTable::DefineColumns( vector<COLUMNDEFINITIONS>& columns )
{
	m_vecColumnDef.clear();
	m_vecColumnDef.resize(columns.size());
	copy(columns.begin(), columns.end(), m_vecColumnDef.begin());

	m_bNeedResetColumns = true;
	SetNeedPreprocessSign(true);
}

void Printing::CPrintUnitStandardTable::CreatePrintFonts()
{
	GPrintUnit::CreatePrintFonts();

	if (m_pUserFontHeading)
	{
		CreateUserDefinedFont(m_fontHeading, m_pUserFontHeading);
	}
}

void Printing::CPrintUnitStandardTable::DeleteDefaultFonts()
{
	m_fontHeading.DeleteObject();

	GPrintUnit::DeleteDefaultFonts();
}

void Printing::CPrintUnitStandardTable::CompleteAllColHeadingsDefinition()
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
	Printing::GPrintUnit::CompleteAllColHeadingsDefinition();
}

Printing::CPrintUnitStandardTable::~CPrintUnitStandardTable()
{
	DELETE_IF_NOT_NULL(m_pUserFontHeading);
	DELETE_IF_NOT_NULL(m_pFontTileSrt);
}

BOOL Printing::CPrintUnitStandardTable::SetRowFormat( UINT nFormat )
{
	if (!(nFormat == DT_CENTER || nFormat == DT_LEFT || nFormat == DT_RIGHT))
	{
		return FALSE;
	}
	m_nRowFormat = nFormat;

	SetNeedPreprocessSign(true);

	return TRUE;
}

BOOL Printing::CPrintUnitStandardTable::CheckCurrentDCCompatibleWithPrevious()
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