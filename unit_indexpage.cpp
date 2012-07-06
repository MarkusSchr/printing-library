
#include "stdafx.h"
#include "gfx_printjob.h"
#include "unit_indexpage.h"


IndexUnit::IndexUnit(GPrintJob *pJob) : GPrintUnit(pJob)
{
   m_pTree = new GPrintIndexTree();
}

IndexUnit::~IndexUnit()
{
   delete m_pTree;
}

void IndexUnit::InitPrintMetrics()
{
   TEXTMETRIC tm;
   JDC.GetTextMetrics(&tm);

   GMakeStructFillZero(m_pum);
   m_pum.pumLineOfText = tm.tmHeight;

   RealizeMetrics();
}

BOOL IndexUnit::Print()
{
   GPrintUnit::Print();

   StartPage();

   // add a title to the page
   CString strLine;
   strLine.Format("%cIndex\n", HFC_CENTER);
   PrintTextLine(strLine);

   PrintTree(m_pTree);
   EndPage();

   return TRUE;
}
