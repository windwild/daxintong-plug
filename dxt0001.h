// dxt0001.h : main header file for the DXT0001 application
//

#if !defined(AFX_DXT0001_H__1D8192E7_E3C8_4D80_8869_50A2EECB0877__INCLUDED_)
#define AFX_DXT0001_H__1D8192E7_E3C8_4D80_8869_50A2EECB0877__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDxt0001App:
// See dxt0001.cpp for the implementation of this class
//

class CDxt0001App : public CWinApp
{
public:
	CDxt0001App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDxt0001App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDxt0001App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXT0001_H__1D8192E7_E3C8_4D80_8869_50A2EECB0877__INCLUDED_)
