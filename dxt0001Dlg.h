// dxt0001Dlg.h : header file
//

#if !defined(AFX_DXT0001DLG_H__71404A1D_9BA2_4135_AC1E_BFE581E43F90__INCLUDED_)
#define AFX_DXT0001DLG_H__71404A1D_9BA2_4135_AC1E_BFE581E43F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDxt0001Dlg dialog

class CDxt0001Dlg : public CDialog
{
// Construction
public:
	CDxt0001Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDxt0001Dlg)
	enum { IDD = IDD_DXT0001_DIALOG };
	CListCtrl	m_inprice;
	CButton	m_stopbtn;
	CButton	m_startbtn;
	CListCtrl	m_list;
	CString	m_edit01;
	CString	m_stocknum;
	CString	m_avalue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDxt0001Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDxt0001Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXT0001DLG_H__71404A1D_9BA2_4135_AC1E_BFE581E43F90__INCLUDED_)
