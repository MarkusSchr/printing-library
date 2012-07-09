

// gfx_printunit.cpp

#include "stdafx.h"
#include "gfx_printunit.h"
#include "gfx_printjob.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(GPrintUnit, CObject)





GPrintUnit::GPrintUnit(GPrintJob *pJob)
{
	m_pJob = pJob;

	m_sizeCurrentRow = CSize(0,0);
	GMakeStructFillZero(m_pum);

	m_lpActiveColDefs = NULL;
	m_pActiveFontPair = NULL;

	m_bPrintingHeading = FALSE;

	m_currentWorkingColums = 0;
}


GPrintUnit::~GPrintUnit()
{
	int nHeadings = m_headings.GetSize();

	for(int nHeading = 0; nHeading < nHeadings; nHeading++)
	{
		LPPRINTUNITCOLDEFS lpColDefs = m_headings.GetAt(nHeading);
		if(lpColDefs)
		{
			int nSize = lpColDefs->GetSize();
			for(int i = 0; i < nSize; i++)
			{
				LPPRINTCOLUMNDEF lpDef = lpColDefs->GetAt(i);
				delete lpDef;
			}

			delete lpColDefs;
		}
	}
}


void GPrintUnit::SetActiveHeading(int nHeading)
{
	m_lpActiveColDefs = m_headings.GetAt(nHeading);
}


void GPrintUnit::SetJob(GPrintJob *pJob)
{
	m_pJob = pJob;
}


GPrintJob *GPrintUnit::GetJob() const
{
	return m_pJob;
}



BOOL GPrintUnit::Print()
{
	CreatePrintFonts();
	InitPrintMetrics();
	CompleteAllColHeadingsDefinition();

	return TRUE;
}


BOOL GPrintUnit::ContinuePrinting() const
{
	return (m_pJob && m_pJob->m_pInfo && JINFO.m_bContinuePrinting) ? TRUE : FALSE;
}



void GPrintUnit::CreatePrintFonts()
{
}


void GPrintUnit::InitPrintMetrics()
{
}



void GPrintUnit::CompleteAllColHeadingsDefinition()
{
	int nHeadings = m_headings.GetSize();

	// traverse all the column sets
	for(int i = 0; i < nHeadings; i++)
	{
		LPPRINTUNITCOLDEFS lpColDefs = m_headings.GetAt(i);

		if(!m_lpActiveColDefs)
		{
			m_lpActiveColDefs = lpColDefs;
		}

		// move the columns to different pages according to the percentage the user identifies
		// first clear the container
		for (int i = 0; i < (int)m_vecColumnPage.size(); i++)
		{
			m_vecColumnPage[i].clear();
		}
		m_vecColumnPage.clear();

		// begin to move columns
		int nSize = lpColDefs->GetSize();
		double currentPercentage = 0;
		vector<int> currentPageColumns;

		for(int i = 0; i < nSize; i++)
		{
			LPPRINTCOLUMNDEF lpDef = lpColDefs->GetAt(i);
			if(lpDef)
			{
				if (currentPercentage + lpDef->fPct <= 1)
				{
					// the column should in this page
					currentPageColumns.push_back(i);

					currentPercentage += lpDef->fPct;
				}
				else
				{
					// the percentage has exceed this page's capacity
					m_vecColumnPage.push_back(currentPageColumns);
					currentPageColumns.clear();
					currentPercentage = 0.0;
					
					// move back to process this column again
					i--;
				}
			}
		}
		m_vecColumnPage.push_back(currentPageColumns);

		// adjust the position of the columns
		for (int i = 0; i < (int)m_vecColumnPage.size(); i++)
		{
			// for each page, columns should start at JRECT.left
			int nStart = JRECT.left;
			int nSize = m_vecColumnPage[i].size();
			for(int j = 0; j < nSize; j++)
			{
				LPPRINTCOLUMNDEF lpDef = lpColDefs->GetAt(m_vecColumnPage[i][j]);
				if(lpDef)
				{
					lpDef->nStart = nStart;
					//  the next start position just after the previous
					nStart += lpDef->nWidth;
				}
			}
		}
	}
}

// it is the user's responsible to check whether all the columns have been set the value
void GPrintUnit::PrintTableContents( vector<vector<char*> >& contents, UINT nRowFormat, UINT nHeadingFormat,BOOL bPrintHeadingWhenChangePage /*= TRUE*/, BOOL bPreprocess /*= FALSE*/ )
{
	if (bPreprocess == TRUE && AreAllColumnsInOnePage() == true)
	{
		// since all the columns can be printed in one page, 
		// it is no necessary to do the preprocess
		return;
	}

	int nRows = contents.size();
	int pageBeforeFirstRowNumBeforeChangePage = -1;
	
	// do the actual job
	bool bNewPage = StartPage(bPreprocess);

	// traverse all the data
	int row = 0;
	while(row < nRows)
	{
		// deal different pages' columns, attention that columns 
		// can be printed in different pages. here we just print 
		// them one page by one page
		int columnPage = 0;
		while(columnPage < (int)m_vecColumnPage.size())
		{
			m_currentWorkingColums = columnPage;

			// in another page
			if (!(bNewPage && IsCurrentRowOverflow()))
			{
				// print the overflow does not need to call 'StartRow'
				StartRow(bPreprocess);
			}

			// print heading first
			// attention that heading is a kind of row
			if (bNewPage && bPrintHeadingWhenChangePage == TRUE)
			{
				// TODO: change the font
				CFont m_fontHeading;
				LOGFONT logFont;
				GMakeStructFillZero(logFont);
				LPCTSTR lpszFaceName = _T("Arial");//I18nok
				logFont.lfCharSet = DEFAULT_CHARSET;
				logFont.lfHeight = 90;
				lstrcpyn(logFont.lfFaceName, lpszFaceName, GGetArrayElementCount(logFont.lfFaceName));
				logFont.lfWeight = FW_BOLD;
				m_fontHeading.CreatePointFontIndirect(&logFont, &JDC);

				GSELECT_OBJECT(&JDC, &m_fontHeading);
				PrintColHeadings(m_vecColumnPage[columnPage], nHeadingFormat, FALSE, bPreprocess);
				EndRow(bPreprocess);
			}

			// draw all the columns of the current row in this column's page
			for(int columnIndex = 0; columnIndex < (int)m_vecColumnPage[columnPage].size(); columnIndex++)
			{
				// check to see whether to print for overflow
				if (bNewPage && IsCurrentRowOverflow())
				{
					// print for overflow
					// TODO: change the font
					// GSELECT_PUFONT(&JDC, &m_fontPairBody);
					int whichColumnToPrint = m_vecColumnPage[columnPage][columnIndex];
					PrintColForOverflow(
						whichColumnToPrint,
						bPreprocess);
				}
				else
				{	
					// the normal row's printing
					// TODO: change the font
					//GSELECT_PUFONT(&JDC, &m_fontPairBody);
					int whichColumnToPrint = m_vecColumnPage[columnPage][columnIndex];
					PrintColumnContent(
						whichColumnToPrint, 
						contents[row][ whichColumnToPrint ], 
						nRowFormat, 
						bPreprocess);
				}				
			}

			bNewPage = EndRow( bPreprocess ) == ER_OVERFLOW || row == nRows - 1;
			// the page is full or we have reached the bottom
			if (true == bNewPage)
			{
				EndPage(bPreprocess);
				
				// if this is the last row of the table, also we have printed all the columns, then just end the page
				if (!(row == nRows - 1 && columnPage == (int)m_vecColumnPage.size() - 1) )
				{
					StartPage(bPreprocess);
				}
				
				// if we reach the last column of the last column's page, begin the next page
				if ( columnPage == (int)m_vecColumnPage.size() - 1 )
				{
					// we should begin the next page
					pageBeforeFirstRowNumBeforeChangePage = row - 1; // -1 means to fill the overflow this 'row'

					// now the next row to print is the row that is overflow
				}
				else
				{
					// we need to print other columns
					row = pageBeforeFirstRowNumBeforeChangePage;
				}

				// the current row has caused change page
				columnPage++;
			}

			// go to next row
			row++;
		}
	}	

	// do not need it, it has been done in the while loop
	//EndPage(bPreprocess);
}




