// PDlgFile.cpp: implementation of the PDlgFile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "PDlgFile.h"
#include <stdlib.h>
#include <shtypes.h>
#include <shlobj.h>
#include <tchar.h>
#include <cderr.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
std::vector<char*> PDlgFile::m_ListFileName;
std::vector<char*> PDlgFile::m_ListTemp;
char			PDlgFile::m_szPathName[MAX_PATH];

PDlgFile::PDlgFile()
{
	strcpy(m_szPathFileName,"*.*");
	strcpy(m_szFileTitle,"No Title");

	head.lStructSize	=	sizeof(OPENFILENAME);
	head.hwndOwner		=	NULL;
	head.hInstance		=	NULL;
	head.lpstrFilter	=	"all Files(*.*)\0*.*\0";
	head.lpstrCustomFilter = NULL;
	head.nMaxCustFilter	=	0;
	head.nFilterIndex	=	0;
	head.lpstrFile		=	NULL;
	head.nMaxFile		=	_MAX_PATH;
	head.lpstrFileTitle =	NULL;
	head.nMaxFileTitle  =	_MAX_FNAME + _MAX_EXT;
	head.lpstrInitialDir=	NULL;
	head.lpstrTitle		=	NULL;
	head.Flags			=	OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	head.nFileOffset	=	0;
	head.nFileExtension	=	0;
	head.lpstrDefExt	=	"*.*";
	head.lCustData		=	0L;
	head.lpfnHook		=	NULL;
	head.lpTemplateName	=	NULL;

	head.lpstrFile		=	m_szPathFileName;
	head.lpstrFileTitle =	m_szFileTitle;
}

PDlgFile::~PDlgFile()
{

}
void PDlgFile::AddString(char* szFileName)
{
	char* szTemp = NULL;

	if(strlen(szFileName) > 0)
	{
		szTemp = new char[strlen(szFileName)+1];
		strcpy(szTemp,szFileName);
		m_ListTemp.push_back(szTemp);
	}
}
void PDlgFile::ClearStringAll()
{
	ClearString();
	m_ListFileName = m_ListTemp;
	m_ListTemp.clear();
	ClearString();
}
void PDlgFile::ClearString()
{
	char* szTemp = NULL;

	std::vector<char*>::iterator it;
	for(it=m_ListFileName.begin();it<m_ListFileName.end();it++)
		delete [] (*it);
	m_ListFileName.clear();
}
char* PDlgFile::GetFileName(int nNum)
{
	if(nNum > -1 && nNum < GetCount())
	{
		return m_ListFileName[nNum];
	}
	return NULL;
}
UINT_PTR CALLBACK PDlgFile::OFNHookProcOldStyle(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	char szTemp[80];
	OFNOTIFY* lpNotify = NULL;
	OFNOTIFYEX * lpNotifyEx = NULL;
	char* szFolder = NULL;
	char* szFileName = NULL;

	switch(uiMsg)
	{
	case WM_NOTIFY:
		lpNotify = (OFNOTIFY*)lParam;
		switch(lpNotify->hdr.code)
		{
		case CDN_INITDONE:
			ClearStringAll();
			break;
		case CDN_FILEOK:
			szFolder = NULL;
			break;
		case CDN_FOLDERCHANGE:
			GetCurrentDirectory(MAX_PATH,m_szPathName);
			ClearStringAll();
			break;
		case CDN_SELCHANGE:
			ClearString();
			m_ListFileName = m_ListTemp;
			m_ListTemp.clear();
			break;
		case CDN_TYPECHANGE:
			ClearStringAll();
			break;
		case CDN_INCLUDEITEM:
			OnIncludeItem((OFNOTIFYEX*)lParam,szTemp);
			AddString(szTemp);
			break;
		}
		break;
	}
	return 0;
}
// =================================================================
//
// =================================================================
void GetFileName (char * szName, char * szFullName)
{
	int nFullNameLen, i, nStart = 0 ;

	nFullNameLen = (int)strlen (szFullName) ;
	for (i = nFullNameLen - 1; i >= 0; i--)
	{
		if (szFullName[i] == '\\' || szFullName[i] == '/')
		{
			nStart = i + 1;
			break ;
		}
	}

	strcpy (szName, &szFullName[nStart]) ;
}
// =================================================================
//
// =================================================================
BOOL PDlgFile::OnIncludeItem(OFNOTIFYEX* pOFNEx, char* pResult)
{
	TCHAR buff[MAX_PATH];
	if(!SHGetPathFromIDList((LPCITEMIDLIST)pOFNEx->pidl, buff)) 
	{
		STRRET s;
		HRESULT hr = ((IShellFolder*)pOFNEx->psf)->GetDisplayNameOf((LPCITEMIDLIST)pOFNEx->pidl, SHGDN_NORMAL|SHGDN_FORPARSING, &s);
		if(S_OK != hr) 
			return FALSE;
		switch(s.uType)
		{
		case STRRET_CSTR: 
			_tcscpy(buff, s.cStr); 
			break;
		case STRRET_WSTR: 
			//						_tcscpy(buff, s.pOleStr); 
			SHFree(s.pOleStr); 
			break;
		default: 
			return FALSE;
		}
	}

	::GetFileName(pResult,buff);
	return TRUE;
}
void PDlgFile::SetHandle(HWND hWnd)
{
	head.hwndOwner		=	hWnd;
	head.Flags			=	OFN_HIDEREADONLY | OFN_CREATEPROMPT;
}

