
#include "gfx_printunit.h"

class IndexUnit : public GPrintUnit
{
public:
   IndexUnit(GPrintJob *pJob);
   virtual ~IndexUnit();

   void InitPrintMetrics();
   virtual BOOL Print();

   GPrintIndexTree *m_pTree;
};

