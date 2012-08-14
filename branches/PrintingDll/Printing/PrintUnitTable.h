#pragma once
#include "gfx_printjob.h"
#include "gfx_printunit.h"
#include "string"

namespace Printing
{
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

	class AFX_EXT_CLASS CPrintUnitStandardTable : public GPrintUnit
	{
	public:
		CPrintUnitStandardTable(GPrintJob *pJob = NULL);
		virtual ~CPrintUnitStandardTable();

	public:
		// define columns
		virtual void DefineColumns(vector<COLUMNDEFINITIONS>& columns);

		virtual BOOL SetRowFormat(UINT nFormat);

		// font related methods
		void SetHeadingFont(int nPointSize, LPCTSTR lpszFaceName);

	public:
		virtual void CompleteAllColHeadingsDefinition();
		virtual void CreatePrintFonts();
		virtual void DeleteDefaultFonts();
		BOOL CheckCurrentDCCompatibleWithPrevious();

	private:
		void GetCurrentTimeAndDate(CString& strDate, CString& time);

	protected:
		srtFont* m_pUserFontHeading;
		
		UINT m_nRowFormat;
		// column information
		vector<COLUMNDEFINITIONS> m_vecColumnDef;

		bool m_bNeedResetColumns;

		// the enumarate num of pages of this print task.
		// the value will be valis if the user has called GPrintJob::EvaluatePageNum()
		int pages;

		CDC * m_pPreviousDC;
	};

	class AFX_EXT_CLASS CPrintUnitStandardTableForBitmap : public CPrintUnitStandardTable
	{
	public:
		CPrintUnitStandardTableForBitmap(GPrintJob *pJob = NULL)
			: CPrintUnitStandardTable(pJob)
		{
		}
		virtual ~CPrintUnitStandardTableForBitmap() {}
		BOOL SetPrintData(vector<vector<CBitmap*>> *data)
		{
			if (data == NULL)
			{
				return FALSE;
			}
			m_pData = data;

			SetNeedPreprocessSign(true);

			return TRUE;
		}
	protected:
		vector<vector<CBitmap*>> *m_pData;
	};

	class AFX_EXT_CLASS CPrintUnitStandardTableForString : public CPrintUnitStandardTable
	{
	public:
		CPrintUnitStandardTableForString(GPrintJob *pJob = NULL)
			: CPrintUnitStandardTable(pJob)
		{
		}
		virtual ~CPrintUnitStandardTableForString(){}
		BOOL SetPrintData(vector<vector<CONST WCHAR*>> *data)
		{
			if (data == NULL)
			{
				return FALSE;
			}
			m_pData = data;

			SetNeedPreprocessSign(true);

			return TRUE;
		}
	protected:
		vector<vector<CONST WCHAR*>> *m_pData;
	};

}

