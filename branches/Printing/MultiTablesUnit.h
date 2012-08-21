#include "StdAfx.h"
#include "Gfx_printunit.h"
#include "MergableTableUnit.h"

namespace Printing
{
	class CMultiTablesUnit : public CPrintUnitStandardTable<CONST WCHAR>
	{
	public:
		CMultiTablesUnit(GPrintJob *pJob = NULL);
		virtual ~CMultiTablesUnit();

	public:
		void InsertTables(CPrintUnitMergableTable& table);
		void ClearTables();

		void SetIntervalBetweenFirstTableInLineOfText(int intervalInLineOfText);
		void SetTableIntervalInLineOfText(int intervalInLineOfText);

	private:
		// the following two methods must be overrided
		// printing
		virtual int PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to);
		virtual int Paint(int from, int to);

	private:
		virtual void CreatePrintFonts();
		virtual void DeleteDefaultFonts();
		virtual void InitPrintMetrics();
		virtual void CompleteAllColHeadingsDefinition();

	private:
		vector<CPrintUnitMergableTable*> m_vecTables;
		int m_intervalBetweenTables;
		int m_intervalBetweenFirstTable;

		int m_printPagesTotal;
	};
}
