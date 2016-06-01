// PDlgFile.h: interface for the PDlgFile class.
//
//////////////////////////////////////////////////////////////////////


#ifndef _PDLGFILE_H__
#define _PDLGFILE_H__

#include	<windows.h>

#include <commdlg.h>
#include <vector>


class PDlgFile  
{
	char m_szFileTitle[40];		//	File Open Dialog Title caption
	char m_szPathFileName[MAX_PATH]; //	File Name...(전체 Full path Name..)
	static	char m_szPathName[MAX_PATH]; //	File Name...(전체 Full path Name..)
	char m_szFileName[80];		//	File Name...
	OPENFILENAME	head;
	static	std::vector<char*> m_ListFileName;
	static	std::vector<char*> m_ListTemp;
public:
	char*	GetPathFileName();
	char*	GetPathName();
	char*	GetTitleName();
	char*	GetFileName();				// Full path name을 전해준다.
	BOOL	GetCurrPathFileName(char* szPathFile);		// Full path name에서 현재 패스까지는 뺀다.
	char*	OpenFileName();
	char*	SaveFileName();
	void	SetTitleDef(char* szTitle,char* szFileName);
	void	SetDlgString(const char* extStr,const char* str2);
	void	SetHandle(HWND hWnd);	
	void	SetFlags(DWORD dwFlag);
	void	AddFlags(DWORD dwFlag);
	int		GetCount()								{return (int)m_ListFileName.size();		}
	char*	GetFileName(int nNum);
	static 	void	AddString(char* szFileName);
	static 	void	ClearString();
	static	void	ClearStringAll();
	static 	UINT_PTR CALLBACK OFNHookProcOldStyle(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam);
	static 	BOOL	OnIncludeItem(OFNOTIFYEX* pOFNEx, char* pResult);
					PDlgFile();
					~PDlgFile();

};


#endif 
