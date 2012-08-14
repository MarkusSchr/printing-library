#include "stdafx.h"

#define _AFXEXT

#include "DataTableUnit.h"

Printing::CDataTableUnit::CDataTableUnit(GPrintJob *pJob)
	: CPrintUnitStandardTableForString(pJob)
{
}

Printing::CDataTableUnit::~CDataTableUnit(void)
{
}

int Printing::CDataTableUnit::PreviewUnit( BOOL bGetPageOnly, int from, int to )
{
	// it is the user's responsible to check whether the column can match the data
	if (m_pData == NULL || m_vecColumnDef.size() == 0 || from > to)
	{
		return -1;
	}

	// it is necessary to call this function if we want to use the column-cross-page table
	SetNeedPreprocessSign(!CheckCurrentDCCompatibleWithPrevious());

	return Paint(from, to);
}

int Printing::CDataTableUnit::Paint( int from, int to )
{
	if (m_pData == NULL || m_vecColumnDef.size() == 0)
	{
		return FALSE;
	}

	EnvSetBeforePrinting();

	int printedPages = PrintTableContents(m_pData, m_nRowFormat, from, to);

	EnvCleanupAfterPrinting();

	return printedPages;
}