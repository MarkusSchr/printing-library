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
}

int CPrintUnitFromDC::PreviewUnit( CDC* pOriginDC, BOOL bGetPageOnly, int from, int to )
{
	return Paint(from, to);
}
