#include "StdAfx.h"
#include "PrintUnitMergableTable.h"
#include <set>
#include <algorithm>
#include "MemDCForPrint.h"

using namespace std;

Printing::CPrintUnitMergableTable::CPrintUnitMergableTable( GPrintJob *pJob /*= NULL*/ )
	:CPrintUnitStandardTable(pJob)
{
	m_bHasDefinedColumns = false;
	m_bHasChangedRowOrColumns = false;

	m_pGridCtrl = NULL;
	m_nRows = m_nColumns = -1;

	m_topMarginInLineOfText = m_bottomMarginInLineOfText = 0;

	m_headingFormat = m_rowFormat = DT_CENTER | DT_WORDBREAK | DT_VCENTER | DT_SINGLELINE;

	m_bDrawOuterLine = true;

	m_bNeedToShowHeading = true;
}

Printing::CPrintUnitMergableTable::~CPrintUnitMergableTable()
{
	DELETE_IF_NOT_NULL(m_pGridCtrl);
}

void Printing::CPrintUnitMergableTable::DefineColumns( vector<COLUMNDEFINITIONS>& columns )
{
	m_vecColumnDef.clear();
	m_vecColumnDef.resize(columns.size());
	copy(columns.begin(), columns.end(), m_vecColumnDef.begin());

	m_nColumns = columns.size();

	m_bHasChangedRowOrColumns = true;
	m_bHasDefinedColumns = true;
}

void Printing::CPrintUnitMergableTable::SetRowNum(int nRowNum)
{
	if (nRowNum == m_nRows)
		return;

	m_nRows = nRowNum;

	// it will erase the former merge cells
	m_mergeCellPos.clear();
	m_mergeCellPos.resize(0);

	// side effect
	SetAllRowsFont(DEFAULT_FONT_HEIGHT, DEFAULT_FONT_NAME);

	m_bHasChangedRowOrColumns = true;
}

