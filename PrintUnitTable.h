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
	
public:
	// printing
	virtual BOOL Print();
	BOOL Print(vector<vector<LPCTSTR>>* pPrintData, UINT nRowFormat);

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
	virtual void InitPrintMetrics();

private:
	void PrepareDefaultFonts();
	void PrepareMetrics();
	void GetCurrentTimeAndDate(CString& strDate, CString& time);
	void CreateUserDefinedFont(CFont& fontDes, srtFont *fontSource);

private:
	srtFont* m_pUserFontHeading;
	vector<vector<LPCTSTR>> *m_pData;
	UINT m_nRowFormat;
	// column information
	vector<COLUMNDEFINITIONS> m_vecColumnDef;
};