void GPrintUnit::InsertPrintCol(int nPos, LPCTSTR lpszName, double fColPct, int nHeading)
{
	// assert the column' percentage must below 1.0
	assert(fColPct <= 1.0);

	PRINTCOLUMN pc;
	GMakeStructFillZero(pc);

	pc.nPos = nPos;
	pc.lpszName = lpszName;
	pc.fColPct = fColPct;
	// always use the rich edit to contain the contents, thus it can 
	// output in multilines
	pc.dwFlags |= PCF_USERICHEDIT; 

	InsertPrintCol(&pc, nHeading);
}


void GPrintUnit::InsertPrintCol(int nPos, UINT nIDName, double fColPct, int nHeading)
{
	CString strName;
	strName.LoadString(nIDName);

	InsertPrintCol(nPos, strName, fColPct, nHeading);
}





void GPrintUnit::InsertPrintCol(LPPRINTCOLUMN pCol, int nHeading)
{
	ASSERT(pCol);
	if(!pCol)
		return;

	// this is an array containing all the columns definitions
	LPPRINTUNITCOLDEFS lpColSet = NULL;

	if(nHeading >= m_headings.GetSize())
	{
		// nHeading indicates a new column set
		lpColSet = new PRINTUNITCOLDEFS;
		m_headings.InsertAt(nHeading, lpColSet);
	}
	else
	{
		// nHeading indicates an existing column set
		lpColSet = m_headings.GetAt(nHeading);
	}

	// to define a variable to contain the column def
	LPPRINTCOLUMNDEF lpNewDef = new PRINTCOLUMNDEF;
	if(pCol->lpszName)
	{
		if(pCol->dwFlags & PCF_TEXTISID)
			lpNewDef->strName.LoadString((UINT)pCol->lpszName);
		else
			lpNewDef->strName = pCol->lpszName;
	}

	if(pCol->dwFlags & PCF_STRETCHY)
	{
		double fRemaining = 100.0;

		int nSize = lpColSet->GetSize();
		for(int i = 0; i < nSize; i++)
		{
			LPPRINTCOLUMNDEF lpDef = lpColSet->GetAt(i);
			if(lpDef)
			{
				fRemaining -= lpDef->fPct;
			}
		}

		pCol->fColPct = fRemaining;
	}

	// if fColPct is 15, Width() returns 90, the GPERCENT returns 90*0.15
	lpNewDef->nWidth = GPERCENT(JINFO.m_rectDraw.Width(), pCol->fColPct);

	lpNewDef->fPct = pCol->fColPct;
	lpNewDef->dwFlags = pCol->dwFlags;
	lpColSet->InsertAt(pCol->nPos, lpNewDef);
}



LPPRINTCOLUMNDEF GPrintUnit::GetPrintColDef(int nCol, int nHeading)
{
	LPPRINTUNITCOLDEFS pColDefs = NULL;

	if(nHeading == -1)
	{
		pColDefs = m_lpActiveColDefs;
	}
	else
	{
		pColDefs = m_headings.GetAt(nHeading);
	}

	LPPRINTCOLUMNDEF pDef = NULL;

	if(pColDefs)
	{
		pDef = pColDefs->GetAt(nCol);
	}

	return pDef;
}



void GPrintUnit::SaveDim(LPJOBUNITDIM pDim)
{
	pDim->pum = m_pum;
	pDim->ptJCUR = JCUR;
	pDim->rectJINFO = JINFO.m_rectDraw;
	pDim->rectJRECT = JRECT;
}


void GPrintUnit::RestoreDim(LPJOBUNITDIM pDim)
{
	m_pum = pDim->pum;
	JCUR = pDim->ptJCUR;
	JINFO.m_rectDraw = pDim->rectJINFO;
	JRECT = pDim->rectJRECT;
}



bool GPrintUnit::StartPage( BOOL bPreprocess /*= FALSE*/, BOOL bDrawHeading /*= FALSE*/ )
{
	ASSERT(!m_pJob->IsEndPagePending());

	m_pJob->SetEndPagePending();

	JCUR = JRECT.TopLeft();
	bool bResult = JDC.StartPage() >= 0;

	PrintHeader();

	return bResult;
}



