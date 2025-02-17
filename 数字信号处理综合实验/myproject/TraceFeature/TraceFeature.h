// TraceFeature.h : main header file for the TRACEFEATURE DLL
//

#if !defined(AFX_TRACEFEATURE_H__DB1200CD_96B2_48AF_B35E_AEE5DA345DD5__INCLUDED_)
#define AFX_TRACEFEATURE_H__DB1200CD_96B2_48AF_B35E_AEE5DA345DD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTraceFeatureApp
// See TraceFeature.cpp for the implementation of this class
//

class CTraceFeatureApp : public CWinApp
{
public:
	CTraceFeatureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTraceFeatureApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTraceFeatureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACEFEATURE_H__DB1200CD_96B2_48AF_B35E_AEE5DA345DD5__INCLUDED_)
