#pragma once


// CDlgOption ��ȭ �����Դϴ�.

class CDlgOption : public CDialog
{
	DECLARE_DYNAMIC(CDlgOption)

public:
	static	int m_stX;
	static	int m_stY;
	static	int m_edX;
	static	int m_edY;

			UINT m_nTimerID;

public:
	CDlgOption(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgOption();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnTimer(UINT nIDEvent);
};
