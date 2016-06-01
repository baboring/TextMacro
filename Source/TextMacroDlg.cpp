// TextMacroDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TextMacro.h"
#include "TextMacroDlg.h"
#include "PDlgFile.h"
#include "DlgOption.h"
#include ".\textmacrodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextMacroDlg dialog

CTextMacroDlg::CTextMacroDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextMacroDlg::IDD, pParent)
	, m_szHeader(_T(""))
	, m_szTail(_T(""))
	, m_bHeader(FALSE)
	, m_bTail(FALSE)
	, m_bMinimize(FALSE)
	, m_nLoop(1)
{
	//{{AFX_DATA_INIT(CTextMacroDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_chkOnlyToApp = FALSE;
	m_timerID = 0;
	m_edtAppTitle = _T("Notepad");
	m_edtKeyStrokes = _T("{WAIT=1000}{DELAY=200}Hello {WAIT=1000}we{BS}orld!");
//	m_edtKeyStrokes = _T("{WAIT=1000}{DELAY=200}Hello {WAIT=1000}we{BS}orld!{MOUSE_MOVE 100 100}");
//	m_edtKeyStrokes = _T("{WAIT=1000}{CURSOR_POS 1053,11}{MOUSE_LDOWN 100}{CURSOR_POS 1053,11}{MOUSE_LUP 100}");
	m_szHeader =  _T("{WAIT=100}{CURSOR_POS 100,100}{MOUSE_LDOWN 100}{MOUSE_LUP 100}");
	m_szTail =  _T("~{WAIT=100}{CURSOR_POS 100,100}{MOUSE_LDOWN 100}{MOUSE_LUP 100}");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTextMacroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextMacroDlg)
	DDX_Check(pDX, IDC_CHK_TOAPP, m_chkOnlyToApp);
	DDX_Text(pDX, IDC_EDIT_APP, m_edtAppTitle);
	DDX_Text(pDX, IDC_EDIT_KEYSTROKES, m_edtKeyStrokes);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LST_LOG, m_ctlLog);
	DDX_Text(pDX, IDC_EDT_HEADER, m_szHeader);
	DDX_Text(pDX, IDC_EDT_TAIL, m_szTail);
	DDX_Check(pDX, IDC_CHK_HEADER, m_bHeader);
	DDX_Check(pDX, IDC_CHK_TAIL, m_bTail);
	DDX_Control(pDX, IDC_LST_MACRO, m_ctrIMacro);
	DDX_Control(pDX, IDC_LST_CURSOR, m_ctrCursor);
	DDX_Check(pDX, IDC_CHK_LOG, g_bLog);
	DDX_Check(pDX, IDC_CHK_MINIMIZE, m_bMinimize);
	DDX_Text(pDX, IDC_EDT_LOOP, m_nLoop);
	DDV_MinMaxInt(pDX, m_nLoop, 1, 1000000);
}