void GPrintUnit::EndPage( BOOL bPreprocess /*= FALSE*/ )
{
	m_pJob->SetEndPagePending(FALSE);

	PrintFooter();

	JDC.EndPage();
	JINFO.m_nCurPage++;
	JINFO.m_nPhysicalCurPage++;
}



void GPrintUnit::AdvancePage(BOOL bIncPageNo)
{
	JCUR = JRECT.TopLeft();

	JDC.StartPage();

	JDC.EndPage();

	if(bIncPageNo)
	{
		JINFO.m_nCurPage++;
		JINFO.m_nPhysicalCurPage++;
	}
}



int GPrintUnit::StartRow( BOOL bPreprocess /*= FALSE*/, int nHeight/*=PT_LINEOFTEXT*/ )
{
	int nRC = SR_NULL;

	// all heights below zero are actually enumerated types...
	// convert to a height
	if(nHeight <= 0)
	{
		nHeight = PumTypeToHeight((PUMTYPE)nHeight);
	}

	// if we'll exceed the bottom of the page, we must start a new page
	if((JCUR.y + nHeight) > JRECT.bottom)
	{
		EndPage(bPreprocess);
		StartPage(bPreprocess);

		nRC = SR_ADVANCEDPAGE;
	}

	return nRC;   
}



// this method will end the row by calculating the height of the row 
// and print the overflow if necessary
int GPrintUnit::EndRow( BOOL bPreprocess/*=FALSE*/, BOOL bCheckForOverflow/*=TRUE*/, BOOL bDrawOutline /*= TRUE*/ )
{
	int nRC = ER_NULL;

	// check for overflow
	BOOL bOverFlow = FALSE;
	if(bCheckForOverflow && m_lpActiveColDefs)
	{
		// is there any column of the row overflow
		bOverFlow = IsCurrentRowOverflow();
	}

	if(bOverFlow)
	{
		// there are some columns of the row overflow
		nRC = ER_OVERFLOW;

		if (bDrawOutline)
		{
			// draw the line before go to the next page
			DrawOuterLine();
		}

		// set the currentRow of the new page to 0z
		m_sizeCurrentRow = CSize(0,0);
	}
	else
	{
		if (bDrawOutline)
		{
			// draw the line before go to the next page
			DrawOuterLine();
		}

		JCUR.y += m_sizeCurrentRow.cy;
		JCUR.x = JRECT.left;
	}

	m_sizeCurrentRow = CSize(0,0);
	return nRC;
}


// return true, if
bool GPrintUnit::OnContinueRow( BOOL bPreprocess /*= FALSE*/ )
{
	EndPage(bPreprocess);
	return StartPage(bPreprocess);
}



