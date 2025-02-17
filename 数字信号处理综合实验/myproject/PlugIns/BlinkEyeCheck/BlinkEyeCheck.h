// BlinkEyeCheck.h : main header file for the BLINKEYECHECK DLL
//

#if !defined(AFX_BLINKEYECHECK_H__85051849_C52D_4290_BDB1_8169B5DE7607__INCLUDED_)
#define AFX_BLINKEYECHECK_H__85051849_C52D_4290_BDB1_8169B5DE7607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBlinkEyeCheckApp
// See BlinkEyeCheck.cpp for the implementation of this class
//

class CBlinkEyeCheckApp : public CWinApp
{
public:
	CBlinkEyeCheckApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlinkEyeCheckApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBlinkEyeCheckApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLINKEYECHECK_H__85051849_C52D_4290_BDB1_8169B5DE7607__INCLUDED_)