BEGIN_MESSAGE_MAP(CTextMacroDlg, CDialog)
	//{{AFX_MSG_MAP(CTextMacroDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BN_LOAD_LIST, OnBnClickedBnLoadList)
	ON_BN_CLICKED(IDC_BN_LOAD_INSERT, OnBnClickedBnLoadInsert)
	ON_BN_CLICKED(IDC_BN_RUN, OnBnClickedBnRun)
	ON_BN_CLICKED(IDC_BN_OPTION, OnBnClickedBnOption)
	ON_BN_CLICKED(IDC_CHK_HEADER, OnBnClickedChkHeader)
	ON_BN_CLICKED(IDC_CHK_TAIL, OnBnClickedChkTail)
	ON_BN_CLICKED(IDC_CHK_MINIMIZE, OnBnClickedChkMinimize)
	ON_BN_CLICKED(IDC_BN_LOAD_LIST_ALL, OnBnClickedBnLoadListAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextMacroDlg message handlers

BOOL CTextMacroDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// 시작 디렉토리 저장
	GetCurrentDirectory(128,m_szAppStartDir);

	int nColumn = 0;
	ListView_SetExtendedListViewStyleEx(m_ctrIMacro.m_hWnd, 0, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ctrIMacro.InsertColumn(nColumn++, "번호", LVCFMT_CENTER, 40 );
	m_ctrIMacro.InsertColumn(nColumn++, "내용", LVCFMT_LEFT, 340 );

	ListView_SetExtendedListViewStyleEx(m_ctrCursor.m_hWnd, 0, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ctrCursor.InsertColumn(nColumn++, "번호", LVCFMT_CENTER, 30 );
	m_ctrCursor.InsertColumn(nColumn++, "위치", LVCFMT_LEFT, 80 );

//	::SetCapture(m_hWnd);
	m_timerID = SetTimer(1011,10,NULL);
	// TODO: Add extra initialization here
	g_hMainFrame = m_hWnd;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTextMacroDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTextMacroDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
//		CDialog::OnPaint();

		CPaintDC dc(this); // device context for painting
		CWnd* lpWnd = GetDlgItem(IDC_PIC_CURSOR);
		if(lpWnd && g_hBitmap)
		{
			RECT rRect;
			lpWnd->GetClientRect(&rRect);
			OffsetRect(&rRect,2,2);
 			PaintBitmap( lpWnd->GetDC()->m_hDC , &rRect, g_hBitmap, &rRect, NULL); 

// 			BITMAP bm;
// 			PAINTSTRUCT ps;
// 
// 			HDC hdcMem = CreateCompatibleDC(dc.m_hDC);
// 			HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, g_hBitmap);
// 
// 			GetObject(g_hBitmap, sizeof(bm), &bm);
// 
// 			BitBlt(dc.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
// 
// 			SelectObject(hdcMem, hbmOld);
// 			DeleteDC(hdcMem);

		}
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTextMacroDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
DWORD WINAPI ThreadSendKeys(LPVOID arg)
{
	//	DWORD tid1;
	CTextMacroDlg* lpThis = (CTextMacroDlg*)arg;

	lpThis->SendKeyProcess();

	return 0;
}
void CTextMacroDlg::SendKeyProcess()
{
	DisableButtons();
	m_sk.SendKeys((LPCTSTR)m_edtKeyStrokes);
	EnableButtons();
}

void CTextMacroDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_chkOnlyToApp && !m_sk.AppActivate((LPCTSTR)m_edtAppTitle))
	{
		MessageBox(_T("Could not send to this application!"));
		return;
	}
	DWORD tid;
	CreateThread(0,0,&ThreadSendKeys,this,0,&tid);			

}

// ===========================================================
//
// ===========================================================
void CTextMacroDlg::AddLog(CString szLog)
{
	if(m_ctlLog.GetCount()>500)
		m_ctlLog.DeleteString(0);
	m_ctlLog.AddString(szLog);
	m_ctlLog.SetCurSel(m_ctlLog.GetCount()-1);
}
// ===========================================================
//
// ===========================================================
LRESULT CTextMacroDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch(message)
	{
	case WM_CLOSE:
	case WM_QUIT:
		Sleep(200);
		break;
	case UM_THREAD_MESSAGE:
		{
// 			m_ctrProgress.SetPos(wParam);
// 			SetDlgItemText(IDC_LABEL,((char*)lParam));
		}
		break;
	case UM_DEBUG_MESSAGE:
		{
			switch(wParam)
			{
			default:
				AddLog((char*)lParam);
				break;
			case DMG_MESSAGE:
				AddLog((char*)lParam);
				break;
			}
		}
		break;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL CTextMacroDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	::ReleaseCapture();
	if(m_timerID)
		KillTimer(m_timerID);
	m_timerID = 0;
	m_bStop = TRUE;

	if (g_hBitmap) 
		DeleteObject(g_hBitmap); 
	g_hBitmap = NULL;

	return CDialog::DestroyWindow();
}
void CTextMacroDlg::DrawMousePos()
{
	POINT pos;
	GetCursorPos(&pos);
	CWnd* lpWnd = GetDlgItem(IDC_LBL_CURSOR_POS);
	char szTemp[128];
	if(lpWnd)
	{
		sprintf(szTemp,"%4d,%4d",pos.x,pos.y);
		lpWnd->SetWindowText(szTemp);
	}
}
void CTextMacroDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	DrawMousePos();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CTextMacroDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	DrawMousePos();
	CDialog::OnMouseMove(nFlags, point);
}

void CTextMacroDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	DrawMousePos();
	CDialog::OnNcMouseMove(nHitTest, point);
}
// ===========================================================
//
// ===========================================================
void CTextMacroDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == m_timerID)
	{
		DrawMousePos();

		SHORT val=GetKeyState(VK_ESCAPE);
		if (val & 0x80000000)
		{
			m_bStop = TRUE;
		}
		SHORT capture=GetKeyState(VK_CONTROL);
		if (capture & 0x80000000)
		{
			RECT rRect;
			POINT posCursor;
			GetCursorPos(&posCursor);
			rRect.left = posCursor.x;
			rRect.top = posCursor.y;
			rRect.right = rRect.left + 50;
			rRect.bottom = rRect.top + 40;
			g_hBitmap = CopyScreenToBitmap(&rRect);

			BITMAP Bitmap;
			GetObject(g_hBitmap, sizeof(BITMAP), &Bitmap);

			Invalidate(FALSE);
		}
	}
	CDialog::OnTimer(nIDEvent);
}
// ===========================================================
//
// ===========================================================
int	CTextMacroDlg::LoadPhone(std::vector<CString>& lstString,CString szPullPathName,BOOL bDisplay)
{
	if(strlen(szPullPathName)<1)
		return 0;

	char	buf[512];
	char	tok[] = "=\t\r\n";
	char	*nametoken;

	m_bStop = FALSE;

	DisableButtons();

	lstString.clear();

	char szFileName[128];
	strcpy(szFileName,szPullPathName);
	strupr(szFileName);

	fstream			m_fs;
	m_fs.open( szFileName ,fstream::in);
	BOOL bResult = !m_fs;
	int nTotal = 0;
	CString szPhoneNum;
	if(bResult == FALSE)
	{
		while( !m_fs.eof() && m_bStop == FALSE )
		{
			m_fs.getline( buf, 512 );
			if( buf[0] == '/' || buf[0] == '#' || buf[0] == '*' )	// #,/,*이 먼저 나오는 문장은 주석처리
				continue;

			nametoken = strtok( buf, tok );
			if( !nametoken )
				continue;

			szPhoneNum = nametoken;
			szPhoneNum.TrimRight();
			if(m_bHeader)
				szPhoneNum = m_szHeader + szPhoneNum;
			if(m_bTail)
				szPhoneNum += m_szTail;
			lstString.push_back(szPhoneNum);

			nTotal++;
		}
	}
	EnableButtons();

	return nTotal;
}
// =========================================================
// =========================================================
void CTextMacroDlg::Load(CString szFullPathName,CString szFileName)
{
	int nPatterns = LoadPhone(m_lstString,szFileName);
// 	CWnd *hDlg	=	GetDlgItem(IDC_LABEL_LOAD_PATTERN);
// 	m_szLog.Format("%s (%s개)",szFileName,ConvertToCurrency(nPatterns));
// 	hDlg->SetWindowText(m_szLog);

	UpdateList();
	UpdateData(FALSE);
}
// =========================================================
// =========================================================
void CTextMacroDlg::LoadScript(CString szFullPathName,CString szFileName)
{
	int nPatterns = LoadScript(m_lstString,szFileName);
	UpdateList();
	UpdateData(FALSE);
}
// =========================================================
// =========================================================
void CTextMacroDlg::AddRow(int nRow,int nNo,char *fmt, ...)
{
	char buf[1024];
	va_list argptr;	

	va_start(argptr, fmt);
	vsprintf(buf, fmt, argptr);
	va_end(argptr);

	char szNo[128];
	itoa(nNo,szNo,10);
	m_ctrIMacro.InsertItem(nRow, szNo);
	m_ctrIMacro.SetItemText(nRow,1, buf);
}
// =========================================================
// =========================================================
void CTextMacroDlg::UpdateList()
{
	m_ctrIMacro.DeleteAllItems();

	std::vector<CString>::iterator it = m_lstString.begin();
	for(int i=0;it<m_lstString.end();it++,i++)
		AddRow(i,i+1,"%s",(*it));

}
// =========================================================
// =========================================================
void CTextMacroDlg::OnBnClickedBnLoadList()
{
	SetCurrentDirectory(m_szAppStartDir);
	PDlgFile dlgFile;

	UpdateData();
	dlgFile.SetHandle(m_hWnd);
	dlgFile.SetDlgString("*.txt","txt Files(*.txt)\0*.txt\0");
	dlgFile.SetTitleDef("txt File Load","*.txt");
	if(dlgFile.OpenFileName())
	{

		CString szFullPathName = dlgFile.GetPathFileName();		// FILE Name..Full path
		CString szFileName = dlgFile.GetFileName();  // FILE Name..only

		CFile			file;
		CFileStatus		fileStatus;

		//		m_sSrcDirName = szFullPathName.Left(szFullPathName.GetLength() - szFileName.GetLength());
		Load(szFullPathName,szFileName);

	}	
}

