// DlgOption.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TextMacro.h"
#include "DlgOption.h"
#include ".\dlgoption.h"

int CDlgOption::m_stX = 0;
int CDlgOption::m_stY = 0;
int CDlgOption::m_edX = 0;
int CDlgOption::m_edY = 0;

// CDlgOption ��ȭ �����Դϴ�.

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


// CDlgOption �޽��� ó�����Դϴ�.

void CDlgOption::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	OnOK();
}

BOOL CDlgOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_nTimerID = SetTimer(1200,20,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CDlgOption::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(m_nTimerID)
		KillTimer(m_nTimerID);
	m_nTimerID = 0;
	return CDialog::DestroyWindow();
}

void CDlgOption::OnTimer(UINT nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
