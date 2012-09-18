// print.h : main header file for the PRINT application
//

#if !defined(AFX_PRINT_H__C7033645_A4C7_11D3_963B_60B35DC10000__INCLUDED_)
#define AFX_PRINT_H__C7033645_A4C7_11D3_963B_60B35DC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPrintApp:
// See print.cpp for the implementation of this class
//

class CPrintApp : public CWinApp
{
public:
	CPrintApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPrintApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINT_H__C7033645_A4C7_11D3_963B_60B35DC10000__INCLUDED_)