int GPrintUnit::PumTypeToHeight(PUMTYPE pt) const
{
	int tmHeight = 0;

	switch(pt)
	{
	case PT_HEADING:
		tmHeight = m_pum.pumHeadingHeight;
		break;

	case PT_LINEOFTEXT:
		tmHeight = m_pum.pumLineOfText;
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	return tmHeight;
}



void GPrintUnit::PrintColumnContent( int nCol, LPCTSTR lpszText, UINT nFormat, BOOL bPreprocess )
{
	if(lpszText)
	{
		LPPRINTCOLUMNDEF lpDef = m_lpActiveColDefs->GetAt(nCol);

		if(lpDef)
		{
			CRect rect;
			GMAKERECT(rect, lpDef->nStart, JCUR.y, lpDef->nWidth, m_pum.pumLineOfText);

			if(lpDef->dwFlags & PCF_RIGHTMARGIN)
			{
				// reduce column width
				int nMargin = GPERCENT(rect.Width(), 0.015);
				rect.right -= nMargin;
			}

			int nLen = _tcslen(lpszText);
			int nHeight = DrawColText(lpszText, nLen, rect, nFormat, nCol, lpDef);

			m_sizeCurrentRow.cy = max(m_sizeCurrentRow.cy, nHeight);

			JCUR.x += lpDef->nWidth;
		}
	}
}



int GPrintUnit::DrawColText(LPCTSTR lpszText, int nLen, CRect rect, UINT nFormat, 
							int nCol, LPPRINTCOLUMNDEF lpDef)
{
	int nHeight = 0;
	int nWidth = rect.Width();

	// get the extent of this text
	CSize size = JDC.GetTextExtent(lpszText, nLen);
	int nTextLen = size.cx;

	BOOL bUseRichEdit = FALSE;

	// forced use of the rich edit control
	if(lpDef->dwFlags & PCF_USERICHEDIT)
	{
		bUseRichEdit = TRUE;
	}
	// this says use the rich edit control only if it will 
	// overflow the available column width
	else
	{
		if(nFormat & DT_EDITCONTROL)
		{
			nFormat &= ~DT_EDITCONTROL;

			ASSERT(m_pActiveFontPair);

			if(nTextLen >= nWidth)
			{
				bUseRichEdit = TRUE;
			}
		}
	}


	if(bUseRichEdit)
	{
		rect.bottom = JRECT.bottom;

		if(m_richEdit.m_hWnd == NULL)
		{
			DWORD dwStyle = ES_MULTILINE | WS_CHILD | ES_LEFT;
			m_richEdit.Create(dwStyle, rect, AfxGetMainWnd(), NULL);
		}

		ASSERT(m_pActiveFontPair);
		if(m_pActiveFontPair)
		{
			m_richEdit.SetFont(&(m_pActiveFontPair->fontScreen));

			CHARFORMAT cf;
			if(GfxFontToCharformat(&m_pActiveFontPair->fontPrinter, cf, &JDC))
			{
				m_richEdit.SetDefaultCharFormat(cf);
			}
		}

		m_richEdit.SetWindowText(lpszText);
		LONG lEditLength = m_richEdit.GetTextLength();

		LONG lStart = 0;
		//		LONG lEnd = lEditLength - 1;

		int nOldMode = JDC.SetMapMode(MM_TWIPS);
		JDC.DPtoLP(rect);

		int nBottom = abs(rect.bottom);
		int nTop = abs(rect.top);
		rect.bottom = nBottom;
		rect.top = nTop;

		FORMATRANGE range;
		range.hdcTarget = JDC.m_hAttribDC;
		range.hdc = JDC.m_hDC;
		range.rcPage = rect;
		range.rc = rect;
		range.chrg.cpMin = lStart;
		range.chrg.cpMax = lEditLength;//lEnd;

		JDC.SetMapMode(nOldMode);

		range.chrg.cpMin = FormatDrawColText(lpszText, nLen, rect, nFormat, nCol, lpDef, &range, TRUE);
		//range.chrg.cpMin = m_richEdit.FormatRange(&range, TRUE);

		if((range.chrg.cpMin) < lEditLength)
		{
			CString strText = lpszText;
			int nOverflow = lEditLength - (range.chrg.cpMin);// + 1);
			lpDef->strOverflow = strText.Right(nOverflow);
		}

		JDC.SetMapMode(MM_TWIPS);
		JDC.LPtoDP(&range.rc);
		nHeight = abs(range.rc.bottom - range.rc.top);
		JDC.SetMapMode(nOldMode);
	}
	else
	{
		JDC.DrawText(lpszText, nLen, &rect, nFormat);
		nHeight = size.cy;
	}
	return nHeight;
}



LONG GPrintUnit::FormatDrawColText(LPCTSTR lpszText, int nLen, CRect r, UINT nFormat, 
								   int nCol, LPPRINTCOLUMNDEF lpDef, FORMATRANGE *pRange, BOOL bDisplay)
{
	return m_richEdit.FormatRange(pRange, bDisplay);
}



void GPrintUnit::PrintColHeadings( vector<int>& headings, UINT nFormat, UINT nEffects/*=0*/, BOOL bPreprocess /*= FALSE*/ )
{
	// Since most derived units will call this from an overidden StartPage(), and
	// we call StartRow() which can trigger a StartPage(), this boolean prevents
	// us from printing out the heading twice...
	if(!m_bPrintingHeading)
	{
		m_bPrintingHeading = TRUE;

		try
		{
			int nSize = headings.size();
			for(int i = 0; i < nSize; i++)
			{
				LPPRINTCOLUMNDEF lpDef = m_lpActiveColDefs->GetAt(headings[i]);
				if(lpDef)
				{
					CRect r;
					GMAKERECT(r, lpDef->nStart, JCUR.y, lpDef->nWidth, m_pum.pumHeadingHeight);
					int nLen = lpDef->strName.GetLength();

					if(lpDef->dwFlags & PCF_RIGHTMARGIN)
					{
						// reduce column width
						int nMargin = GPERCENT(r.Width(), 0.015);
						r.right -= nMargin;
					}

					if(nEffects && !(nEffects & HE_DOLAST))
					{
						DoHeadingEffect(i, lpDef->strName, nLen, r, nFormat, nEffects);
					}
				
					PrintColHeading(lpDef->strName, nLen, r, nFormat, nEffects);

					if(nEffects & HE_DOLAST)
					{
						DoHeadingEffect(i, lpDef->strName, nLen, r, nFormat, nEffects);
					}
				}
			}

			m_sizeCurrentRow.cy = m_pum.pumHeadingHeight;
		}
		catch(...)
		{
			m_bPrintingHeading = FALSE;
			throw;
		}

		m_bPrintingHeading = FALSE;
	}
}





void GPrintUnit::PrintColHeading( LPCTSTR lpszName, int nLen, CRect r, UINT nFormat, UINT nEffects )
{
	// compute the width and height of a line of text 
	// using the current font to determine the dimensions.
	CSize size = JDC.GetTextExtent(lpszName, nLen);
	if(size.cx > r.Width())
	{
		// because certain print drivers seem to overflow 'r' no matter what, we
		// must take extra steps to prevent it...
		do
		{
			size = JDC.GetTextExtent(lpszName, --nLen);

		} while(size.cx > r.Width());
	}

	JDC.DrawText(lpszName, nLen, &r, nFormat);
}



void GPrintUnit::DoHeadingEffect(int nCol, LPCTSTR lpszName, int nLen, CRect r,
								 UINT nFormat, UINT nEffects)
{
}



void GPrintUnit::DrawDots(LPRECT lpRect)
{
	TCHAR chDot = _T('.');//I18nOK
	DrawRepeatChar(chDot, lpRect);
}


void GPrintUnit::DrawRepeatChar(TCHAR ch, LPRECT lpRect) 
{
	if(!lpRect)
		return;

	CSize sizeChar = JDC.GetTextExtent(CString(ch));

	if(!sizeChar.cx)
		return;

	int nWidth = max(lpRect->right - lpRect->left, 0);
	int nChars = nWidth/sizeChar.cx;

	if(nChars > 0)
	{
		CString strRepeatChars(ch, nChars);
		JDC.DrawText(strRepeatChars, lpRect, DT_SINGLELINE);
	}
}

// change the client rect size according to the metrics
void GPrintUnit::RealizeMetrics()
{
	JRECT = JINFO.m_rectDraw;

	JRECT.top += m_pum.pumHeaderHeight;
	JRECT.bottom -= m_pum.pumFooterHeight;
	JRECT.left += m_pum.pumLeftMarginWidth;
	JRECT.right -= m_pum.pumRightMarginWidth;

	// Normalizes CRect so that both the height and width are positive. 
	JRECT.NormalizeRect();
}




void GPrintUnit::PrintFooter()
{

}



void GPrintUnit::PrintHeader()
{

}


static BOOL g_StartEndRow = TRUE;

void GPrintUnit::PrintFooterText(LPCTSTR lpszText)
{
	PRINTTEXTLINE ptl;
	GMakeStructFillZero(ptl);

	ptl.lpszText = lpszText;
	ptl.tmHeight = m_pum.pumFooterLineHeight;

	ptl.rectText.left = JRECT.left;
	ptl.rectText.right = JRECT.right;
	ptl.rectText.top = JRECT.bottom;
	ptl.rectText.bottom = 0;

	g_StartEndRow = FALSE;
	PrintTextLine(&ptl);
}



void GPrintUnit::PrintHeaderText(LPCTSTR lpszText)
{
	PRINTTEXTLINE ptl;
	GMakeStructFillZero(ptl);

	ptl.lpszText = lpszText;
	ptl.tmHeight = m_pum.pumHeaderLineHeight;

	ptl.rectText.left = JRECT.left;
	ptl.rectText.right = JRECT.right;
	ptl.rectText.top = JRECT.top - m_pum.pumHeaderHeight;
	ptl.rectText.bottom = 0;

	g_StartEndRow = FALSE;
	PrintTextLine(&ptl);
}




void GPrintUnit::PrintTextLine(
	LPCTSTR lpszText, 
	UINT nFormat, 
	int tmHeight,
	BOOL bPreprocess)
{
	PRINTTEXTLINE ptl;
	GMakeStructFillZero(ptl);

	ptl.lpszText = lpszText;
	ptl.tmHeight = tmHeight;
	ptl.nFormat = nFormat;
	ptl.dwFlags = (PTLF_STARTROW|PTLF_ENDROW);

	ptl.rectText.left = JRECT.left;
	ptl.rectText.right = JRECT.right;
	ptl.rectText.top = JCUR.y;
	ptl.rectText.bottom = 0;

	g_StartEndRow = TRUE;
	PrintTextLine(&ptl, bPreprocess);
}



void GPrintUnit::PrintTextLine(LPPRINTTEXTLINE lpTextLine, BOOL bPreprocess)
{
	PrintTextLineEx(lpTextLine, bPreprocess);
}


void GPrintUnit::PrintTextLineEx(LPPRINTTEXTLINE lpTextLine, BOOL bPreprocess)
{
	if(!lpTextLine)
		return;

	if(!lpTextLine->tmHeight)
	{
		lpTextLine->tmHeight = m_pum.pumLineOfText;
		ASSERT(lpTextLine->tmHeight); // whoa
	}

	// number of lines we've printed
	int nLines = max(1, GfxCountLines(lpTextLine->lpszText));

	if(lpTextLine->dwFlags & PTLF_STARTROW)
	{
		if(StartRow(bPreprocess, lpTextLine->tmHeight * nLines) == SR_ADVANCEDPAGE)
		{
			lpTextLine->rectText.top = JCUR.y;
		}
	}

	// set up some rectangles
	CRect rectLast(0,0,0,0);
	// default text rect 'r'
	CRect r = lpTextLine->rectText;
	r.bottom = r.top + lpTextLine->tmHeight;

	// establish a parser
	GPrintTextLineParser parser;

	// check if there are any dot formatting characters now for speed
	GPTLPROFILE profile;
	parser.GetProfile(lpTextLine->lpszText, profile);

	// number of rows we've printed
	int nRowsPrinted = 1;
	BOOL bParse = TRUE;
	GPTLTOKEN token;

	while(bParse)
	{
		BOOL bPrint = FALSE;
		GNTRESULT result = parser.GetNextToken(lpTextLine->lpszText, token);

		switch(result)
		{
		case GNTR_TOKENNEWLINE:
			nRowsPrinted++;
			// fall through
		case GNTR_TOKEN:
			bPrint = TRUE;
			break;

		case GNTR_ENDOFLINE:
			bParse = FALSE;
			break;

		case GNTR_ERROR:
		default:
			bParse = FALSE;
			ASSERT(FALSE);
			break;
		}

		// we've got something to print?
		if(bPrint)
		{
			// adjust rect for line number
			if(token.bNewLine)
			{
				r.OffsetRect(0, lpTextLine->tmHeight);
			}

			bPrint = FALSE;

			if(token.bDots || !token.strToken.IsEmpty())
			{
				int nTempFormat = token.nFormat|lpTextLine->nFormat;

				BOOL bIsText = !token.strToken.IsEmpty();
				CString strTemp(token.strToken);

				if(bIsText)
				{
					JDC.DrawText(token.strToken, &r, nTempFormat);
				}

				CRect rectNew;

				if(profile.bDots)
				{
					if(!bIsText)
					{
						rectNew.left = rectNew.right = r.right;
					}
					else
					{
						CSize sizeText = JDC.GetTextExtent(strTemp);
						int nLineWidth = r.Width();

						if(nTempFormat & DT_CENTER)
						{
							rectNew.left = (nLineWidth - sizeText.cx)/2;
							rectNew.left = max(0, rectNew.left);
							rectNew.left += r.left;
							rectNew.right = rectNew.left + sizeText.cx;
						}
						else
							if(nTempFormat & DT_RIGHT)
							{
								rectNew.right = r.right; 
								rectNew.left = r.right - min(nLineWidth, sizeText.cx);
							}
							// default to left
							else
							{
								rectNew.left = r.left;
								rectNew.right = r.left + min(nLineWidth, sizeText.cx);
							}
					}

					rectNew.top = r.top;
					rectNew.bottom = r.bottom;
				}

				// print a row of dots between this and the last text
				if(token.bDots)
				{
					if(rectNew.left > rectLast.right)
					{
						CRect rectDots(r);
						rectDots.left = rectLast.right;
						rectDots.right = rectNew.left;

						DrawDots(&rectDots);
					}
				}

				rectLast = rectNew;
			}
		}
	}

	if(nRowsPrinted)
	{
		m_sizeCurrentRow.cy = (lpTextLine->tmHeight * nRowsPrinted);

		if(lpTextLine->dwFlags & PTLF_ENDROW)
		{
			EndRow(bPreprocess, FALSE);
		}
	}
}



int GPrintUnit::SetMapMode(int nMapMode)
{
	CPoint ptTemp(0,0);

	// convert all the current  
	int nCurrentMapMode = JDC.GetMapMode();

	// no need to convert if LP and DP are equal
	if(nCurrentMapMode != MM_TEXT)
	{
		ptTemp.y = m_pum.pumHeadingHeight;
		JDC.LPtoDP(&ptTemp);
		m_pum.pumHeadingHeight = abs(ptTemp.y);

		ptTemp.y = m_pum.pumFooterHeight;
		JDC.LPtoDP(&ptTemp);
		m_pum.pumFooterHeight = abs(ptTemp.y);

		ptTemp.y = m_pum.pumHeaderHeight;
		JDC.LPtoDP(&ptTemp);
		m_pum.pumHeaderHeight = abs(ptTemp.y);

		ptTemp.x = m_pum.pumRightMarginWidth;
		JDC.LPtoDP(&ptTemp);
		m_pum.pumRightMarginWidth = abs(ptTemp.x);

		ptTemp.x = m_pum.pumLeftMarginWidth;
		JDC.LPtoDP(&ptTemp);
		m_pum.pumLeftMarginWidth = abs(ptTemp.x);

		ptTemp.y = m_pum.pumFooterLineHeight;
		JDC.LPtoDP(&ptTemp);
		m_pum.pumFooterLineHeight = abs(ptTemp.y);

		ptTemp.y = m_pum.pumHeaderLineHeight;
		JDC.LPtoDP(&ptTemp);
		m_pum.pumHeaderLineHeight = abs(ptTemp.y);

		ptTemp.y = m_pum.pumLineOfText;
		JDC.LPtoDP(&ptTemp);
		m_pum.pumLineOfText = abs(ptTemp.y);

		JDC.LPtoDP(JRECT);
		JRECT.left = abs(JRECT.left);
		JRECT.right = abs(JRECT.right);
		JRECT.top = abs(JRECT.top);
		JRECT.bottom = abs(JRECT.bottom);

		JDC.LPtoDP(&JCUR);
		JCUR.x = abs(JCUR.x);
		JCUR.y = abs(JCUR.y);

		JDC.LPtoDP(JINFO.m_rectDraw);
		JINFO.m_rectDraw.left = abs(JINFO.m_rectDraw.left);
		JINFO.m_rectDraw.right = abs(JINFO.m_rectDraw.right);
		JINFO.m_rectDraw.top = abs(JINFO.m_rectDraw.top);
		JINFO.m_rectDraw.bottom = abs(JINFO.m_rectDraw.bottom);
	}

	// change the mapping mode
	int nOldMapMode = JDC.SetMapMode(nMapMode);

	ptTemp.y = m_pum.pumHeadingHeight;
	JDC.DPtoLP(&ptTemp);
	m_pum.pumHeadingHeight = abs(ptTemp.y);

	ptTemp.y = m_pum.pumFooterHeight;
	JDC.DPtoLP(&ptTemp);
	m_pum.pumFooterHeight = abs(ptTemp.y);

	ptTemp.y = m_pum.pumHeaderHeight;
	JDC.DPtoLP(&ptTemp);
	m_pum.pumHeaderHeight = abs(ptTemp.y);

	ptTemp.x = m_pum.pumRightMarginWidth;
	JDC.DPtoLP(&ptTemp);
	m_pum.pumRightMarginWidth = abs(ptTemp.x);

	ptTemp.x = m_pum.pumLeftMarginWidth;
	JDC.DPtoLP(&ptTemp);
	m_pum.pumLeftMarginWidth = abs(ptTemp.x);

	ptTemp.y = m_pum.pumFooterLineHeight;
	JDC.DPtoLP(&ptTemp);
	m_pum.pumFooterLineHeight = abs(ptTemp.y);

	ptTemp.y = m_pum.pumHeaderLineHeight;
	JDC.DPtoLP(&ptTemp);
	m_pum.pumHeaderLineHeight = abs(ptTemp.y);

	ptTemp.y = m_pum.pumLineOfText;
	JDC.DPtoLP(&ptTemp);
	m_pum.pumLineOfText = abs(ptTemp.y);

	JDC.DPtoLP(JRECT);
	JRECT.left = abs(JRECT.left);
	JRECT.right = abs(JRECT.right);
	JRECT.top = abs(JRECT.top);
	JRECT.bottom = abs(JRECT.bottom);

	JDC.DPtoLP(&JCUR);
	JCUR.x = abs(JCUR.x);
	JCUR.y = abs(JCUR.y);

	JDC.DPtoLP(JINFO.m_rectDraw);
	JINFO.m_rectDraw.left = abs(JINFO.m_rectDraw.left);
	JINFO.m_rectDraw.right = abs(JINFO.m_rectDraw.right);
	JINFO.m_rectDraw.top = abs(JINFO.m_rectDraw.top);
	JINFO.m_rectDraw.bottom = abs(JINFO.m_rectDraw.bottom);

	// the print unit and job expects the device orientation to be y positive down, 
	// and x positive right, just as with MM_TEXT.  It makes any required adjustments...
	CSize sizeOrigViewportExt = JDC.GetViewportExt();
	CSize sizeOrigWindowExt = JDC.GetWindowExt();

	sizeOrigViewportExt.cx = abs(sizeOrigViewportExt.cx);
	sizeOrigViewportExt.cy = abs(sizeOrigViewportExt.cy);

	sizeOrigWindowExt.cx = abs(sizeOrigWindowExt.cx);
	sizeOrigWindowExt.cy = abs(sizeOrigWindowExt.cy);

	JDC.SetWindowExt(sizeOrigWindowExt);
	JDC.SetViewportExt(sizeOrigViewportExt);

	return nOldMapMode;
}



/////////////////////////////
// print index stuff

void GPrintUnit::PrintTree(GPrintIndexTree *pObj, int nLevel)
{
	int nSize = pObj->GetSize();
	for(int i = 0; i < nSize; i++)
	{
		INDEXITEM ti = pObj->GetAt(i);
		PrintTreeItem(&ti, nLevel);

		if(ti.pChildren)
		{
			PrintTree(ti.pChildren, nLevel + 1);
		}
	}
}



void GPrintUnit::PrintTreeItem(LPINDEXITEM lpIndex, int nLevel)
{
	CString strLineText(lpIndex->strName);
	CString strPage;

	if(lpIndex->nFlags & INDEXF_PAGENO)
	{
		strPage.Format(_T("%d"), lpIndex->wPage);  // I18nOk

		if(lpIndex->nFlags & INDEXF_DASHES)
		{
			strLineText += HFC_DOTS;
		}

		strLineText += HFC_RIGHTJUSTIFY;
		strLineText += strPage;
	}

	INDEXLEVELINFO li;
	GMakeStructFillZero(li);

	GetLevelInfo(li, lpIndex, nLevel);

	CFont *pOldFont = NULL;
	if(li.pFont)
	{
		JDC.SelectObject(li.pFont);
	}

	if(!li.nRowHeight)
	{
		CSize size = JDC.GetTextExtent(strLineText);
		li.nRowHeight = size.cy;
	}

	if(JRECT.top + li.nRowHeight > JRECT.bottom)
	{
		EndPage(FALSE);
		StartPage(FALSE);
	}

	PRINTTEXTLINE ptl;
	GMakeStructFillZero(ptl);

	ptl.lpszText = strLineText;
	ptl.tmHeight = li.nRowHeight;
	ptl.dwFlags = (PTLF_STARTROW|PTLF_ENDROW);
	ptl.rectText = JRECT;
	ptl.rectText.top = JCUR.y;
	ptl.rectText.left += li.nIndent;

	PrintTextLine(&ptl);

	if(pOldFont)
	{
		JDC.SelectObject(pOldFont);
	}
}




void GPrintUnit::GetLevelInfo(INDEXLEVELINFO& li, LPINDEXITEM lpIndex, int nLevel)
{
	li.pFont = NULL;
	li.nIndent = nLevel * 100;
}




void GPrintUnit::AddIndexItem(INDEXITEM *pII)
{
	if(m_pJob)
		m_pJob->AddIndexItem(pII);
}

// draw the outer line of the row
void GPrintUnit::DrawOuterLine()
{
	// get the rect's left-top and right-top
	int nSize = m_vecColumnPage[m_currentWorkingColums].size();

	CRect rect;
	for (int i = 0; i < nSize; i++)
	{
		LPPRINTCOLUMNDEF lpDefFirst = m_lpActiveColDefs->GetAt(m_vecColumnPage[m_currentWorkingColums][i]);
		GMAKERECT(
			rect, 
			lpDefFirst->nStart, 
			JCUR.y, 
			lpDefFirst->nWidth, 
			m_sizeCurrentRow.cy);

		CBrush brEdge; 
		brEdge.CreateSolidBrush(RGB(0,0,0));

		GSELECT_OBJECT(&JDC, &brEdge);
		JDC.DrawEdge(&rect, EDGE_BUMP, BF_ADJUST | BF_BOTTOMLEFT | BF_TOPRIGHT);
	}
}

bool GPrintUnit::AreAllColumnsInOnePage()
{
	// can all the columns be fit in a single page
	return m_vecColumnPage.size() == 1;
}

BOOL GPrintUnit::IsCurrentRowOverflow()
{
	BOOL bOverFlow = FALSE;

	int nColumnSize = m_vecColumnPage[m_currentWorkingColums].size();

	for (int i = 0; i < nColumnSize; i++)
	{
		LPPRINTCOLUMNDEF lpDef = m_lpActiveColDefs->GetAt(m_vecColumnPage[m_currentWorkingColums][i]);
		if(lpDef)
		{
			if(!lpDef->strOverflow.IsEmpty())
			{
				bOverFlow = TRUE;
				break;
			}
		}
	}

	return bOverFlow;
}

void GPrintUnit::PrintColForOverflow( int nCol,BOOL bPreprocess /*= FALSE*/ )
{
	LPPRINTCOLUMNDEF lpDef = m_lpActiveColDefs->GetAt(nCol);
	if(lpDef && !lpDef->strOverflow.IsEmpty())
	{
		CString strTemp = lpDef->strOverflow;
		// clear
		lpDef->strOverflow.Empty();
		PrintColumnContent(nCol, strTemp, DT_EDITCONTROL, bPreprocess);
	}
}




///////////////////////////////////////////////////////////////////////


GPrintIndexTree::GPrintIndexTree() 
{

}


GPrintIndexTree::~GPrintIndexTree() 
{
	DeletePrintIndexTree(this);
}



void GPrintIndexTree::DeletePrintIndexTree(GPrintIndexTree *pTree)
{
	int nSize = pTree->GetSize();

	for(int i = 0; i < nSize; i++)
	{
		INDEXITEM ti = pTree->GetAt(i);

		if(ti.pChildren)
		{
			DeletePrintIndexTree(ti.pChildren);
			delete ti.pChildren;
			ti.pChildren = NULL;
		}
	}

	pTree->RemoveAll();
}


//////////////////////////////////////////////////////////////////////////


GSelectActivePair::GSelectActivePair(GPrintUnit *pUnit, LPPUFONTPAIR pPair)
{
	if(pUnit)
	{
		m_pUnit = pUnit;
		m_pOldPair = pUnit->m_pActiveFontPair;
		pUnit->m_pActiveFontPair = pPair;
	}
}


GSelectActivePair::~GSelectActivePair()
{
	if(m_pUnit)
	{
		m_pUnit->m_pActiveFontPair = m_pOldPair;
	}
}


/////////////////////////////////////////////////////////////////////


GIndexItem::GIndexItem()
{
	nFlags = 0;
	pChildren = NULL; 
	wPage = 0;
}




////////////////////////////////////////////////////////////////////////////
// text line parser


GPrintTextLineParser::GPrintTextLineParser()
{
	m_lpszCurChar = NULL;
	m_nSkipChars = 0;
	m_chCur = GNUL;
	m_nNewFormat = 0;
	m_bNewDots = FALSE;
	m_bNewLine = FALSE;
}


GPrintTextLineParser::~GPrintTextLineParser()
{
}


BOOL GPrintTextLineParser::GetProfile(LPCTSTR lpszText, GPTLPROFILE& profile)
{
	BOOL bProfile = FALSE;

	if(lpszText)
	{
		profile.bDots = _tcschr(lpszText, (int)HFC_DOTS) ? TRUE : FALSE;
		bProfile = TRUE;
	}

	return bProfile; 
}




GNTRESULT GPrintTextLineParser::GetNextToken(LPCTSTR lpszText, GPTLTOKEN& token)
{
	GNTRESULT result = GNTR_ERROR;

	int nFormat;
	BOOL bDots;
	BOOL bNewLine;

	if(m_lpszCurChar)
	{
		if(m_chCur == GNUL)
			return GNTR_ENDOFLINE;

		nFormat = m_nNewFormat;
		bDots = m_bNewDots;
		bNewLine = m_bNewLine;

		m_nNewFormat = 0;
		m_bNewDots = FALSE;
		m_bNewLine = FALSE;
	}
	else
	{
		// first time in
		nFormat = DT_LEFT;
		bDots = FALSE;
		bNewLine = FALSE;
		m_lpszCurChar = lpszText;
	}

	// initialize the token in case the user does not pass an empty one
	token.strToken.Empty();
	token.nFormat = 0;
	token.bDots = FALSE;
	token.bNewLine = FALSE;

	do
	{
		// point 'lpszChar' to the next available character
		while(m_nSkipChars--)
		{
			m_lpszCurChar = _tcsinc(m_lpszCurChar);
		}

		m_chCur = *m_lpszCurChar;

		TCHAR chNext = GNUL;
		// get the character after 'm_chCur'...it will let us
		// know if we have consecutive formatting characters
		if(m_chCur != GNUL)
		{
			LPCTSTR lpszNextChar = m_lpszCurChar;
			lpszNextChar = _tcsinc(lpszNextChar);

			if(lpszNextChar)
			{
				chNext = *lpszNextChar;
			}
		}

		// default to 1 
		m_nSkipChars = 1;

		switch(m_chCur)
		{
		case HFC_CENTER:
			// check for double HFC_CENTER
			if(chNext == HFC_CENTER)
			{
				token.strToken += m_chCur;
				m_nSkipChars++;
			}
			else
			{
				result = GNTR_TOKEN;
				m_nNewFormat = DT_CENTER;
			}

			break;

		case HFC_RIGHTJUSTIFY:
			// check for double HFC_RIGHTJUSTIFY
			if(chNext == HFC_RIGHTJUSTIFY)
			{
				token.strToken += m_chCur;
				m_nSkipChars++;
			}
			else
			{
				result = GNTR_TOKEN;
				m_nNewFormat = DT_RIGHT;
			}

			break;

		case HFC_NEWLINE:
			result = GNTR_TOKENNEWLINE;
			m_nNewFormat = DT_LEFT;
			m_bNewLine = TRUE;
			break;

		case HFC_DOTS:
			// check for double HFC_DOTS
			if(chNext == HFC_DOTS)
			{
				token.strToken += m_chCur;
				m_nSkipChars++;
			}
			else
			{
				m_bNewDots = TRUE;
			}

			break;

		case GNUL:  // end of the line
			result = GNTR_TOKEN;
			token.nFormat = DT_LEFT;
			break;

		default:
			token.strToken += m_chCur;
			break;
		}

	} while(result == GNTR_ERROR);


	token.nFormat = nFormat;
	token.bDots = bDots;
	token.bNewLine = bNewLine;

	return result;
}


//////////////////////////////////////////////////////

BOOL GfxFontToCharformat(CFont *pFont, CHARFORMAT& cf, CDC *pDC)
{
	BOOL bConvert = FALSE;

	if(pFont)
	{
		LOGFONT lf;
		if(pFont->GetLogFont(&lf))
		{
			bConvert = TRUE;

			GMakeStructFillZero(cf);
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_FACE | CFM_SIZE | CFM_CHARSET;
			cf.bCharSet = DEFAULT_CHARSET;
			cf.bPitchAndFamily = lf.lfPitchAndFamily;

			int nPointSize = GfxHeightToPointSize(lf.lfHeight, pDC);
			//multiply by 20 to convert to twips
			cf.yHeight = nPointSize * 20;
			memcpy(&cf.szFaceName,&lf.lfFaceName,sizeof(cf.szFaceName));

			if(lf.lfItalic)
			{
				cf.dwEffects |= CFE_ITALIC;
				cf.dwMask |= CFM_ITALIC;
			}

			if(lf.lfWeight == FW_BOLD)
			{
				cf.dwEffects |= CFE_BOLD;
				cf.dwMask |= CFM_BOLD;
			}

			if(lf.lfUnderline)
			{
				cf.dwEffects |= CFE_UNDERLINE;
				cf.dwMask |= CFM_UNDERLINE;
			}

			if(lf.lfStrikeOut)
			{
				cf.dwEffects |= CFE_STRIKEOUT;
				cf.dwMask |= CFM_STRIKEOUT;
			}
		}
	}

	return bConvert;
}


int GfxHeightToPointSize(int nHeight, CDC *pDC)
{
	HDC hDC = NULL;

	if(pDC != NULL)
	{
		ASSERT_VALID(pDC);
		ASSERT(pDC->m_hAttribDC != NULL);
		hDC = pDC->m_hAttribDC;
	}
	else
		hDC = ::GetDC(NULL);

	int nPointSize = -MulDiv(nHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));

	if (pDC == NULL)
		ReleaseDC(NULL, hDC);

	return nPointSize;
}


