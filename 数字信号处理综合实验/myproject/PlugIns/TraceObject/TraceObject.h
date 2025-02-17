// TraceObject.h : main header file for the TRACEOBJECT DLL
//

#if !defined(AFX_TRACEOBJECT_H__D518230D_CE87_4235_A824_23ED5C8D7D31__INCLUDED_)
#define AFX_TRACEOBJECT_H__D518230D_CE87_4235_A824_23ED5C8D7D31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTraceObjectApp
// See TraceObject.cpp for the implementation of this class
//

class CTraceObjectApp : public CWinApp
{
public:
	CTraceObjectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTraceObjectApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTraceObjectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACEOBJECT_H__D518230D_CE87_4235_A824_23ED5C8D7D31__INCLUDED_)
