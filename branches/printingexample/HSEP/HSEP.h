// HSEP.h : main header file for the HSEP application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHSEPApp:
// See HSEP.cpp for the implementation of this class
//

class CHSEPApp : public CWinApp
{
public:
	CHSEPApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHSEPApp theApp;