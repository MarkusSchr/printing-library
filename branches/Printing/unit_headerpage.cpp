
#include "stdafx.h"
#include "unit_headerpage.h"
#include "gfx_printjob.h"


HeaderPage::HeaderPage(GPrintJob *pJob) : GPrintUnit(pJob)
{
}

HeaderPage::~HeaderPage()
{
}

BOOL HeaderPage::Print()
{
   GPrintUnit::Print();

   StartPage();

   CFont font;
   font.CreatePointFont(480, TEXT("Arial"), &JDC);
   GSELECT_OBJECT(&JDC, &font);

   CString str("The Header Page");

   CSize size = JDC.GetTextExtent(str);

   CRect rect;
   rect.left = (JRECT.Width() - size.cx)/2;
   rect.right = rect.left + size.cx;
   rect.top = (JRECT.Height() - size.cy)/2;
   rect.bottom = rect.top + size.cy;
    
   JDC.DrawText(str, &rect, DT_SINGLELINE);

   EndPage();

   return TRUE;
}


