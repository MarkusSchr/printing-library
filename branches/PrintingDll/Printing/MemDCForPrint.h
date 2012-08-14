#include "..\\PrintingDll\\stdafx.h"

#ifndef _MEMDC_H_
#define _MEMDC_H_

namespace Printing
{
	class CMemDCUsedForPrinter : public CDC {
	protected:
		CBitmap  m_bitmap;       // Offscreen bitmap
		CBitmap* m_oldBitmap;    // bitmap originally found in CMemDC
		CDC*     m_pDC;          // Saves CDC passed in constructor
		CRect    m_rect;         // Rectangle of drawing area. in logical coordinates 
		BOOL     m_bMemDC;       // TRUE if CDC really is a Memory DC.

		void Construct(CDC* pDC)
		{
			ASSERT(pDC != NULL); 

			// Some initialization
			m_pDC = pDC;
			m_oldBitmap = NULL;
			m_bMemDC = pDC->IsPrinting();

			if (m_bMemDC) {
				// Create a Memory DC
				CreateCompatibleDC(pDC);
				pDC->LPtoDP(&m_rect);

				m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
				m_oldBitmap = SelectObject(&m_bitmap);

				SetMapMode(pDC->GetMapMode());
				pDC->DPtoLP(&m_rect);
				SetWindowOrg(m_rect.left, m_rect.top);
			} else {
				// Make a copy of the relavent parts of the current DC for printing
				m_bPrinting = pDC->m_bPrinting;
				m_hDC       = pDC->m_hDC;
				m_hAttribDC = pDC->m_hAttribDC;
			}

			// Fill background 
			FillSolidRect(m_rect, pDC->GetBkColor());
		}

		// TRK begin
	public:
		CMemDCUsedForPrinter(CDC* pDC                  ) : CDC() { pDC->GetClipBox(&m_rect); Construct(pDC); }
		CMemDCUsedForPrinter(CDC* pDC, const RECT& rect) : CDC() { m_rect = rect           ; Construct(pDC); }
		// TRK end

		virtual ~CMemDCUsedForPrinter()
		{        
			if (m_bMemDC) {
				// Copy the offscreen bitmap onto the screen.
				m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
					this, m_rect.left, m_rect.top, SRCCOPY);            

				//Swap back the original bitmap.
				SelectObject(m_oldBitmap);        
			} else {
				// All we need to do is replace the DC with an illegal value,
				// this keeps us from accidently deleting the handles associated with
				// the CDC that was passed to the constructor.            
				m_hDC = m_hAttribDC = NULL;
			}    
		}

		// Allow usage as a pointer    
		CMemDCUsedForPrinter* operator->() 
		{
			return this;
		}    

		// Allow usage as a pointer    
		operator CMemDCUsedForPrinter*() 
		{
			return this;
		}
	};

	// the only difference between this MemDC and the CMemDC is CMyMemDC will not copy 
	// the DC's content to the original DC
	class CMemDcNotDraw : public CMemDCUsedForPrinter
	{
	public:
		CMemDcNotDraw(CDC* pDC) : CMemDCUsedForPrinter(pDC) {  }
		CMemDcNotDraw(CDC* pDC, const RECT& rect) : CMemDCUsedForPrinter(pDC, rect) {  }
		virtual ~CMemDcNotDraw()
		{   
			SelectObject(m_oldBitmap);
			m_bMemDC = false;
		}
	};
}


#endif

