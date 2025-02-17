// VFWCAPTURE.h : main header file for the VFWCAPTURE application
//

#if !defined(AFX_VFWCAPTURE_H__D16E5E9C_1C85_4D3F_8344_8C455C82E6CD__INCLUDED_)
#define AFX_VFWCAPTURE_H__D16E5E9C_1C85_4D3F_8344_8C455C82E6CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVFWCAPTUREApp:
// See VFWCAPTURE.cpp for the implementation of this class
//

class CVFWCAPTUREApp : public CWinApp
{
public:
	CVFWCAPTUREApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVFWCAPTUREApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVFWCAPTUREApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VFWCAPTURE_H__D16E5E9C_1C85_4D3F_8344_8C455C82E6CD__INCLUDED_)
