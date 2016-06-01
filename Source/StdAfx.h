// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CD06FA67_CB58_4425_A14C_8E1000121138__INCLUDED_)
#define AFX_STDAFX_H__CD06FA67_CB58_4425_A14C_8E1000121138__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

#define		UM_THREAD_MESSAGE			WM_USER + 1
#define		UM_DEBUG_MESSAGE			WM_USER + 2

#define		DMG_START					0
#define		DMG_END						1
#define		DMG_STOP					2
#define		DMG_MESSAGE					3

#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left) 
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top) 


extern	HWND	g_hMainFrame;
extern	BOOL	m_bStop;
extern	BOOL	g_bLog;
extern	HBITMAP g_hBitmap;


void	THREAD_MESSAGE(int nPos,char *fmt, ...);
void	DEBUG_MESSAGE(char *fmt, ...);
HBITMAP CopyScreenToBitmap(LPRECT lpRect);
BOOL	CompareBitmap(HBITMAP hDDB1,HBITMAP hDDB2);
BOOL	PaintBitmap(HDC hDC, LPRECT lpDCRect, HBITMAP hDDB, LPRECT lpDDBRect, HPALETTE hPal);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CD06FA67_CB58_4425_A14C_8E1000121138__INCLUDED_)
