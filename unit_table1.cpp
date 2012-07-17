
#include "stdafx.h"
#include "unit_table1.h"
#include "gfx_printjob.h"

#include <string>
#include <sstream>
using namespace std;

Table1Unit::Table1Unit(GPrintJob *pJob, int row, int column) : GPrintUnit(pJob)
{
	m_row = row;
	m_column = column;
}

Table1Unit::~Table1Unit()
{
}

void Table1Unit::CompleteAllColHeadingsDefinition()
{
   // define my four columns...
	for (int i = 0; i < m_column; i++)
	{
		TCHAR buf[2000];
		_itow_s(i, buf, 10);
		wstring str = buf;
		str.append(TEXT("th Column"));
		InsertPrintCol(i, str.c_str(), 0.2);
	}
   
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
	wstring **strArr = NULL;
	strArr = new wstring* [m_row];
	for (int i = 0; i < m_row; i++)
	{
		strArr[i] = new wstring [m_column];
	}

	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_column; j++)
		{
			srand((unsigned)time(0));
			int randomPart1 = rand() % 10000;
			int randomPart2 = rand() % 1000;

			TCHAR c = 0;
			if (rand() % 2)
			{
				c = TEXT('+');
			}
			else
			{
				c = TEXT('-');
			}
			std::wstringstream ss;
			ss << c << randomPart1 << TEXT(".") << randomPart2 << TEXT('\0');
			strArr[i][j] = ss.str(); 

			wstring temp = strArr[i][j];
		}
	}

	vector<vector<LPCTSTR> > vecParts;

	for (int i = 0; i < m_row ; i++)
	{
		vector<LPCTSTR> vecTemp;
		
		for (int j = 0; j < m_column ; j++)
		{
			vecTemp.push_back(const_cast<LPCTSTR>(strArr[i][j].c_str()));
		}

		vecParts.push_back(vecTemp);
	}

	GSELECT_PUFONT(&JDC, &m_fontPairBody);

	// start to printing
	
	// first do the preprocessing
	//PrintTableContents(printData, nRowFormat);

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
   strPage.Format(TEXT("Page: %d"), JINFO.m_nCurPage);

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

