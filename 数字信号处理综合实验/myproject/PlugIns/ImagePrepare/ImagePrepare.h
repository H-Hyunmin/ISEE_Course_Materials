// ImagePrepare.h : main header file for the IMAGEPREPARE DLL
//

#if !defined(AFX_IMAGEPREPARE_H__AFD73098_D19F_41B1_96B0_1439CC5134C6__INCLUDED_)
#define AFX_IMAGEPREPARE_H__AFD73098_D19F_41B1_96B0_1439CC5134C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImagePrepareApp
// See ImagePrepare.cpp for the implementation of this class
//

class CImagePrepareApp : public CWinApp
{
public:
	CImagePrepareApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImagePrepareApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CImagePrepareApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPREPARE_H__AFD73098_D19F_41B1_96B0_1439CC5134C6__INCLUDED_)
