#include "StdAfx.h"
#include "PrintUnitMergableTable.h"

namespace Printing
{
	// this table do not has the concept of column
	class CHeaderFooterTable
	{
	public:
		CHeaderFooterTable(GPrintJob *job)
		{
			m_pTable = new CPrintUnitMergableTable(job);
			m_pTable->SetNeedDrawHeadings(false);
			m_columnNum = 0;
		}

		~CHeaderFooterTable()
		{
			DELETE_IF_NOT_NULL(m_pTable)
		}

		void SetRowColumnNum(int rowNum, int columnNum)
		{
			// clear the formal call
			m_vecColumns.clear();
			m_vecColumns.resize(0);

			// set row
			m_rowNum = rowNum;
			m_pTable->SetRowNum(m_rowNum); // the first row is the one showing the heading

			// set column
			m_columnNum = columnNum;
			for (int i = 0; i < m_columnNum; i++)
			{
				COLUMNDEFINITIONS cd;
				cd.strName = L"never used";
				cd.fPct = 0.2;
				m_vecColumns.push_back(cd);
			}
			m_pTable->DefineColumns(m_vecColumns);
		}

		void MergeRows(int minRow, int minColumn, int maxRow, int maxColumn)
		{
			ASSERT(m_columnNum != 0 && m_rowNum != 0);

			// the GridCtrl regards the first row of the table as the heading
			// so the first row of this table is the second row of GridCtrl
			m_pTable->MergeCell(minRow + 1, minColumn, maxRow + 1, maxColumn);
		}

		void SetRowUnitText(int row, int column, LPCTSTR text)
		{
			ASSERT(m_columnNum != 0 && m_rowNum != 0);

			// deal with the columns specially
			// the cell no. 1 is row no.0
			m_pTable->SetCellText(row + 1, column, text);
		}

		void SetRowUnitFormat(int row, int column, int format)
		{
			ASSERT(m_columnNum != 0 && m_rowNum != 0);

			// deal with the columns specially
			// the cell no. 1 is row no.0
			m_pTable->SetCellFormat(row + 1, column, format);
		}

		void SetRowUnitFont(int row, int column, int nPoint, LPCTSTR fontName)
		{
			ASSERT(m_columnNum != 0 && m_rowNum != 0);
			m_pTable->SetCellFont(row + 1, column, nPoint, fontName);
		}

		void NeedOuterLine(bool bNeed)
		{
			m_pTable->NeedDrawTableOuterline(bNeed);
		}

	private:
		friend class GPrintUnit;

		void SetAllRowsFont(int nPointSize, LPCTSTR lpszFaceName)
		{
			m_pTable->SetAllRowsFont(nPointSize, lpszFaceName);
		}

		void Paint( CPrintInfo* info, int bodyFontPoints, LPCTSTR bodyFontName, CFont* printFont, CDC* pDc, int page, CRect rect, PntPrintEndResult *result)
		{
			m_pTable->SetBodyPrinterFont(bodyFontPoints, bodyFontName);
			
			// to save the JRECT in case the sub-table revise it
			// for all the units, including the footer and header are sharing the same job, 
			// if one of the sub-table changes the job, all other units are changed
			CRect oldRect = m_pTable->m_pJob->m_rectClient;

			m_pTable->EnvSetBeforePrinting();

			m_pTable->SetPrintFont(printFont);
			m_pTable->BeginPrinting(pDc, info, rect);
			m_pTable->Paint(pDc, page, rect, result);
			
			m_pTable->EnvCleanupAfterPrinting();

			m_pTable->m_pJob->m_rectClient = oldRect;
		}
		
		PRINTUNITMETRICS GetMetrics() 
		{
			return m_pTable->GetMetrics();	
		}

		void SetMetrics(PRINTUNITMETRICS pm)
		{
			m_pTable->SetMetrics(pm);
		}

		int GetRowNum()
		{
			return m_rowNum;
		}
	private:
		CPrintUnitMergableTable * m_pTable;
		vector<COLUMNDEFINITIONS> m_vecColumns;

		int m_columnNum;
		int m_rowNum;
	};
}
