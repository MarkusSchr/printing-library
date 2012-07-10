
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
   InsertPrintCol(4, "Column5_0.45", 0.35);
   InsertPrintCol(5, "Column6_0.30", 0.30);
   
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


   // sample 1
	//char* parts[30][8] = {
	//	{"1", "1111111111111111111111111111111111111111111111111111111111111111111111", "1","$34.45", "1111", "1122","1133","1144"},
	//	{"2","22222222222222222222222222222222222222222222222222222222222222222222222","12","$0.99","1111","1122","1133","1144"},
	//	{"3","333333333333333333","6","$99.99","33333333333333333333333333333333333333333333333333333333333333333333333333333","1122","1133","1144"},
	//	{"4","44444444444444444444444444444444444444444444444444444444444444444444444444444444444","1","$34.45","1111","1122","1133","1144"},
	//	{"5","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","555555555555555555555555555555555","1144"},
	//	{"6","66666666666666666666666666666666666666666666666666666666666666666666","1","$34.45","1111","1122","1133","666666666666666666666666666666666666666666"},
	//	{"7","7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777","1","$34.45","1111","77777777777777777777777","1133","1144"},
	//	{"8","888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888","1","$34.45","1111","8888888888888888888888888888888888888","1133","1144"},
	//	{"9","99999999999999999999999999999999999999999999kdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfsla9999999999999999999999999","1","$34.499999999999999999999999995","1111","1122","1133","1144"},
	//	{"a","aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","1","$34.45","1111","1122","1133","1144"},
	//	{"1", "1111111111111111111111111111111111111111111111111111111111111111111111", "1","$34.45", "1111", "1122","1133","1144"},
	//	{"2","22222222222222222222222222222222222222222222222222222222222222222222222","12","$0.99","1111","1122","1133","1144"},
	//	{"3","333333333333333333","6","$99.99","33333333333333333333333333333333333333333333333333333333333333333333333333333","1122","1133","1144"},
	//	{"4","44444444444444444444444444444444444444444444444444444444444444444444444444444444444","1","$34.45","1111","1122","1133","1144"},
	//	{"5","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","555555555555555555555555555555555","1144"},
	//	{"6","66666666666666666666666666666666666666666666666666666666666666666666","1","$34.45","1111","1122","1133","666666666666666666666666666666666666666666"},
	//	{"7","7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777","1","$34.45","1111","77777777777777777777777","1133","1144"},
	//	{"8","888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888","1","$34.45","1111","8888888888888888888888888888888888888","1133","1144"},
	//	{"9","99999999999999999999999999999999999999999999kdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfsla9999999999999999999999999","1","$34.499999999999999999999999995","1111","1122","1133","1144"},
	//	{"a","aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","1","$34.45","1111","1122","1133","1144"},{"21","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","1133","1144"},
	//	{"1", "1111111111111111111111111111111111111111111111111111111111111111111111", "1","$34.45", "1111", "1122","1133","1144"},
	//	{"2","22222222222222222222222222222222222222222222222222222222222222222222222","12","$0.99","1111","1122","1133","1144"},
	//	{"3","333333333333333333","6","$99.99","33333333333333333333333333333333333333333333333333333333333333333333333333333","1122","1133","1144"},
	//	{"4","44444444444444444444444444444444444444444444444444444444444444444444444444444444444","1","$34.45","1111","1122","1133","1144"},
	//	{"5","Binary flip flop module, with 4t5 ratialskdfjfkdlsalaskdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfslalaksdjfjfkdslang","1","$34.45","1111","1122","555555555555555555555555555555555","1144"},
	//	{"6","66666666666666666666666666666666666666666666666666666666666666666azzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz666","1","$34.45","1111","1122","1133","666666666666666666666666666666666666666666"},
	//	{"7","7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777","1","$34.45","1111","77777777777777777777777","1133","1144"},
	//	{"8","888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888","1","$34.45","1111","8888888888888888888888888888888888888","1133","1144"},
	//	{"9","99999999999999999999999999999999999999999999kdjfjfdklsalaskdjfjfkdlsalaskjdfjfkdslalaksdjfkjdfsla9999999999999999999999999","1","$34.499999999999999999999999995","1111","1122","1133","1144"},
	//};

	// sample 2
   /*char* parts[30][8] = {
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
   };*/
	//vector<vector<char*> > vecParts;
 //  for (int i = 0; i < sizeof(parts)/sizeof(parts[0]) ; i++)
 //  {
	//   vector<char*> vecTemp;
	//   for (int j = 0; j < sizeof(parts[0])/sizeof(parts[0][0]) ; j++)
	//   {
	//	   vecTemp.push_back(parts[i][j]);
	//   }
	//   vecParts.push_back(vecTemp);
 //  }


   	// sample 3
    string strArr[30 * 6];
	vector<vector<char*> > vecParts;
	const char* standard = "1234567890";

	for (int i = 0; i < 30 ; i++)
	{
		vector<char*> vecTemp;
		
		for (int j = 0; j < 6 ; j++)
		{
			string str(standard);
			for (int k = 0; k < j + 10; k++)
			{
				str += standard;
			}
			
			if (j == 0 || j == 3)
			{
				char temp[20];
				itoa(i + 1, temp, 10);
				strArr[i * 6 + j] = temp;
			}
			else
				strArr[i * 6 + j] = str;
			vecTemp.push_back(const_cast<char*>(strArr[i * 6 + j].c_str()));
		}

		vecParts.push_back(vecTemp);
	}

	GSELECT_PUFONT(&JDC, &m_fontPairBody);

	// start to printing
	
	// first do the preprocessing
	PrintTableContents(vecParts, DT_LEFT, DT_CENTER, TRUE, TRUE);

	// actual printing
	PrintTableContents(vecParts, DT_LEFT, DT_CENTER, TRUE, FALSE);

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

