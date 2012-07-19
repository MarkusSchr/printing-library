#include "StdAfx.h"
#include "MemDC.h"
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

	if(m_pPrinter)
		m_pPrinter->Print(&JDC, &JINFO);

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
		m_pPrinter->EndPrinting(&JDC, &JINFO);
}

int CPrintUnitFromDC::PreviewUnit( int from, int to )
{
	return Paint(from, to);
}
