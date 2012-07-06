// HSEPDoc.cpp : implementation of the CHSEPDoc class
//

#include "stdafx.h"
#include "HSEP.h"

#include "HSEPDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHSEPDoc

IMPLEMENT_DYNCREATE(CHSEPDoc, CDocument)

BEGIN_MESSAGE_MAP(CHSEPDoc, CDocument)
END_MESSAGE_MAP()


// CHSEPDoc construction/destruction

CHSEPDoc::CHSEPDoc()
{
	// TODO: add one-time construction code here

}

CHSEPDoc::~CHSEPDoc()
{
}

BOOL CHSEPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CHSEPDoc serialization

void CHSEPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CHSEPDoc diagnostics

#ifdef _DEBUG
void CHSEPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHSEPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHSEPDoc commands
