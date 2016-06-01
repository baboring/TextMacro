// DlgOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TextMacro.h"
#include "DlgOption.h"
#include ".\dlgoption.h"

int CDlgOption::m_stX = 0;
int CDlgOption::m_stY = 0;
int CDlgOption::m_edX = 0;
int CDlgOption::m_edY = 0;

// CDlgOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgOption, CDialog)
CDlgOption::CDlgOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOption::IDD, pParent)
{
	m_nTimerID = 0;
}

CDlgOption::~CDlgOption()
{
}

void CDlgOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ST_POS_X, m_stX);
	DDX_Text(pDX, IDC_EDT_ST_POS_Y, m_stY);
	DDX_Text(pDX, IDC_EDT_ED_POS_X, m_edX);
	DDX_Text(pDX, IDC_EDT_ED_POS_Y, m_edY);
}


BEGIN_MESSAGE_MAP(CDlgOption, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgOption 메시지 처리기입니다.

void CDlgOption::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	OnOK();
}

BOOL CDlgOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_nTimerID = SetTimer(1200,20,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDlgOption::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_nTimerID)
		KillTimer(m_nTimerID);
	m_nTimerID = 0;
	return CDialog::DestroyWindow();
}

void CDlgOption::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == m_nTimerID)
	{
		SHORT val1=GetKeyState(VK_CONTROL);
		if (val1 & 0x80000000)
		{
			POINT pos;
			GetCursorPos(&pos);
			if(GetKeyState('1') & 0x80000000)
			{
				m_stX = pos.x;
				m_stY = pos.y;

				UpdateData(FALSE);
			}
			else
			if(GetKeyState('2') & 0x80000000)
			{
				m_edX = pos.x;
				m_edY = pos.y;

				UpdateData(FALSE);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}
