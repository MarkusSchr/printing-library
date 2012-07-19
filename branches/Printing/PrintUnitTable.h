#pragma once
#include "gfx_printjob.h"
#include "gfx_printunit.h"
#include "string"

typedef struct tagColumnDefinitions
{
	wstring strName;
	double fPct; // percent
	UINT nFormat;

	tagColumnDefinitions()
	{
		nFormat = DT_CENTER | DT_WORD_ELLIPSIS;
		fPct = 0.2;
	}
}COLUMNDEFINITIONS, *LPCOLUMNDEFINITIONS;


class CPrintUnitStandardTable : public GPrintUnit
{
public:
	CPrintUnitStandardTable(GPrintJob *pJob = NULL);
	virtual ~CPrintUnitStandardTable();
	
private:
	// printing
	virtual int Paint(int from, int to);

public:
	// define columns
	void DefineColumns(vector<COLUMNDEFINITIONS>& columns);

	BOOL SetPrintData(vector<vector<LPCTSTR>> *data);
	BOOL SetRowFormat(UINT nFormat);

	// font related methods
	void SetHeadingFont(int nPointSize, LPCTSTR lpszFaceName);

protected:
	virtual void CompleteAllColHeadingsDefinition();
	virtual void CreatePrintFonts();
	virtual void DeleteDefaultFonts();
	virtual void InitPrintMetrics();

private:
	void PrepareDefaultFonts();

	void PrepareMetrics();
	void GetCurrentTimeAndDate(CString& strDate, CString& time);
	void CreateUserDefinedFont(CFont& fontDes, srtFont *fontSource);

	virtual int Preview(int from, int to);

	BOOL CheckCurrentDCCompatibleWithPrevious();

private:
	srtFont* m_pUserFontHeading;
	vector<vector<LPCTSTR>> *m_pData;
	UINT m_nRowFormat;
	// column information
	vector<COLUMNDEFINITIONS> m_vecColumnDef;

	bool m_bNeedResetColumns;

	// the enumarate num of pages of this print task.
	// the value will be valis if the user has called GPrintJob::EvaluatePageNum()
	int pages;

	CDC * m_pPreviousDC;
};
