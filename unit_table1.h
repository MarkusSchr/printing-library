
#include "gfx_printunit.h"


class Table1Unit : public GPrintUnit
{
public:
   Table1Unit(GPrintJob *pJob);
   virtual ~Table1Unit();

   void DefineColHeadings();
   void CreatePrintFonts();
   void InitPrintMetrics();

   void PrintHeader();
   void PrintFooter();

   BOOL Print();

protected:
   PUFONTPAIR m_fontPairBody;
   CFont m_fontHeading;

   CFont m_fontHeader;
   CFont m_fontFooter;
};

