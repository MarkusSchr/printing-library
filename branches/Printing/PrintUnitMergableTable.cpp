#include "StdAfx.h"
#include "PrintUnitMergableTable.h"
#include <algorithm>

CPrintUnitMergableTable::CPrintUnitMergableTable( GPrintJob *pJob /*= NULL*/ )
	:CPrintUnitStandardTable(pJob)
{
	m_bHasDefinedColumns = false;
	m_bHasChangedRowOrColumns = false;

	m_pGridCtrl = NULL;
	m_nRows = m_nColumns = -1;

	m_topMarginInLineOfText = m_bottomMarginInLineOfText = 0;

	m_colFormat = m_rowFormat = DT_CENTER | DT_WORDBREAK | DT_VCENTER | DT_SINGLELINE;
}

CPrintUnitMergableTable::~CPrintUnitMergableTable()
{
	DELETE_IF_NOT_NULL(m_pGridCtrl);
}

void CPrintUnitMergableTable::DefineColumns( vector<COLUMNDEFINITIONS>& columns )
{
	m_vecColumnDef.clear();
	m_vecColumnDef.resize(columns.size());
	copy(columns.begin(), columns.end(), m_vecColumnDef.begin());

	m_nColumns = columns.size();

	m_bHasChangedRowOrColumns = true;
	m_bHasDefinedColumns = true;
}

void CPrintUnitMergableTable::SetRowNum(int nRowNum)
{
	if (nRowNum == m_nRows)
		return;

	m_nRows = nRowNum;

	// it will erase the former merge cells
	m_mergeCellPos.clear();
	m_mergeCellPos.resize(0);

	m_bHasChangedRowOrColumns = true;
}

BOOL CPrintUnitMergableTable::MergeCell( int minRow, int minColumn, int maxRow, int maxColumn )
{
	ASSERT(m_nRows != 0 && m_nColumns != 0);

	CCellRange range(minRow,minColumn,maxRow,maxColumn);
	bool bLegal = (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 &&
		range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
		range.GetMaxRow() < m_nRows && range.GetMaxCol() < m_nColumns &&
		range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
	
	if (bLegal)
	{
		m_mergeCellPos.push_back(range);
		m_bHasChangedRowOrColumns = true;
	}

	return bLegal;
}

BOOL CPrintUnitMergableTable::SetCellText( int row, int column, LPCTSTR text )
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

void CPrintUnitMergableTable::CompleteAllColHeadingsDefinition()
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
	}

	// for this method will be called every time we've prepared the DC, following with the 
	// initialization of JRECT, so we need to change the JRECT every time this method is called
	// set margins
	AdjustTopAndBottomMargin();

	CPrintUnitStandardTable::CompleteAllColHeadingsDefinition();
}

BOOL CPrintUnitMergableTable::SetCellFormat( int row, int column, UINT nFormat )
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

void CPrintUnitMergableTable::SetColumnRowHeight( int heightInLineOfText )
{
	SetRowHeight(0, heightInLineOfText);
}

void CPrintUnitMergableTable::SetRowHeight( int nRow, int heightInLineOfText )
{
	m_rowHeight[nRow] = heightInLineOfText;
}

void CPrintUnitMergableTable::SetTopMarginInLineOfText( int topMarginInLineOfText )
{
	m_topMarginInLineOfText = topMarginInLineOfText;
}

void CPrintUnitMergableTable::SetBottomMargin( int bottomMarginInLineOfText )
{
	m_bottomMarginInLineOfText = bottomMarginInLineOfText;
}

BOOL CPrintUnitMergableTable::SetRowFormat( UINT nFormat )
{
	return m_rowFormat = nFormat;
}

BOOL CPrintUnitMergableTable::SetColFormat( UINT nFormat )
{
	return m_colFormat = nFormat;
}

