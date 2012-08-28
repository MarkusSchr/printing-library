#include "stdafx.h"
#include "DataTableUnit.h"
#include "MemDC.h"

Printing::CDataTableUnit::CDataTableUnit(GPrintJob *pJob)
	: CPrintUnitStandardTable<CONST WCHAR>(pJob)
{
}

Printing::CDataTableUnit::~CDataTableUnit(void)
{
}

int Printing::CDataTableUnit::PreviewUnit( CDC* pOriginDC, BOOL bGetPageOnly, int from, int to )
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

int Printing::CDataTableUnit::EveluateUnitPages( CDC* pOriginDC, int from, int to )
{
	int pages = 1;

	// prepare work
	EnvSetBeforePrinting();
	m_preprocessRowStartPosY.resize(m_pData->size());
	m_preprocessRowHeight.resize(m_pData->size());

	// save the old ones
	CPoint oldCur = JCUR;
	int nRows = m_pData->size();
	int rowHeight = m_restrictedRowHeightInTextLine * m_pum.pumLineOfText;
	
	// begin test
	int offsetY = JRECT.TopLeft().y;
	SetPreprocessValue(true);
	// first page must print the title
	// pass the title and title if need to print
	PrintTitleAndMoveCursor(false);
	offsetY = JCUR.y;
	int printedRows = 0;
	while(printedRows < nRows)
	{
		// move heading
		offsetY += m_headingHeightInTextLine * m_pum.pumLineOfText;
		// now the offsetY is where we can print the data
		int canContainRows = (double)(JRECT.bottom - offsetY) / (double)rowHeight;
		// now set the preprocess results
		// pre-process the row starting position
		int i = 0;
		for (; i < canContainRows && printedRows + i < nRows; i++)
		{
			m_preprocessRowStartPosY[printedRows + i].rowHeight = offsetY + i * rowHeight;
			m_preprocessRowStartPosY[printedRows + i].overflowHeight = 0;
			m_preprocessRowHeight[printedRows + i].rowHeight = rowHeight;
			m_preprocessRowHeight[printedRows + i].overflowHeight = 0;
		}

		if (printedRows + i < nRows)
		{
			// begin the next page
			pages++;
			offsetY = JRECT.TopLeft().y;
			if (m_bNeedPrintTitleExcpetFirstPage)
			{
				// print the title
				PrintTitleAndMoveCursor(m_bNeedPrintTitleExcpetFirstPage && !GetPreprocessValue());
			}
		}

		printedRows += canContainRows;
	}

	// if this table's columns exceed the a single page, adjust them
	pages *= m_vecColumnPage.size();

	m_bCheckPosition = false;
	SetPreprocessValue(false);

	EnvCleanupAfterPrinting();
	JCUR = oldCur;


	return pages;
}
