
#include "stdafx.h"
#include "unit_table1.h"
#include "gfx_printjob.h"



Table1Unit::Table1Unit(GPrintJob *pJob) : GPrintUnit(pJob)
{
}

Table1Unit::~Table1Unit()
{
}

void Table1Unit::CompleteAllColHeadingsDefinition()
{
   // define my four columns...percentages should all up to 1.00
   InsertPrintCol(0, "Column1_0.45", 0.45);
   InsertPrintCol(1, "Column2_0.30", 0.30);
   InsertPrintCol(2, "Column3_0.10", 0.10);
   InsertPrintCol(3, "Column4_0.35", 0.35);

   InsertPrintCol(4, "Column5_0.45", 0.45);
   InsertPrintCol(5, "Column6_0.30", 0.30);
   InsertPrintCol(6, "Column7_0.10", 0.10);
   InsertPrintCol(7, "Column8_0.15", 0.15);

   // must call base class
   GPrintUnit::CompleteAllColHeadingsDefinition();
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

   // prepare the index item to indicate this table
   INDEXITEM ii;
   ii.strName = "Table 1";
   ii.wPage = (WORD)JINFO.m_nCurPage;
   ii.nFlags = INDEXF_DASHES|INDEXF_PAGENO;
   AddIndexItem(&ii);

   
	//char* parts[30][8] = {
	//	{"1", "Binary flip flop module, with 4t5 rating", "1","$34.45", "1111", "1122","1133","1144"},
	//	{"2","Overhead flimflam","12","$0.99","1111","1122","1133","1144"},
	//	{"3","Left-handed gangly wrench","6","$99.99","1111","1122","1133","1144"},
	//	{"4","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"5","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"6","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"7","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"8","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"9","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"10","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"11","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"12","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"13","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"14","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"15","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"16","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"17","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"18","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"19","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"20","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"21","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"22","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"23","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"24","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"25","Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"26","Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"27","Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"28","Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"29","Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"30","Binary flip flop module, with 4t5 rasdfdsasdfdasasssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssatialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//};

   char* parts[30][8] = {
   	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
   	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
   	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"},
	{"1", "1115", "1","$34.45", "1111", "1122","1133","1144"}
   };
	vector<vector<char*> > vecParts;
	for (int i = 0; i < sizeof(parts)/sizeof(parts[0]) ; i++)
	{
		vector<char*> vecTemp;
		for (int j = 0; j < sizeof(parts[0])/sizeof(parts[0][0]) ; j++)
		{
			vecTemp.push_back(parts[i][j]);
		}
		vecParts.push_back(vecTemp);
	}

	GSELECT_PUFONT(&JDC, &m_fontPairBody);

	// start to printing
	
	// first do the preprocessing
	PrintTableContents(vecParts, DT_LEFT, TRUE);

	//// actual printing
	//PrintTableContents(vecParts, DT_LEFT, FALSE);

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

