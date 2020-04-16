// sndVibr.h : main header file for the SNDVIBR application
//

#if !defined(AFX_SNDVIBR_H__3B1C2A02_6BD8_4B23_ACBB_B136C2772539__INCLUDED_)
#define AFX_SNDVIBR_H__3B1C2A02_6BD8_4B23_ACBB_B136C2772539__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSndVibrApp:
// Класс приложения. Он первым создаётся и запускает всё остальное

class CSndVibrApp : public CWinApp
{
public:
	CSndVibrApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSndVibrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSndVibrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNDVIBR_H__3B1C2A02_6BD8_4B23_ACBB_B136C2772539__INCLUDED_)
