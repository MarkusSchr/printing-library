#pragma once
#include "PrintUnitTable.h"
#include "GridCtrl/GridCtrl.h"
#include <map>
using namespace std;

namespace Printing
{
	// this table is designed as one-page-table
	class CPrintUnitMergableTable : public CPrintUnitStandardTable<CONST WCHAR>
	{
	public:
		CPrintUnitMergableTable(GPrintJob *pJob = NULL);
		virtual ~CPrintUnitMergableTable();

	private:
		virtual int PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to) {return -1;}
		virtual int Paint(int from, int to) {return -1;}
		virtual int EveluateUnitPages(CDC* pOriginDC, int from, int to) {return -1;}

	public:
		// call this method before any merge methods
		virtual void DefineColumns(vector<COLUMNDEFINITIONS>& columns);
		// if the merged cell exceeds the page, the system will restrict it into one page
		BOOL MergeCell(int minRow, int minColumn, int maxRow, int maxColumn);
		BOOL SetCellText(int row, int column, LPCTSTR text);

		// not including column row
		void SetRowNum(int nRowNum);
		int GetRowNum() {return m_nRows; }

		// set the column and the row height, they are in multiply of line-of-text
		void SetHeadingHeight(int heightInLineOfText);
		// row == 0, means changing column height
		void SetCellHeight(int nRow, int heightInLineOfText);

		// format related methods
		BOOL SetHeadingFormat( UINT nFormat );
		virtual BOOL SetAllRowFormat(UINT nFormat);
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
		void SetCellFont(int nRowIndex, int nColIndex, int nPointSize, LPCTSTR lpszFaceName);
		void SetHeadingFont( int nPointSize, LPCTSTR lpszFaceName );

		// need to draw column?
		void SetNeedDrawHeadings(bool bNeedDrawColumn){ m_bNeedToShowHeading = bNeedDrawColumn; }

		// set image in the cell
		void SetImageSize(int height, int width); 
		void SetCellImage(int row, int column, int nImageID);

	public:
		void SetPrintFont(CFont* font);
		int BeginPrinting(CDC* pDc, CPrintInfo* info, CRect rect);
		void Paint(CDC* pDc, int page, CRect rect, PntPrintEndResult *result = NULL);
		int PrintTitleAndMoveCursor(BOOL bNeedPrintContinue);

	public:
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
		void SetImage();


	protected:
		// the grid ctrl instance
		CPntGridCtrl* m_pGridCtrl;

		// indicate whether we have defined columns
		bool m_bHasDefinedColumns;

		// store all the merged cell position
		vector<CPntCellRange> m_mergeCellPos;

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
		map<int, int> m_cellHeight;

		// the top and bottom margin between headings and grid ctrl
		int m_topMarginInLineOfText, m_bottomMarginInLineOfText;

		// row, column and special cell's format
		UINT m_rowFormat, m_headingFormat;
		map<tagCell, UINT, ltCell> m_cellFormat;

		// switch to draw outer line
		bool m_bDrawOuterLine;

		// swith to show whether we need to show columns
		bool m_bNeedToShowHeading;

		struct ImageItem
		{
			int row;
			int column;
			int imageIndex;
		};
		vector<ImageItem> m_imageIndexLists;

		// the variable to contain the image list
		CImageList m_ImageList;
		int m_imageHeight, m_imageWidth; // the size of the image, which will be used to print the cell, attention that
										 // the printer will only print out the size we have specified here
	};
}
