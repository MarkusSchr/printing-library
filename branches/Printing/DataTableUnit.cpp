#include "stdafx.h"
#include "DataTableUnit.h"

CDataTableUnit::CDataTableUnit(GPrintJob *pJob)
	: CPrintUnitStandardTable<CONST WCHAR>(pJob)
{
}

CDataTableUnit::~CDataTableUnit(void)
{
}

int CDataTableUnit::PreviewUnit( int from, int to )
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

int CDataTableUnit::Paint( int from, int to )
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