
#include "stdafx.h"
#include "unit_table1.h"
#include "gfx_printjob.h"



Table1Unit::Table1Unit(GPrintJob *pJob) : GPrintUnit(pJob)
{
}

Table1Unit::~Table1Unit()
{
}

void Table1Unit::CompleteAllColHeadingsStartPosition()
{
   // define my four columns...percentages should all up to 1.00
   InsertPrintCol(0, "Part Number", 0.45);
   InsertPrintCol(1, "Description", 0.30);
   InsertPrintCol(2, "Qty.", 0.10);
   InsertPrintCol(3, "Cost", 0.15);

   // must call base class
   GPrintUnit::CompleteAllColHeadingsStartPosition();
}

void Table1Unit::CreatePrintFonts()
{
   LOGFONT logFont;
   GMakeStructFillZero(logFont);

	LPCTSTR lpszFaceName = _T("Arial");//I18nok

   logFont.lfCharSet = DEFAULT_CHARSET;
   logFont.lfHeight = 90;
   lstrcpyn(logFont.lfFaceName, lpszFaceName, GGetArrayElementCount(logFont.lfFaceName));
   logFont.lfWeight = FW_BOLD;

   m_fontHeading.CreatePointFontIndirect(&logFont, &JDC);
   m_fontPairBody.fontPrinter.CreatePointFont(90, lpszFaceName, &JDC);
   m_fontPairBody.fontScreen.CreatePointFont(90, lpszFaceName);

   m_fontHeader.CreatePointFont(110, _T("Garamond"), &JDC);//I18nOK
   m_fontFooter.CreatePointFont(90, _T("Garamond"), &JDC);//I18nOK
}


void Table1Unit::InitPrintMetrics()
{
   TEXTMETRIC tm;

   {
      GSELECT_OBJECT(&JDC, &m_fontHeader);
      JDC.GetTextMetrics(&tm);

      m_pum.pumHeaderHeight = tm.tmHeight * 2;
      m_pum.pumHeaderLineHeight = tm.tmHeight;
   }

   {
      GSELECT_OBJECT(&JDC, &m_fontFooter);
      JDC.GetTextMetrics(&tm);

      m_pum.pumFooterHeight = tm.tmHeight * 2;
      m_pum.pumFooterLineHeight = tm.tmHeight;
   }

   {
      GSELECT_OBJECT(&JDC, &(m_fontPairBody.fontPrinter));
      JDC.GetTextMetrics(&tm);
      m_pum.pumLineOfText = tm.tmHeight;
   }

   {
      GSELECT_OBJECT(&JDC, &(m_fontHeading));
      JDC.GetTextMetrics(&tm);
      m_pum.pumHeadingHeight = m_pum.pumLineOfText;
   }


   // this must be called to make the metrics initialized before work
   RealizeMetrics();
}


BOOL Table1Unit::Print()
{
   GPrintUnit::Print();

   // start a new page and print the header
   StartPage();
   
   // prepare the index item to indicate this table
   INDEXITEM ii;
   ii.strName = "Table 1";
   ii.wPage = (WORD)JINFO.m_nCurPage;
   ii.nFlags = INDEXF_DASHES|INDEXF_PAGENO;
   AddIndexItem(&ii);

	// print the headings, actually it is titles of the columns)
   {
      GSELECT_OBJECT(&JDC, &m_fontHeading);
      PrintColHeadings(DT_LEFT);
   }
   
   struct part
   {
      LPCTSTR lpszPart;
      LPCTSTR lpszDesc;
      LPCTSTR lpszQty;
      LPCTSTR lpszCost;
   };

   //struct part parts[] = {
	  //"123-4567", "Binary flip flop module, with 4t5 rating", "1","$34.45",
   //   "aq4-9909", "Overhead flimflam", "12", "$0.99",
   //   "b59-123", "Left-handed gangly wrench", "6", "$99.99"};                 

	struct part parts[] = {
		  "123-4567", "Binary flip flop module, with 4t5 rating", "1","$34.45",
		  "aq4-9909", "Overhead flimflam", "12", "$0.99",
		  "b59-123", "Left-handed gangly wrench", "6", "$99.99",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45",
	"123-4567", "Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang", "1","$34.45"};


	GSELECT_PUFONT(&JDC, &m_fontPairBody);

   for(int i = 0; i < sizeof(parts)/sizeof(parts[0]); i++)
   {
      StartRow();

      struct part *pPart = &parts[i];

      PrintColContent(0, pPart->lpszPart, DT_LEFT);
      PrintColContent(1, pPart->lpszDesc, DT_LEFT);
      PrintColContent(2, pPart->lpszQty, DT_LEFT);
      PrintColContent(3, pPart->lpszCost, DT_LEFT);

      EndRow();
   }


   EndPage();
   return TRUE;
}



void Table1Unit::PrintHeader()
{
	// use the JDC as the current DC and use the attributes of m_fontHeader
	// to draw the following contents. Besides, at the end of the scope, 
	// the old DC will be restored
   GSELECT_OBJECT(&JDC, &m_fontHeader);

   CString strDateTime;
   // get the current time
   SYSTEMTIME sysTime;
   GetLocalTime(&sysTime);
   // format it...
   TCHAR szBuf[100];
   GMakeStringFillZero(szBuf);
   // get the time
   GetDateFormat(LOCALE_USER_DEFAULT, NULL, &sysTime, NULL, szBuf, sizeof(szBuf));
   strDateTime = szBuf;
   strDateTime += " ";
   // get the date
   GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &sysTime, NULL, szBuf, sizeof(szBuf));
   strDateTime += szBuf;

   CString strHeader = strDateTime;
   // indicate the above text (date) should be drawn at the center
   strHeader += HFC_CENTER; 
   strHeader += "Company Name";
   // indicate the above text("Company Name") should be drawn right justify
   strHeader += HFC_RIGHTJUSTIFY; 
  
   CString strPage;
   strPage.Format("Page: %d", JINFO.m_nCurPage);

   strHeader += strPage;

   PrintHeaderText(strHeader);
}


void Table1Unit::PrintFooter()
{
   GSELECT_OBJECT(&JDC, &m_fontFooter);

   CString strFooter;

	strFooter = "Print Sample";
   strFooter += HFC_CENTER;
   strFooter += "www.codetools.com";
   strFooter += HFC_RIGHTJUSTIFY;
   strFooter += "table 1";

   PrintFooterText(strFooter);
}
