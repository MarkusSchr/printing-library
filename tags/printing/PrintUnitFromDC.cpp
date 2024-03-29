#include "StdAfx.h"
#include "MemDCForPrint.h"
#include "PrintUnitFromDC.h"


CPrintUnitFromDC::CPrintUnitFromDC(GPrintJob *pJob)
	:GPrintUnit(pJob)
{
	m_pPrinter = new CHSEPrinter;
}

CPrintUnitFromDC::~CPrintUnitFromDC()
{
	DELETE_IF_NOT_NULL(m_pPrinter);
}

int CPrintUnitFromDC::Paint( int from, int to )
{
	int beginPage = JINFO.m_nCurPage;
	
	EnvSetBeforePrinting();

	// the following is all controlled by yourself
	SetPreprocessValue(false);
	
	// because this example only print one page, so just test form == 1 && to >= 1
	if (from == 1 && to >= from)
	{
		m_bPrintThePage = true;
	}

	// attention that, it is the m_pPrinter that blocks print out the header and footer
	// you can delete all the method invoking of m_pPrinter to test
	StartPage();
	if(m_pPrinter)
	{
		m_pPrinter->Print(&JDC, &JINFO);
	}

	EndPage();
	
	EnvCleanupAfterPrinting();

	return JINFO.m_nCurPage - beginPage;
}

void CPrintUnitFromDC::OnBeginPrinting()
{
	// transfer the printing mapping mode, from MM_TEXT to MM_LOMETRIC
	CRect rect = JINFO.m_rectDraw;

	CRect rect2(0,0,1977,-2868);
	JDC.SetMapMode(MM_LOMETRIC);
	// JDC.LPtoDP(rect);
	JDC.DPtoLP(rect);
	JINFO.m_rectDraw = rect;


	if(m_pPrinter)
	{
		m_pPrinter->PrepareDC(&JDC, &JINFO);
		m_pPrinter->PreparePrinting(&JINFO);
		m_pPrinter->BeginPrinting(&JDC, &JINFO);
	}
}

void CPrintUnitFromDC::OnEndPrinting()
{
	if(m_pPrinter)
	{
		m_pPrinter->EndPrinting(&JDC, &JINFO);
	}

	// JDC.LPtoDP(rect);
	JDC.LPtoDP(JINFO.m_rectDraw);

	JDC.SetMapMode(MM_TEXT);
}

int CPrintUnitFromDC::PreviewUnit( CDC* pOriginDC, BOOL bGetPageOnly, int from, int to )
{
	int printedPages = Paint(from, to);
	return printedPages;
}

int CPrintUnitFromDC::EveluateUnitPages( CDC* pOriginDC, int from, int to )
{
	return 1;
}
