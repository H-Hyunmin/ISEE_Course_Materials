// FinalProc.h : main header file for the FINALPROC DLL
//

#if !defined(AFX_FINALPROC_H__CBBABD0C_91AF_4959_91D8_1C57E5FBDEAE__INCLUDED_)
#define AFX_FINALPROC_H__CBBABD0C_91AF_4959_91D8_1C57E5FBDEAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFinalProcApp
// See FinalProc.cpp for the implementation of this class
//

class CFinalProcApp : public CWinApp
{
public:
	CFinalProcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFinalProcApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFinalProcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINALPROC_H__CBBABD0C_91AF_4959_91D8_1C57E5FBDEAE__INCLUDED_)
