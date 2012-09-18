#pragma once
#include "stdafx.h"

#include "gfx_printjob.h"
#include "gfx_printunit.h"

using namespace Printing;

namespace Printing
{
	class CUserPaintingExampleUnit : public GPrintUnit
	{
	public:
		CUserPaintingExampleUnit(GPrintJob *pJob = NULL)
			:GPrintUnit(pJob)
		{
		}
		~CUserPaintingExampleUnit(){}

	private:
		virtual int PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to)
		{
			// if you want to draw the page out, rather than calculation, you must make the 
			// following two signs available
			SetPreprocessValue(false);
			m_bPrintThePage = true;

			// prepare painting
			EnvSetBeforePrinting();

			// call Paint()
			int printedPages = Paint(from, to);

			EnvCleanupAfterPrinting();

			// only one page
			return printedPages; // attention that, this value must be the same as the return value of EveluateUnitPages
		}

		// Paint will not be called by the frame, it is just a method for you to place your printing codes
		// Generally, Paint() should be called in PreviewUnit()
		virtual int Paint(int from, int to)
		{
			// you must call the StartPage() and EndPage() in order to print the header and footer
			StartPage();

			// draw your defined data here
			// the painting area are restricted in JRECT
			CBrush brush(RGB(0,255,0));
			GSELECT_OBJECT(&JDC, &brush);

			JDC.Rectangle(JRECT.left, JRECT.top + 10, JRECT.right, JRECT.bottom - 10);

			EndPage();

			// only paint one page
			return 1;
		}

		virtual int EveluateUnitPages(CDC* pOriginDC, int from, int to)
		{
			// return the pages this unit will paint
			// only one page
			return 1;
		}

		// called when the print job is ready to begin
		virtual void OnBeginPrinting(){}
		virtual void OnEndPrinting(){}
	};
}
