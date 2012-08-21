#include "StdAfx.h"

namespace Printing
{
	class IPrintable
	{
	public:
		// for all the deprived print unit task to override
		// return the pages this unit task will print
		virtual int PreviewUnit(CDC* pOriginDC, BOOL bGetPageOnly, int from, int to) = 0;
		// call to print the job, returns FALSE if printing should stop
		// 'from' is the page index based on the current unit, beginning with 1
		// 'to' may exceed the whole range
		virtual int Paint(int from, int to) = 0;
	};
}
