// TextMacro.h : main header file for the TEXTMACRO application
//

#if !defined(AFX_TEXTMACRO_H__936ABF79_5298_4499_8252_D6289A590108__INCLUDED_)
#define AFX_TEXTMACRO_H__936ABF79_5298_4499_8252_D6289A590108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTextMacroApp:
// See TextMacro.cpp for the implementation of this class
//

class CTextMacroApp : public CWinApp
{
public:
	CTextMacroApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextMacroApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTextMacroApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTMACRO_H__936ABF79_5298_4499_8252_D6289A590108__INCLUDED_)
