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
			m_pJob = job;
			m_pTable = new CPrintUnitMergableTable(job);
			m_pTable->SetNeedDrawHeadings(false);
			m_columnNum = 0;
			m_rowNum = 0;
		}

		~CHeaderFooterTable()
		{
			DELETE_IF_NOT_NULL(m_pTable)
		}

		CHeaderFooterTable& operator=(CHeaderFooterTable &other)
		{
			if (&other == this)
			{
				return *this;
			}

			// set row column
			SetRowColumnNum(other.m_rowNum, other.m_columnNum);
			
			// set cell merging
			m_vecMergedCells.clear();
			m_vecMergedCells.resize(0);
			for (int i = 0; i < other.m_vecMergedCells.size(); i++)
			{
				MergedCell temp(other.m_vecMergedCells[i]);
				MergeRows(temp.minRow, temp.minColumn, temp.maxRow, temp.maxColumn);
			}

			// set cell text
			m_cellTexts.clear();
			m_cellTexts.resize(0);
			for (int i = 0; i < other.m_cellTexts.size(); i++)
			{
				CellText temp(other.m_cellTexts[i]);
				SetRowUnitText(temp.row, temp.column, temp.text.c_str());
			}

			// set cell format
			m_cellFormat.clear();
			m_cellFormat.resize(0);
			for (int i = 0; i < other.m_cellFormat.size(); i++)
			{
				CellFormat temp(other.m_cellFormat[i]);
				SetRowUnitFormat(temp.row, temp.column, temp.format);
			}

			// set cell font
			m_cellFont.clear();
			m_cellFont.resize(0);
			for (int i = 0; i < other.m_cellFont.size(); i++)
			{
				CellFont temp(other.m_cellFont[i]);
				SetRowUnitFont(temp.row, temp.column, temp.nPoint, temp.fontName.c_str());
			}

			// set draw line or not
			NeedOuterLine(other.m_bNeedOuterLine);

			return *this;
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
				cd.fPct = (double)((double)1 / (double)columnNum);
				m_vecColumns.push_back(cd);
			}
			m_pTable->DefineColumns(m_vecColumns);
		}

		void MergeRows(int minRow, int minColumn, int maxRow, int maxColumn)
		{
			ASSERT(m_columnNum != 0 && m_rowNum != 0);

			MergedCell mergedCell;
			mergedCell.minRow = minRow;
			mergedCell.minColumn = minColumn;
			mergedCell.maxRow = maxRow;
			mergedCell.maxColumn = maxColumn;
			m_vecMergedCells.push_back(mergedCell);

			// the GridCtrl regards the first row of the table as the heading
			// so the first row of this table is the second row of GridCtrl
			m_pTable->MergeCell(minRow + 1, minColumn, maxRow + 1, maxColumn);
		}

		void SetRowUnitText(int row, int column, LPCTSTR text)
		{
			ASSERT(m_columnNum != 0 && m_rowNum != 0);

			CellText celltext;
			celltext.row = row;
			celltext.column = column;
			celltext.text = text;
			m_cellTexts.push_back(celltext);

			// deal with the columns specially
			// the cell no. 1 is row no.0
			m_pTable->SetCellText(row + 1, column, text);
		}

		void SetRowUnitFormat(int row, int column, int format)
		{
			ASSERT(m_columnNum != 0 && m_rowNum != 0);

			CellFormat cellformat;
			cellformat.row = row;
			cellformat.column = column;
			cellformat.format = format;
			m_cellFormat.push_back(cellformat);

			// deal with the columns specially
			// the cell no. 1 is row no.0
			m_pTable->SetCellFormat(row + 1, column, format);
		}

		void SetRowUnitFont(int row, int column, int nPoint, LPCTSTR fontName)
		{
			ASSERT(m_columnNum != 0 && m_rowNum != 0);

			CellFont cellfont;
			cellfont.row = row;
			cellfont.column = column;
			cellfont.nPoint = nPoint;
			cellfont.fontName = fontName;
			m_cellFont.push_back(cellfont);

			m_pTable->SetCellFont(row + 1, column, nPoint, fontName);
		}

		void NeedOuterLine(bool bNeed)
		{
			m_bNeedOuterLine = bNeed;
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

		bool m_bNeedOuterLine;
		
		struct MergedCell
		{
			int minRow;
			int minColumn;
			int maxRow;
			int maxColumn;
		};
		vector<MergedCell> m_vecMergedCells;

		struct CellText
		{
			int row;
			int column;
			wstring text;
		};
		vector<CellText> m_cellTexts;

		struct CellFormat
		{
			int row;
			int column;
			int format;
		};
		vector<CellFormat> m_cellFormat;

		struct CellFont
		{
			int row;
			int column;
			int nPoint;
			wstring fontName; 
		};
		vector<CellFont> m_cellFont;

		GPrintJob * m_pJob;
	};
}
