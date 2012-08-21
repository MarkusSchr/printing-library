#pragma once
#include "PrintUnitTable.h"

namespace Printing
{
	class CBitmapTableUnit : public CPrintUnitStandardTable<CBitmap>
	{
	public:
		CBitmapTableUnit(GPrintJob *pJob = NULL);
		~CBitmapTableUnit(void);

		int SetRowsInEachPage(int rowInEachPage);
		virtual BOOL SetPrintData(vector<vector<CBitmap*>> *data);

	private:
		// the following two methods must be overridden
		// printing
		virtual int PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to);
		virtual int Paint(int from, int to);
		int SetColumnsInEachPage(int columns);

	private:
		// draw bitmap
		// return how many pages have been drawn
		int PrintContents(int from, int to, BOOL bPrintHeadingWhenChangePage = TRUE);

		void PreCalRowHeight(int from, int to, BOOL bPrintHeadingWhenChangePage);
		void PreCalColumnWidth(int from, int to, BOOL bPrintHeadingWhenChangePage);
		int DrawTableBitmaps( int from, int to, BOOL bPrintTitleWhenChangePage = FALSE);

		void PrintBitmap( int printedRows, int currentWorkingColums );


	private:
		RECT m_rectMargin;

		int m_rowHeight;
		int m_rowWidth;
		vector<int> m_rowPositionX;

		int m_nRowInEachPage;
		int m_nColumnsInEachPage;

		// margins for the picture in the table unit
		int m_pictureMargin;
	};
}
