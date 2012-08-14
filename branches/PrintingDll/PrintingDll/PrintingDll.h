// PrintingDll.h : main header file for the PrintingDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPrintingDllApp
// See PrintingDll.cpp for the implementation of this class
//

class CPrintingDllApp : public CWinApp
{
public:
	CPrintingDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
