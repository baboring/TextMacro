// TextMacroDlg.h : header file
//

#if !defined(AFX_TEXTMACRODLG_H__F9E2E85A_157E_4F99_9BBF_ADB78495F302__INCLUDED_)
#define AFX_TEXTMACRODLG_H__F9E2E85A_157E_4F99_9BBF_ADB78495F302__INCLUDED_

#include "SendKeys.h"	// Added by ClassView
#include "afxwin.h"
#include "afxcmn.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTextMacroDlg dialog

class CTextMacroDlg : public CDialog
{
// Construction
public:
	CTextMacroDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTextMacroDlg)
	enum { IDD = IDD_TEXTMACRO_DIALOG };
	BOOL	m_chkOnlyToApp;
	CString	m_edtAppTitle;
	CString	m_edtKeyStrokes;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextMacroDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CSendKeys	m_sk;
	UINT		m_timerID;
	char		m_szAppStartDir[128];	// 시작 디렉토리
	std::vector<CString>	m_lstString;					// 패턴 리스트

public:
	CString m_szHeader;
	CString m_szTail;
	BOOL m_bHeader;
	BOOL m_bTail;
	CListBox m_ctlLog;
	CListCtrl m_ctrIMacro;
	CListCtrl m_ctrCursor;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTextMacroDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void	AddLog(CString szLog);
	void	SendKeyProcess();
	void	DrawMousePos();
	int		LoadScript(std::vector<CString>& lstString,CString szPullPathName,BOOL bDisplay = FALSE);
	void	LoadScript(CString szFullPathName,CString szFileName);
	int		LoadPhone(std::vector<CString>& lstString,CString szPullPathName,BOOL bDisplay = FALSE);
	void	Load(CString szFullPathName,CString szFileName);
	void	AddRow(int nRow,int nNo,char *fmt, ...);
	void	UpdateList();
	void	RunProcess();
	void	DisableButtons();
	void	EnableButtons();
	void	OnCancel();


	afx_msg void OnBnClickedOk();

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL DestroyWindow();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedBnLoadList();
	afx_msg void OnBnClickedBnLoadInsert();
	afx_msg void OnBnClickedBnRun();
	afx_msg void OnBnClickedBnOption();
	afx_msg void OnBnClickedChkHeader();
	afx_msg void OnBnClickedChkTail();
	afx_msg void OnBnClickedChkMinimize();
	BOOL m_bMinimize;
	afx_msg void OnBnClickedBnLoadListAll();
	int m_nLoop;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTMACRODLG_H__F9E2E85A_157E_4F99_9BBF_ADB78495F302__INCLUDED_)