void PDlgFile::SetDlgString(const char * extStr, const char * str2)
{
	head.lpstrDefExt	=	extStr ;
	head.lpstrFilter	=	str2;
}

void PDlgFile::SetTitleDef(char * szTitle, char * szFileName)
{
	strcpy(m_szFileTitle,szTitle);
	strcpy(m_szPathFileName,szFileName);
}

char* PDlgFile::OpenFileName()
{
	char str[80];
	GetCurrentDirectory(80,str);
	head.lpstrInitialDir = str;
	int nCount = 0;
	if(::GetOpenFileName(&head))
		return m_szPathFileName;

	DWORD dwError = CommDlgExtendedError();
	switch(dwError)
	{
	case CDERR_DIALOGFAILURE:
		break;
	case CDERR_GENERALCODES:
		ClearStringAll();
		break;
	}
	return NULL;
}

char* PDlgFile::SaveFileName()
{
	char str[80];
	GetCurrentDirectory(80,str);
	head.lpstrInitialDir = str;
	if(::GetSaveFileName(&head))
		return m_szPathFileName;
	return FALSE;
}
char* PDlgFile::GetFileName()
{
	char szString[160];
	char* str1 = szString;
	char* str2 = NULL;


	strcpy(szString,m_szPathFileName);
	str1 = strrchr(szString,'\\');
	if(str1)
	{
		str1++;
		strcpy(m_szFileName,str1);
	}
	return m_szFileName;
}
void PDlgFile::SetFlags(DWORD dwFlag)
{
	head.Flags = dwFlag;

	if(head.Flags & OFN_EXPLORER)
	{
		head.Flags |=(OFN_ENABLEHOOK|OFN_ENABLEINCLUDENOTIFY);
		head.lpfnHook = (LPOFNHOOKPROC)OFNHookProcOldStyle;
	}
}
void PDlgFile::AddFlags(DWORD dwFlag)
{
	head.Flags |= dwFlag;
	if(head.Flags & OFN_EXPLORER)
	{
		head.Flags |=(OFN_ENABLEHOOK|OFN_ENABLEINCLUDENOTIFY);
		head.lpfnHook = (LPOFNHOOKPROC)OFNHookProcOldStyle;
	}
}
BOOL PDlgFile::GetCurrPathFileName(char* szPathFile)
{
	if(szPathFile)
	{
		char str[80];
		GetCurrentDirectory(80,str);
		strcpy(szPathFile,&m_szPathFileName[strlen(str)+1]);
		return TRUE;
	}

	return FALSE;
}
char* PDlgFile::GetTitleName()
{	
	return m_szFileTitle;
}

char* PDlgFile::GetPathFileName()
{
	return m_szPathFileName;
}
char* PDlgFile::GetPathName()
{
	return m_szPathName;
}
