#pragma once
#include "PrintUnitTable.h"
#include "GridCtrl/GridCtrl.h"
#include <map>
using namespace std;

// this table is designed as one-page-table
class CPrintUnitMergableTable : public CPrintUnitStandardTable<CONST WCHAR>
{
public:
	CPrintUnitMergableTable(GPrintJob *pJob = NULL);
	virtual ~CPrintUnitMergableTable();

public:
	// call this method before any merge methods
	virtual void DefineColumns(vector<COLUMNDEFINITIONS>& columns);
	// if the merged cell exceeds the page, the system will restrict it into one page
	BOOL MergeCell(int minRow, int minColumn, int maxRow, int maxColumn);
	BOOL SetCellText(int row, int column, LPCTSTR text);

	// not including column row
	void SetRowNum(int nRowNum);
	
	// set the column and the row height, they are in multiply of line-of-text
	void SetColumnRowHeight(int heightInLineOfText);
	// row == 0, means changing column height
	void SetRowHeight(int nRow, int heightInLineOfText);

	// format related methods
	BOOL SetColFormat( UINT nFormat );
	virtual BOOL SetRowFormat(UINT nFormat);
	BOOL SetCellFormat(int row, int column, UINT nFormat);

	// margin between header/footer and context
	// they are in multiply of line-of-text
	void SetTopMarginInLineOfText(int topMarginInLineOfText);
	void SetBottomMargin(int bottomMarginInLineOfText);

	// switch to drawing line
	void NeedDrawTableOuterline(bool bNeedOterLine) {m_bDrawOuterLine = bNeedOterLine; }

	// font related methods
	// inherited from base class
	// void SetHeadingFont(int nPointSize, LPCTSTR lpszFaceName);

	// if nRowIndex is 0, indicate setting heading font
	void SetAllRowsFont( int nPointSize, LPCTSTR lpszFaceName );
	void SetRowFont(int nRowIndex, int nPointSize, LPCTSTR lpszFaceName);
	void SetColFont(int nColIndex, int nPointSize, LPCTSTR lpszFaceName);
	void SetCellFont(int nRowIndex, int nColIndex, int nPointSize, LPCTSTR lpszFaceName);

protected:
	virtual void CompleteAllColHeadingsDefinition();

private:
	void SetTableTextFormat();
	void CreateGridCtrl();
	void DefineRowsAndColumns();
	void SetItemFont();
	void AdjustTopAndBottomMargin();
	void SetGridCellContents();
	void MergeGridCells();
	void SetOuterLine();


protected:
	// the grid ctrl instance
	CGridCtrl* m_pGridCtrl;
	
	// indicate whether we have defined columns
	bool m_bHasDefinedColumns;

	// store all the merged cell position
	vector<CCellRange> m_mergeCellPos;

	struct tagCell
	{
		int row;
		int column;
		tagCell(int r, int c)
		{
			row = r;
			column = c;
		}
	};
	struct ltCell
	{
		bool operator()(const tagCell s1, const tagCell s2) const
		{
			if (s1.row < s2.row)
				return true;
			else if (s1.row > s2.row)
				return false;
			else if (s1.column < s2.column)
				return true;
			else
				return false;
		}
	};
	map<tagCell, wstring, ltCell> m_cellText;


	// column and row font
	LOGFONT m_columnLogFont;
	LOGFONT m_rowLogFont;

	// special item font
	map<tagCell, LOGFONT, ltCell> m_cellLogFont;

	// indicate how many rows and columns are there in the grid ctrl
	int m_nRows;
	int m_nColumns;

	// whether we have changed rows' or columns' definition?
	bool m_bHasChangedRowOrColumns;

	// store the special user-defined row height, including the column's row
	map<int, int> m_rowHeight;

	// the top and bottom margin between headings and grid ctrl
	int m_topMarginInLineOfText, m_bottomMarginInLineOfText;

	// row, column and special cell's format
	UINT m_rowFormat, m_colFormat;
	map<tagCell, UINT, ltCell> m_cellFormat;

	// switch to draw outer line
	bool m_bDrawOuterLine;
};