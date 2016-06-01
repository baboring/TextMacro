#pragma once


// CDlgOption 대화 상자입니다.

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
	CDlgOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnTimer(UINT nIDEvent);
};
