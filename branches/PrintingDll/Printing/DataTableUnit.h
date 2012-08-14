#pragma once

#include "PrintUnitTable.h"

namespace Printing
{
	class AFX_EXT_CLASS CDataTableUnit : public CPrintUnitStandardTableForString
	{
	public:
		CDataTableUnit(GPrintJob *pJob = NULL);
		~CDataTableUnit(void);

	private:
		// the following two methods must be overrided
		// printing
		virtual int PreviewUnit(BOOL bGetPageOnly, int from, int to);
		virtual int Paint(int from, int to);
	};
}