BOOL Printing::CPrintUnitMergableTable::MergeCell( int minRow, int minColumn, int maxRow, int maxColumn )
{
	ASSERT(m_nRows != 0 && m_nColumns != 0);

	CPntCellRange range(minRow,minColumn,maxRow,maxColumn);
	bool bLegal = (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 &&
		range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
		range.GetMaxRow() <= m_nRows && range.GetMaxCol() < m_nColumns &&
		range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
	
	if (bLegal)
	{
		m_mergeCellPos.push_back(range);
		m_bHasChangedRowOrColumns = true;
	}

	return bLegal;
}

BOOL Printing::CPrintUnitMergableTable::SetCellText( int row, int column, LPCTSTR text )
{
	bool bLegal = (row >= 0 && row <= m_nRows) && (column >= 0 && column <= m_nColumns);
	if (bLegal == false)
	{
		return FALSE;
	}

	tagCell tempCell(row, column);
	if (m_cellText.find(tempCell) != m_cellText.end())
	{
		m_cellText.erase(tempCell);
	}
	m_cellText.insert(std::make_pair(tempCell, wstring(text)));

	m_bHasChangedRowOrColumns = true;

	return TRUE;
}

void Printing::CPrintUnitMergableTable::CompleteAllColHeadingsDefinition()
{
	if (m_bHasChangedRowOrColumns == true)
	{
		m_bHasChangedRowOrColumns = false;

		// now we can create the GridCtrl
		CreateGridCtrl();

		DefineRowsAndColumns();
		
		SetTableTextFormat();

		MergeGridCells();

		SetGridCellContents();

		SetItemFont();
		
		SetOuterLine();

		SetImage();
	}

	// for this method will be called every time we've prepared the DC, following with the 
	// initialization of JRECT, so we need to change the JRECT every time this method is called
	// set margins
	AdjustTopAndBottomMargin();

	CPrintUnitStandardTable::CompleteAllColHeadingsDefinition();
}

BOOL Printing::CPrintUnitMergableTable::SetCellFormat( int row, int column, UINT nFormat )
{
	bool bLegal = (row >= 0 && row <= m_nRows) && (column >= 0 && column <= m_nColumns);
	if (bLegal == false)
	{
		return FALSE;
	}

	tagCell tempCell(row, column);
	if (m_cellFormat.find(tempCell) != m_cellFormat.end())
	{
		m_cellFormat.erase(tempCell);
	}
	m_cellFormat.insert(std::make_pair(tempCell, nFormat));

	m_bHasChangedRowOrColumns = true;

	return TRUE;
}

void Printing::CPrintUnitMergableTable::SetHeadingHeight( int heightInLineOfText )
{
	SetCellHeight(0, heightInLineOfText);
}

void Printing::CPrintUnitMergableTable::SetCellHeight( int nRow, int heightInLineOfText )
{
	m_cellHeight[nRow] = heightInLineOfText;
}

void Printing::CPrintUnitMergableTable::SetTopMarginInLineOfText( int topMarginInLineOfText )
{
	m_topMarginInLineOfText = topMarginInLineOfText;
}

void Printing::CPrintUnitMergableTable::SetBottomMargin( int bottomMarginInLineOfText )
{
	m_bottomMarginInLineOfText = bottomMarginInLineOfText;
}

BOOL Printing::CPrintUnitMergableTable::SetAllRowFormat( UINT nFormat )
{
	return m_rowFormat = nFormat;
}

BOOL Printing::CPrintUnitMergableTable::SetHeadingFormat( UINT nFormat )
{
	return m_headingFormat = nFormat;
}

void Printing::CPrintUnitMergableTable::SetTableTextFormat()
{
	// set row format
	for (int i = 1; i < m_nRows + 1; i++)
	{
		for (int j = 0; j < m_nColumns; j++)
		{
			m_pGridCtrl->SetCellFormat(i, j, m_rowFormat);
		}
	}

	// set column format
	for (int j = 0; j < m_nColumns; j++)
	{
		m_pGridCtrl->SetCellFormat(0, j, m_headingFormat);
	}

	// set special cell format
	map<tagCell, UINT, ltCell>::iterator itFormat = m_cellFormat.begin();
	for (; itFormat != m_cellFormat.end(); itFormat++)
	{
		m_pGridCtrl->SetCellFormat( (*itFormat).first.row, (*itFormat).first.column, (*itFormat).second);
	}
}

void Printing::CPrintUnitMergableTable::CreateGridCtrl()
{
	// first delete the former instance
	DELETE_IF_NOT_NULL(m_pGridCtrl);

	// create the grid ctrl
	m_pGridCtrl = new CPntGridCtrl(0,0,0,0,&JDC);

	m_pGridCtrl->Create(JINFO.m_rectDraw, AfxGetMainWnd(), 100);
	// fill it up with stuff
	m_pGridCtrl->SetEditable(FALSE);
	m_pGridCtrl->EnableDragAndDrop(FALSE);
}

void Printing::CPrintUnitMergableTable::DefineRowsAndColumns()
{
	// define rows
	// RowCount including the heading
	m_pGridCtrl->SetRowCount(m_nRows + 1); // '1' means heading row
	for (int i = 0; i < m_nRows + 1; i++)
	{
		int rowHeight = m_pum.pumLineOfText;
		if (m_cellHeight.find(i) != m_cellHeight.end())
		{
			rowHeight = m_cellHeight[i] * rowHeight;
		}

		m_pGridCtrl->SetRowHeight(i, rowHeight);
	}

	// define the columns
	int nSize = m_vecColumnDef.size();
	m_pGridCtrl->SetColumnCount(nSize);
	for (int col = 0; col < nSize; col++)
	{
		PNT_GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = m_headingFormat;
		Item.strText = m_vecColumnDef[col].strName.c_str();
		m_pGridCtrl->SetColumnWidth(col, (int)(JRECT.Width() * m_vecColumnDef[col].fPct));
		m_pGridCtrl->SetItem(&Item);
	}

	// if the user do not need the column, set the height of it to 0
	m_pGridCtrl->SetNeedDrawHeading(m_bNeedToShowHeading);
}

void Printing::CPrintUnitMergableTable::SetAllRowsFont( int nPointSize, LPCTSTR lpszFaceName )
{
	for (int i = 0; i < m_nRows; i++)
	{
		SetRowFont(i, nPointSize, lpszFaceName);
	}
}

void Printing::CPrintUnitMergableTable::SetRowFont( int nRowIndex, int nPointSize, LPCTSTR lpszFaceName )
{
	for (int i = 0; i < m_nColumns; i++)
	{
		SetCellFont(nRowIndex + 1, i, nPointSize, lpszFaceName);
	}
}

void Printing::CPrintUnitMergableTable::SetCellFont( int nRowIndex, int nColIndex, int nPointSize, LPCTSTR inLpszFaceName )
{
	LOGFONT logFont;
	GMakeStructFillZero(logFont);

	LPCTSTR lpszFaceName = inLpszFaceName;
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = nPointSize;
	lstrcpyn(logFont.lfFaceName, lpszFaceName, GGetArrayElementCount(logFont.lfFaceName));
	logFont.lfWeight = FW_BOLD;

	tagCell tempCell(nRowIndex, nColIndex);
	if (m_cellLogFont.find(tempCell) != m_cellLogFont.end())
	{
		m_cellLogFont.erase(tempCell);
	}
	m_cellLogFont.insert(std::make_pair(tempCell, logFont));
}

void Printing::CPrintUnitMergableTable::SetItemFont()
{
	// if we have changed the heading font
	if (NULL != m_pUserFontHeading)
	{
		// we changed the default font
		for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
		{
			LOGFONT lf;
			m_fontHeader.GetLogFont(&lf);
			m_pGridCtrl->SetItemFont(0, col, &lf);
		}
	}

	// set special cell font
	map<tagCell, LOGFONT, ltCell>::iterator itFont = m_cellLogFont.begin();
	for (; itFont != m_cellLogFont.end(); itFont++)
	{
		m_pGridCtrl->SetItemFont( (*itFont).first.row, (*itFont).first.column, &((*itFont).second));
	}
}

void Printing::CPrintUnitMergableTable::AdjustTopAndBottomMargin()
{
	JRECT.top += m_topMarginInLineOfText * m_pum.pumLineOfText;
	JRECT.bottom -= m_bottomMarginInLineOfText * m_pum.pumLineOfText;
}

void Printing::CPrintUnitMergableTable::MergeGridCells()
{
	// merge cell
	for (int i = 0 ; i < (int)(m_mergeCellPos.size()); i++)
	{
		m_pGridCtrl->MergeCells(m_mergeCellPos[i]);
	}
}

void Printing::CPrintUnitMergableTable::SetGridCellContents()
{
	// set cell contents
	map<tagCell, wstring, ltCell>::iterator itCellText = m_cellText.begin();
	for (; itCellText != m_cellText.end(); itCellText++)
	{
		m_pGridCtrl->SetItemText( (*itCellText).first.row, (*itCellText).first.column, (*itCellText).second.c_str());
	}
}

void Printing::CPrintUnitMergableTable::SetOuterLine()
{
	if (m_bDrawOuterLine == false)
	{
		m_pGridCtrl->SetGridLines(GVL_NONE);
	}
	else
	{
		m_pGridCtrl->SetGridLines(GVL_BOTH);
	}
}

void Printing::CPrintUnitMergableTable::SetPrintFont( CFont* font )
{
	m_pGridCtrl->SetPrintFont(font);
}

int Printing::CPrintUnitMergableTable::BeginPrinting( CDC* pDc, CPrintInfo* info, CRect rect )
{
	int index = pDc->SaveDC();
	int printedPages = m_pGridCtrl->OnBeginPrinting(pDc, info, rect);
	pDc->RestoreDC(index);
	return printedPages;
}

void CPrintUnitMergableTable::Paint( CDC* pDc, int page, CRect rect, PntPrintEndResult *result )
{
	m_pGridCtrl->OnPrint(pDc, page, rect, result);
}

int Printing::CPrintUnitMergableTable::PrintTitleAndMoveCursor( BOOL bNeedPrintContinue )
{
	return GPrintUnit::PrintTitleAndMoveCursor(bNeedPrintContinue);
}

void Printing::CPrintUnitMergableTable::SetHeadingFont( int nPointSize, LPCTSTR lpszFaceName )
{
	for (int i = 0; i < m_nColumns; i++)
	{
		SetCellFont(0, i, nPointSize, lpszFaceName);
	}
}

void Printing::CPrintUnitMergableTable::SetCellImage( int row, int column, int nImageID )
{
	ImageItem item;
	item.row = row;
	item.column = column;
	item.imageIndex = nImageID;
	m_imageIndexLists.push_back(item);
}

void Printing::CPrintUnitMergableTable::SetImage()
{
	m_ImageList.Create(m_imageWidth, m_imageHeight, ILC_COLORDDB, 0, 1);
	for (int i = 0; i < m_imageIndexLists.size(); i++)
	{
		CBitmap bm;
		int j = m_imageIndexLists[i].imageIndex;
		bm.LoadBitmap(m_imageIndexLists[i].imageIndex);
		m_ImageList.Add(&bm, RGB(0,0,0));
	}
	m_pGridCtrl->SetImageList(&m_ImageList);

	// set image item
	for (int i = 0; i < m_imageIndexLists.size(); i++)
	{
		m_pGridCtrl->SetItemImage(m_imageIndexLists[i].row, m_imageIndexLists[i].column, i);
	}
}

void Printing::CPrintUnitMergableTable::SetImageSize( int height, int width )
{
	m_imageHeight = height; 
	m_imageWidth = width;
}


