
#include "gfx_printunit.h"


class HeaderPage : public GPrintUnit
{
public:
   HeaderPage(GPrintJob *pJob);
   virtual ~HeaderPage();

   virtual BOOL Print();
};