void CTextMacroDlg::OnBnClickedBnLoadInsert()
{
	CString szTemp =  _T("{WAIT=10000}{BEEP}");

	UpdateData();
//	m_lstString.insert(m_lstString.begin(),szTemp);
	m_lstString.insert(m_lstString.begin(),m_edtAppTitle);

	UpdateList();

}
DWORD WINAPI ThreadRun(LPVOID arg)
{
	//	DWORD tid1;
	CTextMacroDlg* lpThis = (CTextMacroDlg*)arg;

	lpThis->RunProcess();

	return 0;
}
void CTextMacroDlg::DisableButtons()
{
	if(m_bMinimize)
		ShowWindow(SW_MINIMIZE);
	DEBUG_MESSAGE("매크로 작업 시작");
	CWnd* lpWnd = GetDlgItem(IDOK);
	if(lpWnd)
		lpWnd->EnableWindow(FALSE);
	lpWnd = GetDlgItem(IDC_BN_RUN);
	if(lpWnd)
		lpWnd->EnableWindow(FALSE);
}
void CTextMacroDlg::EnableButtons()
{
	CWnd* lpWnd = GetDlgItem(IDOK);
	if(lpWnd)
		lpWnd->EnableWindow(TRUE);
	lpWnd = GetDlgItem(IDC_BN_RUN);
	if(lpWnd)
		lpWnd->EnableWindow(TRUE);
	DEBUG_MESSAGE("매크로 작업 완료");

	if(m_bMinimize)
		ShowWindow(SW_RESTORE);

}
void CTextMacroDlg::RunProcess()
{
	DisableButtons();

	m_bStop = FALSE;
	for(int i=0;i<m_nLoop;i++)
	{
		std::vector<CString>::iterator it = m_lstString.begin();
		for(int i=0;it<m_lstString.end();it++,i++)
		{
			if(m_bStop)
				break;
			DEBUG_MESSAGE("[%d] %s",i+1,(LPCTSTR)(*it));
			m_sk.SendKeys((LPCTSTR)(*it));
		}
	}
	EnableButtons();

}

void CTextMacroDlg::OnBnClickedBnRun()
{
	UpdateData();
	DWORD tid;
	CreateThread(0,0,&ThreadRun,this,0,&tid);

}

void CTextMacroDlg::OnBnClickedBnOption()
{
	CDlgOption dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		m_szHeader.Format("{WAIT=100}{CURSOR_POS %d,%d}{MOUSE_LDOWN 100}{MOUSE_LUP 100}",dlg.m_stX,dlg.m_stY);
		m_szTail.Format("~{WAIT=100}{CURSOR_POS %d,%d}{MOUSE_LDOWN 100}{MOUSE_LUP 200}",dlg.m_edX,dlg.m_edY);
		UpdateData(FALSE);
	}
}
void CTextMacroDlg::OnCancel()
{
	m_bStop = TRUE;
	if(AfxMessageBox("종료 하시겠습니까?",MB_YESNO) == IDYES)
		CDialog::OnCancel();
}

