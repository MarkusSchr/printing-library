#include "stdafx.h"
#include "MergableTableUnit.h"

CMergableTableUnit::CMergableTableUnit(GPrintJob *pJob)
	:CPrintUnitMergableTable(pJob)
{
	m_printPagesTotal = 0;
}

CMergableTableUnit::~CMergableTableUnit(void)
{
}

int CMergableTableUnit::PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to )
{
	// we will use calculation rather than virtual draw to calc the pages, 
	// so set back the dc
	CDC* old = m_pJob->m_pDC;
	m_pJob->m_pDC = pOriginDC;

	int printedPages = m_printPagesTotal;
	
	EnvSetBeforePrinting();
	if (bGetPageOnly == TRUE)
	{
		CPrintUnitMergableTable::SetPrintFont(&m_fontPairBody.fontPrinter);
		
		// preview how many pages do we need
		m_printPagesTotal = CPrintUnitMergableTable::BeginPrinting(&JDC, &JINFO, JRECT);
		
		if (from > m_printPagesTotal)
		{
			printedPages = 0;
		}
		else if (to <= m_printPagesTotal)
		{
			// from <= m_needToPrintPages && to <= m_needToPrintPages
			printedPages = to - from + 1;
		}
		else
		{
			// from <= m_needToPrintPages && to > m_needToPrintPages
			printedPages = m_printPagesTotal - from + 1;
		}
	}
	else 
	{
		printedPages = Paint(from, to);
	}
	EnvCleanupAfterPrinting();

	m_pJob->m_pDC = old;

	return printedPages;
}

int CMergableTableUnit::Paint( int from, int to )
{
	ASSERT(m_printPagesTotal != 0);

	bool bContinuePrinting = true;

	int beginPage = JINFO.m_nCurPage;

	SetPreprocessValue(false);
	m_bPrintThePage = true;

	int movedHeight = 0;
	int page = from;
	for (; page <= to && bContinuePrinting; page++)
	{
		StartPage();      
		
		// print title
		if (page == 1 || m_bNeedPrintTitleExcpetFirstPage)
		{
			movedHeight = PrintTitleAndMoveCursor(!(page == 1));
			// this unit is special for not using JCUR.y to print, we need to adjust JRECT instead
			JRECT.top += movedHeight;
		}

		CPrintUnitMergableTable::Paint(&JDC, page, JRECT);

		// we need to change the JCUR back
		if (movedHeight != 0)
		{
			JRECT.top -= movedHeight;
			movedHeight = 0;
		}

		EndPage();

		if (page == m_printPagesTotal)
		{
			bContinuePrinting = false;
		}
	}

	return page - from;
}


