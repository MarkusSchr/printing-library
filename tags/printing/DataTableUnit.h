#pragma once

#include "PrintUnitTable.h"

namespace Printing
{
	class CDataTableUnit : public CPrintUnitStandardTable<CONST WCHAR>
	{
	public:
		CDataTableUnit(GPrintJob *pJob = NULL);
		~CDataTableUnit(void);

	private:
		// the following two methods must be overrided
		// printing
		virtual int PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to);
		virtual int Paint(int from, int to);
		virtual int EveluateUnitPages(CDC* pOriginDC, int from, int to);
	};
}
