// printDlg.h : header file
//

#if !defined(AFX_PRINTDLG_H__C7033647_A4C7_11D3_963B_60B35DC10000__INCLUDED_)
#define AFX_PRINTDLG_H__C7033647_A4C7_11D3_963B_60B35DC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg dialog

class CPrintDlg : public CDialog
{
// Construction
public:
	CPrintDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintDlg)
	enum { IDD = IDD_PRINT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPrintDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_editRows;
private:
	CEdit m_editColumns;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTDLG_H__C7033647_A4C7_11D3_963B_60B35DC10000__INCLUDED_)
