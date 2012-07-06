
#include "gfx_printunit.h"


class Table1Unit : public GPrintUnit
{
public:
   Table1Unit(GPrintJob *pJob);
   virtual ~Table1Unit();

   virtual void CompleteAllColHeadingsStartPosition();
   virtual void CreatePrintFonts();
   virtual void InitPrintMetrics();

   virtual void PrintHeader();
   virtual void PrintFooter();

   virtual BOOL Print();

protected:
   PUFONTPAIR m_fontPairBody;
   CFont m_fontHeading;

   CFont m_fontHeader;
   CFont m_fontFooter;
};

