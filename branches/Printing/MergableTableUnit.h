#pragma once
#include "StdAfx.h"
#include "PrintUnitMergableTable.h"

namespace Printing
{
	class CMergableTableUnit : public CPrintUnitMergableTable
	{
	public:
		CMergableTableUnit(GPrintJob *pJob = NULL);
		~CMergableTableUnit(void);

	private:
		// the following two methods must be overrided
		// printing
		virtual int PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to);
		virtual int Paint(int from, int to);
		virtual int EveluateUnitPages(CDC* pOriginDC, int from, int to );

	private:
		int m_printPagesTotal;
	};
}
