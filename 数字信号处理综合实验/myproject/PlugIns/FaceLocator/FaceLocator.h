// FaceLocator.h : main header file for the FACELOCATOR DLL
//

#if !defined(AFX_FACELOCATOR_H__ABCD319D_4036_4579_88C7_E66296E5D396__INCLUDED_)
#define AFX_FACELOCATOR_H__ABCD319D_4036_4579_88C7_E66296E5D396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFaceLocatorApp
// See FaceLocator.cpp for the implementation of this class
//

class CFaceLocatorApp : public CWinApp
{
public:
	CFaceLocatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaceLocatorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFaceLocatorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACELOCATOR_H__ABCD319D_4036_4579_88C7_E66296E5D396__INCLUDED_)
