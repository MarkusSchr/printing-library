#include "stdafx.h"

#define _AFXEXT

#include "BitmapTableUnit.h"
#include "MemDCForPrint.h"

Printing::CBitmapTableUnit::CBitmapTableUnit(Printing::GPrintJob *pJob)
	:CPrintUnitStandardTableForBitmap(pJob)
{
	m_nRowInEachPage = 3;
	m_nColumnsInEachPage = 3;
	m_pictureMargin = 2;
	m_currentWorkingColums = 0;

	SetNeedPreprocessSign(true);
}

Printing::CBitmapTableUnit::~CBitmapTableUnit(void)
{
}

int Printing::CBitmapTableUnit::PreviewUnit( BOOL bGetPageOnly, int from, int to )
{
	return Paint(from, to);
}

int Printing::CBitmapTableUnit::Paint( int from, int to )
{
	if (m_pData == NULL)
	{
		return FALSE;
	}

	EnvSetBeforePrinting();

	int printedPages = PrintContents(from, to);

	EnvCleanupAfterPrinting();

	return printedPages;
}


int Printing::CBitmapTableUnit::PrintContents( int from, int to, BOOL bPrintHeadingWhenChangePage /*= TRUE*/ )
{
	// if we have precalculate before, just skip
	if (GetNeedPreprocessSign() == true)
	{
		// all the preprocess stage will not increase the pages
		PreCalColumnWidth(from , to, bPrintHeadingWhenChangePage);
		PreCalRowHeight(from , to, bPrintHeadingWhenChangePage);
		
		SetNeedPreprocessSign(false);
	}

	// here it will actually print, and increase the pages
	// set preprocessing mode in order not to print pages that are before 'from'
	SetPreprocessValue(false);
	return DrawTableBitmaps(from , to, m_bNeedPrintTitleExcpetFirstPage);
}

void Printing::CBitmapTableUnit::PreCalRowHeight( int from, int to, BOOL bPrintHeadingWhenChangePage )
{
	RECT rectClient = JRECT;
	int totalHeight = abs(rectClient.bottom - rectClient.top);

	// to test the height of the text
	int textHeight = 0;
	{
		Printing::CMemDcNotDraw dc(&JDC);
		CDC* oldDC = m_pJob->m_pDC;
		m_pJob->m_pDC = &dc;
		textHeight = PrintTitle(FALSE);
		m_pJob->m_pDC = oldDC;
	}

	m_rowHeight = (int)((double)(totalHeight - textHeight - 2 * m_titleMargin)/(double)(m_nRowInEachPage));
}

void Printing::CBitmapTableUnit::PreCalColumnWidth( int from, int to, BOOL bPrintHeadingWhenChangePage )
{
	int clientWidth = abs(JRECT.right - JRECT.left);
	m_rowWidth = clientWidth / m_nColumnsInEachPage;
}

int Printing::CBitmapTableUnit::SetRowsInEachPage( int rowInEachPage )
{
	ASSERT(rowInEachPage > 0);

	int old = m_nRowInEachPage;
	m_nRowInEachPage = rowInEachPage;

	if (old != m_nRowInEachPage)
	{
		SetNeedPreprocessSign(true);
	}

	return old;
}


int Printing::CBitmapTableUnit::SetColumnsInEachPage( int columns )
{
	int old = m_nColumnsInEachPage;
	m_nColumnsInEachPage = columns;

	if (old != m_nColumnsInEachPage)
	{
		SetNeedPreprocessSign(true);
	}

	// prepare the columns
	m_bNeedResetColumns = true;
	m_vecColumnDef.clear();
	m_vecColumnDef.resize(columns);

	for (int i = 0; i < columns; i++)
	{
		double fPct = (double)1 / (double)columns;
		m_vecColumnDef[i].fPct = fPct;
	}

	return old;
}

int Printing::CBitmapTableUnit::DrawTableBitmaps( int from, int to, BOOL bPrintTitleWhenChangePage /*= FALSE*/ )
{
	int nRows = m_pData->size();

	// now check whether this page should be printed
	int currentPage = 1;
	m_bPrintThePage = currentPage >= from ? true : false;
	int printedPages = 0;

	int printedRows = 0;
	while(printedRows < nRows)
	{
		// do the actual job
		StartPage();
		printedPages++;
		if (printedRows == 0 || bPrintTitleWhenChangePage)
		{
			PrintTitleAndMoveCursor(!(printedRows == 0) && !GetPreprocessValue());
		}

		// traverse all the data
		int rowInThisPage = 0;	
		while(rowInThisPage < m_nRowInEachPage && printedRows < nRows)
		{
			StartRow();

			int currentWorkingColums = 0;
			while(currentWorkingColums < m_nColumnsInEachPage)
			{
				//paint the bitmap
				PrintBitmap(printedRows, currentWorkingColums);
				currentWorkingColums++;
			}

			// end the row
			m_sizeCurrentRow.cy = m_rowHeight;
			EndRow();

			printedRows++;
			rowInThisPage++;
		}
		EndPage();
		++currentPage;

		m_bPrintThePage = currentPage >= from? true : false;
		if (currentPage > to)
		{
			return currentPage - from + 1;
		}
	}

	return currentPage - from;
}

void Printing::CBitmapTableUnit::PrintBitmap( int printedRows, int currentWorkingColums )
{
	RECT rectToPaint;
	rectToPaint.left = JCUR.x + m_pictureMargin;
	rectToPaint.right = JCUR.x + m_rowWidth - m_pictureMargin;
	rectToPaint.top = JCUR.y + m_pictureMargin;
	rectToPaint.bottom = JCUR.y + m_rowHeight - m_pictureMargin;

	CDC dcMem;
	dcMem.CreateCompatibleDC(&JDC);
	CBitmap* bmp = (*m_pData)[printedRows][currentWorkingColums];
	//TODO: to assure that the bitmap is not null
	BITMAP bm;
	if (bmp)
	{
		bmp->GetBitmap(&bm);
	}
	CBitmap* oldBmp = (CBitmap*)dcMem.SelectObject((*m_pData)[printedRows][currentWorkingColums]);

	JDC.StretchBlt(
		rectToPaint.left,
		rectToPaint.top,
		rectToPaint.right - rectToPaint.left,
		rectToPaint.bottom - rectToPaint.top,
		&dcMem,
		0,
		0,
		bm.bmWidth,
		bm.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(oldBmp);

	JCUR.x += m_rowWidth;
}


BOOL Printing::CBitmapTableUnit::SetPrintData( vector<vector<CBitmap*>> *data )
{
	SetColumnsInEachPage((*data)[0].size());
	return CPrintUnitStandardTableForBitmap::SetPrintData(data);
}
