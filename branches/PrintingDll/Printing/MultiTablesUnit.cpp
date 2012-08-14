#include "StdAfx.h"
#define _AFXEXT

#include "MultiTablesUnit.h"
#include "MemDCForPrint.h"

Printing::CMultiTablesUnit::CMultiTablesUnit( Printing::GPrintJob *pJob /*= NULL*/ )
	:CPrintUnitStandardTableForString(pJob)
{
	m_intervalBetweenTables = 1;
	m_intervalBetweenFirstTable = 1;

	m_printPagesTotal = 0;
}

Printing::CMultiTablesUnit::~CMultiTablesUnit()
{
}

void Printing::CMultiTablesUnit::InsertTables( CPrintUnitMergableTable& table )
{
	m_vecTables.push_back(&table);
}

void Printing::CMultiTablesUnit::ClearTables()
{
	m_vecTables.clear();
}

int Printing::CMultiTablesUnit::PreviewUnit( BOOL bGetPageOnly, int from, int to )
{
	int printedPages = 1;

	CDC* oldDC = m_pJob->m_pDC;
	CMemDcNotDraw dc(&JDC);

	if (bGetPageOnly)
	{
		oldDC = &JDC;
		m_pJob->m_pDC = &dc;
	}

	EnvSetBeforePrinting();
	m_printPagesTotal = this->Paint(from, to);
	EnvCleanupAfterPrinting();
	
	if (bGetPageOnly)
	{
		m_pJob->m_pDC = oldDC;
	}
	
	return m_printPagesTotal;
}

int Printing::CMultiTablesUnit::Paint( int from, int to )
{
	bool bContinuePrinting = true;

	int beginPage = JINFO.m_nCurPage;

	SetPreprocessValue(false);
	m_bPrintThePage = true;

	int movedHeight = 0;
	
	int iTable = 0;
	bool bNeedPreview = true;

	int page = 1;
	int basePage = 1;

	CDC* oldDC = m_pJob->m_pDC;
	CMemDcNotDraw dc(&JDC);

	for (; page <= to && iTable != m_vecTables.size(); page++)
	{
		// pass the unnecessary pages
		if (page < from)
		{
			oldDC = &JDC;
			m_pJob->m_pDC = &dc;
		}
		else
		{
			m_pJob->m_pDC = oldDC;
		}

		// begin to draw
		StartPage();
		// adjust the top margin
		JRECT.top += m_intervalBetweenFirstTable * m_pum.pumLineOfText;

		CRect oldRect = JRECT;
		bool bReachPageBottom = false;
		while (!bReachPageBottom && iTable != m_vecTables.size())
		{
			// we have not reached the bottom of the page

			// print title
			m_vecTables[iTable]->SetPrintFont(&m_fontPairBody.fontPrinter);
			m_vecTables[iTable]->m_pJob->m_ptCursor = JRECT.TopLeft();
			if (bNeedPreview || m_bNeedPrintTitleExcpetFirstPage)
			{
				// the first time to print this table
				movedHeight = m_vecTables[iTable]->PrintTitleAndMoveCursor(!bNeedPreview);
			}
			JRECT.top += movedHeight;

			if (bNeedPreview)
			{
				m_vecTables[iTable]->BeginPrinting(&JDC, &JINFO, JRECT);
				bNeedPreview = false;
			}

			PrintEndResult printEndResult;
			m_vecTables[iTable]->Paint(&JDC, page - basePage + 1, JRECT, &printEndResult); 

			if (printEndResult.bEndOfTable)
			{
				iTable++;
				basePage = page;

				bNeedPreview = true;
			}

			bReachPageBottom = printEndResult.bEndOfPage;

			// print another table
			JRECT.top = printEndResult.pixelOfBottom;
			JRECT.top += m_intervalBetweenTables * m_pum.pumLineOfText;
		}

		// we need to change the JCUR back
		JRECT = oldRect;
		JRECT.top -= m_intervalBetweenFirstTable * m_pum.pumLineOfText;

		EndPage();

		if (page == m_printPagesTotal)
		{
			bContinuePrinting = false;
		}
	}

	return page - from;	
}

void Printing::CMultiTablesUnit::SetTableIntervalInLineOfText( int intervalInLineOfText )
{
	if (intervalInLineOfText <= 1)
	{
		m_intervalBetweenTables = 1;
	}
	else
	{
		m_intervalBetweenTables = intervalInLineOfText;
	}
}

void Printing::CMultiTablesUnit::SetIntervalBetweenFirstTableInLineOfText( int intervalInLineOfText )
{
	if (intervalInLineOfText <= 1)
	{
		m_intervalBetweenFirstTable = 1;
	}
	else
	{
		m_intervalBetweenFirstTable = intervalInLineOfText;
	}
}

void Printing::CMultiTablesUnit::CreatePrintFonts()
{
	// for the unit itself
	CPrintUnitStandardTableForString::CreatePrintFonts();

	for (int i = 0; i < m_vecTables.size(); i++)
	{
		m_vecTables[i]->m_pJob = this->m_pJob;
		m_vecTables[i]->CreatePrintFonts();
	}
}

void Printing::CMultiTablesUnit::InitPrintMetrics()
{
	// for the unit itself
	CPrintUnitStandardTableForString::InitPrintMetrics();

	// for each table
	for (int i = 0; i < m_vecTables.size(); i++)
	{
		m_vecTables[i]->m_pJob = this->m_pJob;
		m_vecTables[i]->InitPrintMetrics();
	}
}

void Printing::CMultiTablesUnit::DeleteDefaultFonts()
{
	// for the unit itself
	CPrintUnitStandardTableForString::DeleteDefaultFonts();

	for (int i = 0; i < m_vecTables.size(); i++)
	{
		m_vecTables[i]->m_pJob = this->m_pJob;
		m_vecTables[i]->DeleteDefaultFonts();
	}
}

void Printing::CMultiTablesUnit::CompleteAllColHeadingsDefinition()
{
	CPrintUnitStandardTableForString::CompleteAllColHeadingsDefinition();

	for (int i = 0; i < m_vecTables.size(); i++)
	{
		m_vecTables[i]->m_pJob = this->m_pJob;
		m_vecTables[i]->CompleteAllColHeadingsDefinition();
	}
}
