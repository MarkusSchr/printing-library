#pragma once
#include "PrintUnitTable.h"

class CBitmapTableUnit : public CPrintUnitStandardTable<CBitmap>
{
public:
	CBitmapTableUnit(GPrintJob *pJob = NULL);
	~CBitmapTableUnit(void);

	int SetRowsInEachPage(int rowInEachPage);
	void SetTitle(LPCTSTR title, UINT nFormat = DT_CENTER);
	void SetTitlePen(int nPointSize, LPCTSTR lpszFaceName);
	// set the interval between the title , header and the table contents
	int SetTitleMargin(int titleMargin);
	virtual BOOL SetPrintData(vector<vector<CBitmap*>> *data);

private:
	// the following two methods must be overridden
	// printing
	virtual int PreviewUnit(int from, int to);
	virtual int Paint(int from, int to);
	int SetColumnsInEachPage(int columns);

private:
	// draw bitmap
	// return how many pages have been drawn
	int PrintContents(int from, int to, BOOL bPrintHeadingWhenChangePage = TRUE);
	
	void PreCalRowHeight(int from, int to, BOOL bPrintHeadingWhenChangePage);
	void PreCalRowStartPosition(int from, int to, BOOL bPrintHeadingWhenChangePage);
	int DrawTableBitmaps( int from, int to, BOOL bPrintTitleWhenChangePage = FALSE);

	void PrintBitmap( int printedRows, int currentWorkingColums );
	void PrintTitleAndMoveCursor();
	int PrintTitle();

private:
	RECT m_rectMargin;

	int m_rowHeight;
	int m_rowWidth;
	vector<int> m_rowPositionX;

	int m_nRowInEachPage;
	int m_nColumnsInEachPage;

	// title related attributes
	wstring m_title;
	UINT m_nTitleFormat;
	CFont m_FontTitle;
	srtFont * m_pFontTileSrt;
	int m_titleMargin;

	// margins for the picture in the table unit
	int m_pictureMargin;
};