void CPrintUnitMergableTable::SetTableTextFormat()
{
	// set row format
	for (int i = 1; i < m_nRows + 1; i++)
	{
		for (int j = 0; j < m_nColumns; j++)
		{
			m_pGridCtrl->SetItemFormat(i, j, m_rowFormat);
		}
	}

	// set column format
	for (int j = 0; j < m_nColumns; j++)
	{
		m_pGridCtrl->SetItemFormat(0, j, m_colFormat);
	}

	// set special cell format
	map<tagCell, UINT, ltCell>::iterator itFormat = m_cellFormat.begin();
	for (; itFormat != m_cellFormat.end(); itFormat++)
	{
		m_pGridCtrl->SetItemFormat( (*itFormat).first.row, (*itFormat).first.column, (*itFormat).second);
	}
}

void CPrintUnitMergableTable::CreateGridCtrl()
{
	// first delete the former instance
	DELETE_IF_NOT_NULL(m_pGridCtrl);

	// create the grid ctrl
	m_pGridCtrl = new CGridCtrl;

	m_pGridCtrl->Create(JINFO.m_rectDraw, AfxGetMainWnd(), 100);
	// fill it up with stuff
	m_pGridCtrl->SetEditable(FALSE);
	m_pGridCtrl->EnableDragAndDrop(FALSE);
}

void CPrintUnitMergableTable::DefineRowsAndColumns()
{
	// define rows
	m_pGridCtrl->SetRowCount(m_nRows + 1); // '1' means column row
	for (int i = 0; i < m_nRows + 1; i++)
	{
		int rowHeight = m_pum.pumLineOfText;
		if (m_rowHeight.find(i) != m_rowHeight.end())
		{
			rowHeight = m_rowHeight[i] * rowHeight;
		}

		m_pGridCtrl->SetRowHeight(i, rowHeight);
	}

	// define the columns
	int nSize = m_vecColumnDef.size();
	m_pGridCtrl->SetColumnCount(nSize);
	for (int col = 0; col < nSize; col++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = m_colFormat;
		Item.strText = m_vecColumnDef[col].strName.c_str();
		m_pGridCtrl->SetColumnWidth(col, JRECT.Width() * m_vecColumnDef[col].fPct);
		m_pGridCtrl->SetItem(&Item);
	}
}

void CPrintUnitMergableTable::SetAllRowsFont( int nPointSize, LPCTSTR lpszFaceName )
{
	for (int i = 0; i < m_nRows + 1; i++)
	{
		SetRowFont(i, nPointSize, lpszFaceName);
	}
}

void CPrintUnitMergableTable::SetRowFont( int nRowIndex, int nPointSize, LPCTSTR lpszFaceName )
{
	for (int i = 0; i < m_nColumns; i++)
	{
		SetCellFont(nRowIndex, i, nPointSize, lpszFaceName);
	}
}

void CPrintUnitMergableTable::SetColFont( int nColIndex, int nPointSize, LPCTSTR lpszFaceName )
{
	for (int i = 0; i < m_nRows; i++)
	{
		SetCellFont(i, nColIndex, nPointSize, lpszFaceName);
	}
}

void CPrintUnitMergableTable::SetCellFont( int nRowIndex, int nColIndex, int nPointSize, LPCTSTR inLpszFaceName )
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

void CPrintUnitMergableTable::SetItemFont()
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

void CPrintUnitMergableTable::AdjustTopAndBottomMargin()
{
	JRECT.top += m_topMarginInLineOfText * m_pum.pumLineOfText;
	JRECT.bottom -= m_bottomMarginInLineOfText * m_pum.pumLineOfText;
}

void CPrintUnitMergableTable::MergeGridCells()
{
	// merge cell
	for (int i = 0 ; i < m_mergeCellPos.size(); i++)
	{
		m_pGridCtrl->MergeCells(m_mergeCellPos[i]);
	}
}

void CPrintUnitMergableTable::SetGridCellContents()
{
	// set cell contents
	map<tagCell, wstring, ltCell>::iterator itCellText = m_cellText.begin();
	for (; itCellText != m_cellText.end(); itCellText++)
	{
		m_pGridCtrl->SetItemText( (*itCellText).first.row, (*itCellText).first.column, (*itCellText).second.c_str());
	}
}


