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

template<class T>
class CPrintUnitStandardTable : public GPrintUnit
{
public:
	CPrintUnitStandardTable(GPrintJob *pJob = NULL);
	virtual ~CPrintUnitStandardTable();

public:
	// define columns
	virtual void DefineColumns(vector<COLUMNDEFINITIONS>& columns);

	BOOL SetPrintData(vector<vector<T*>> *data);
	virtual BOOL SetRowFormat(UINT nFormat);

	// font related methods
	void SetHeadingFont(int nPointSize, LPCTSTR lpszFaceName);

protected:
	virtual void CompleteAllColHeadingsDefinition();
	virtual void CreatePrintFonts();
	virtual void DeleteDefaultFonts();
	BOOL CheckCurrentDCCompatibleWithPrevious();

private:
	void GetCurrentTimeAndDate(CString& strDate, CString& time);

protected:
	srtFont* m_pUserFontHeading;
	vector<vector<T*>> *m_pData;
	UINT m_nRowFormat;
	// column information
	vector<COLUMNDEFINITIONS> m_vecColumnDef;

	bool m_bNeedResetColumns;

	// the enumarate num of pages of this print task.
	// the value will be valis if the user has called GPrintJob::EvaluatePageNum()
	int pages;

	CDC * m_pPreviousDC;
};

template<class T>
CPrintUnitStandardTable<T>::CPrintUnitStandardTable( GPrintJob *pJob )
: GPrintUnit(pJob)
{
	m_bNeedHeaderSeparateLine = true;
	m_bNeedFooterSeperateLine = true;

	m_separateLineWidth = 2;
	m_separateLineInterval = 15;

	m_pData = NULL;
	m_nRowFormat = DT_CENTER;

	m_pUserFontHeading = NULL;

	m_bNeedResetColumns = true;

	pages = 0;

	m_pPreviousDC = NULL;
}

template<class T>
void CPrintUnitStandardTable<T>::SetHeadingFont( int nPointSize, LPCTSTR lpszFaceName )
{
	DELETE_IF_NOT_NULL(m_pUserFontHeading);
	m_pUserFontHeading = new srtFont(nPointSize, lpszFaceName);

	SetNeedPreprocessSign(true);
}

template<class T>
void CPrintUnitStandardTable<T>::DefineColumns( vector<COLUMNDEFINITIONS>& columns )
{
	m_vecColumnDef.clear();
	m_vecColumnDef.resize(columns.size());
	copy(columns.begin(), columns.end(), m_vecColumnDef.begin());

	m_bNeedResetColumns = true;
	SetNeedPreprocessSign(true);
}

template<class T>
void CPrintUnitStandardTable<T>::CreatePrintFonts()
{
	GPrintUnit::CreatePrintFonts();

	if (m_pUserFontHeading)
	{
		CreateUserDefinedFont(m_fontHeading, m_pUserFontHeading);
	}
}

template<class T>
void CPrintUnitStandardTable<T>::DeleteDefaultFonts()
{
	m_fontHeading.DeleteObject();

	GPrintUnit::DeleteDefaultFonts();
}

template<class T>
void CPrintUnitStandardTable<T>::CompleteAllColHeadingsDefinition()
{
	if (m_bNeedResetColumns)
	{
		ClearColumnSet();
	}

	for (int i = 0; m_bNeedResetColumns && i < (int)m_vecColumnDef.size(); i++)
	{
		InsertPrintCol(i, m_vecColumnDef[i].strName.c_str(), m_vecColumnDef[i].fPct, m_vecColumnDef[i].nFormat);
	}

	m_bNeedResetColumns = false;

	// must call base class
	GPrintUnit::CompleteAllColHeadingsDefinition();
}

template<class T>
CPrintUnitStandardTable<T>::~CPrintUnitStandardTable()
{
	DELETE_IF_NOT_NULL(m_pUserFontHeading);
	DELETE_IF_NOT_NULL(m_pFontTileSrt);
}

template<class T>
BOOL CPrintUnitStandardTable<T>::SetPrintData( vector<vector<T*>> *data )
{
	if (data == NULL)
	{
		return FALSE;
	}
	m_pData = data;

	SetNeedPreprocessSign(true);

	return TRUE;
}

template<class T>
BOOL CPrintUnitStandardTable<T>::SetRowFormat( UINT nFormat )
{
	if (!(nFormat == DT_CENTER || nFormat == DT_LEFT || nFormat == DT_RIGHT))
	{
		return FALSE;
	}
	m_nRowFormat = nFormat;

	SetNeedPreprocessSign(true);

	return TRUE;
}

template<class T>
BOOL CPrintUnitStandardTable<T>::CheckCurrentDCCompatibleWithPrevious()
{
	if (m_pPreviousDC == NULL)
	{
		m_pPreviousDC = &JDC;	
		return false;
	}
	else
	{
		// check if compatible
		// TODO : replace the following code to check whether the m_pDC is compatible with 
		// the input dc. Aicro does not find that function. return the right value

		// return compatible? true:false;
		return false;
	}
}