int GfxCountLines(LPCTSTR lpszText)
{
	int nLines = 0;

	if(lpszText && *lpszText)
	{
		nLines = 1;
		do
		{
			if(*lpszText == _T('\n'))  //I18nOK
				nLines++;

			//Advance a string pointer by one character.
			lpszText = _tcsinc(lpszText);

		} while(*lpszText);
	}

	return nLines;
}


///////////////////////////////////////////////////////////////

GSelectGdiObject::GSelectGdiObject(CDC *pDC, CGdiObject *pObject)
{
	m_hOldGdiObject = NULL;
	m_pDC = NULL;

	if(pDC)
	{
		ASSERT(pObject);
		if(pObject)
		{
			m_pDC = pDC;

			HDC hDC = m_pDC->m_hDC;
			HDC hAttribDC = m_pDC->m_hAttribDC;

			if(hDC != hAttribDC)
				m_hOldGdiObject = ::SelectObject(hDC, pObject->m_hObject);
			if(hAttribDC != NULL)
				m_hOldGdiObject = ::SelectObject(hAttribDC, pObject->m_hObject);
		}
	}
}


GSelectGdiObject::~GSelectGdiObject()
{
	if(m_hOldGdiObject && m_pDC)
	{
		HDC hDC = m_pDC->m_hDC;
		HDC hAttribDC = m_pDC->m_hAttribDC;

		if(hDC != hAttribDC)
			::SelectObject(hDC, m_hOldGdiObject);
		if(hAttribDC != NULL)
			::SelectObject(hAttribDC, m_hOldGdiObject);
	}
}

