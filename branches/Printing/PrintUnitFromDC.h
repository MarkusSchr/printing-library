#pragma once
#include "StdAfx.h"


#include "gfx_printjob.h"
#include "gfx_printunit.h"
#include "HSECPrintPageConfig.h"
#include "HSEPrinter.h"

#include "MemDC.h"

using namespace Printing;
class CPrintUnitFromDC : public Printing::GPrintUnit
{
public:
	CPrintUnitFromDC(GPrintJob *pJob = NULL);
	~CPrintUnitFromDC();

private:
	// printing
	virtual int PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to);
	virtual int Paint(int from, int to);
	virtual int EveluateUnitPages(CDC* pOriginDC, int from, int to);

	// called when the print job is ready to begin
	virtual void OnBeginPrinting();
	virtual void OnEndPrinting();

private:
	CHSEPrinter* m_pPrinter;

	CMemDCUsedForPrinter* dc;
	CDC * pOld;
};