void CTextMacroDlg::OnBnClickedChkHeader()
{
	UpdateData();
	if(m_bHeader)
	{
		SetCurrentDirectory(m_szAppStartDir);
		PDlgFile dlgFile;

		UpdateData();
		dlgFile.SetHandle(m_hWnd);
		dlgFile.SetDlgString("*.txt","txt Files(*.txt)\0*.txt\0");
		dlgFile.SetTitleDef("txt File Load","*.txt");
		if(dlgFile.OpenFileName())
		{

			CString szFullPathName = dlgFile.GetPathFileName();		// FILE Name..Full path
			CString szFileName = dlgFile.GetFileName();  // FILE Name..only

			FILE* fp = fopen(szFullPathName,"rt");
			if(fp)
			{
				char szTemp[4096] = "";
				fgets(szTemp,4096,fp);
				fclose(fp);
				m_szHeader = szTemp;
				UpdateData(FALSE);
			}
		}	

	}
}

void CTextMacroDlg::OnBnClickedChkTail()
{
	UpdateData();
	if(m_bTail)
	{
		SetCurrentDirectory(m_szAppStartDir);
		PDlgFile dlgFile;

		UpdateData();
		dlgFile.SetHandle(m_hWnd);
		dlgFile.SetDlgString("*.txt","txt Files(*.txt)\0*.txt\0");
		dlgFile.SetTitleDef("txt File Load","*.txt");
		if(dlgFile.OpenFileName())
		{

			CString szFullPathName = dlgFile.GetPathFileName();		// FILE Name..Full path
			CString szFileName = dlgFile.GetFileName();  // FILE Name..only

			FILE* fp = fopen(szFullPathName,"rt");
			if(fp)
			{
				char szTemp[4096] = "";
				fgets(szTemp,4096,fp);
				fclose(fp);
				m_szTail = szTemp;
				UpdateData(FALSE);
			}
		}	

	}

}

void CTextMacroDlg::OnBnClickedChkMinimize()
{
	UpdateData();
}

void CTextMacroDlg::OnBnClickedBnLoadListAll()
{
	SetCurrentDirectory(m_szAppStartDir);
	PDlgFile dlgFile;

	UpdateData();
	dlgFile.SetHandle(m_hWnd);
	dlgFile.SetDlgString("*.txt","txt Files(*.txt)\0*.txt\0");
	dlgFile.SetTitleDef("txt File Load","*.txt");
	if(dlgFile.OpenFileName())
	{

		CString szFullPathName = dlgFile.GetPathFileName();		// FILE Name..Full path
		CString szFileName = dlgFile.GetFileName();  // FILE Name..only

		LoadScript(szFullPathName,szFileName);
	}
}
// ===========================================================
//
// ===========================================================
int	CTextMacroDlg::LoadScript(std::vector<CString>& lstString,CString szPullPathName,BOOL bDisplay)
{
	if(strlen(szPullPathName)<1)
		return 0;

	char	buf[2048];
	char	tok[] = "\r\n";
	char	*nametoken;

	m_bStop = FALSE;

	DisableButtons();

	lstString.clear();

	char szFileName[128];
	strcpy(szFileName,szPullPathName);
	strupr(szFileName);

	fstream			m_fs;
	m_fs.open( szFileName ,fstream::in);
	BOOL bResult = !m_fs;
	int nTotal = 0;
	CString szPhoneNum;
	if(bResult == FALSE)
	{
		while( !m_fs.eof() && m_bStop == FALSE )
		{
			m_fs.getline( buf, 2048 );

			nametoken = strtok( buf, tok );
			if( !nametoken )
				continue;

			szPhoneNum = nametoken;
			szPhoneNum.TrimRight();
			lstString.push_back(szPhoneNum);

			nTotal++;
		}
	}
	EnableButtons();

	return nTotal;
}