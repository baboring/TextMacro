// stdafx.cpp : source file that includes just the standard includes
//	TextMacro.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


HWND	g_hMainFrame = NULL;
BOOL	m_bStop = FALSE;
BOOL	g_bLog = TRUE;
HBITMAP g_hBitmap = NULL;
// =================================================================
//
// =================================================================
void THREAD_MESSAGE(int nPos,char *fmt, ...)
{
	char buf[512];
	va_list argptr;	

	va_start(argptr, fmt);
	vsprintf(buf, fmt, argptr);
	va_end(argptr);

	if(g_hMainFrame)
		::SendMessage( g_hMainFrame, UM_THREAD_MESSAGE, nPos, (LPARAM)buf);
}
// =================================================================
//
// =================================================================
void DEBUG_MESSAGE(char *fmt, ...)
{
	if(g_bLog)
	{

		char buf[512];
		va_list argptr;	

		va_start(argptr, fmt);
		vsprintf(buf, fmt, argptr);
		va_end(argptr);

		if(g_hMainFrame)
			::SendMessage( g_hMainFrame, UM_DEBUG_MESSAGE, 0, (LPARAM)buf);
	}
}
HBITMAP MakeDIBSection(HDC hdc, int width, int height)
{
	BITMAPINFO  bmi;
	LPVOID      pBits;
	HBITMAP     hBitmap;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount    = 24;          // 칼라수  : 1, 4, 8, 16, 24, 31
	bmi.bmiHeader.biWidth       = width;       //비트맵 너비
	bmi.bmiHeader.biHeight      = height;      //비트매 높이
	bmi.bmiHeader.biPlanes      = 1;
	hBitmap = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, (void **)&pBits, NULL, 0);
	return hBitmap;
}


HBITMAP CopyScreenToBitmap(LPRECT lpRect)
{
	HDC         hScrDC, hMemDC;         // screen DC and memory DC     
	//	HBITMAP     hBitmap; //, 
	//	HBITMAP     hBitmap;
	//	HBITMAP     hOldBitmap;    // handles to deice-dependent bitmaps     
	int         nX, nY, nX2, nY2;       // coordinates of rectangle to grab     
	int         nWidth, nHeight;        // DIB width and height     
	int         xScrn, yScrn;           // screen resolution      

	HGDIOBJ     hOldBitmap , hBitmap;

	// check for an empty rectangle 
	if (IsRectEmpty(lpRect))       
		return NULL;      
	// create a DC for the screen and create     
	// a memory DC compatible to screen DC          

	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);     
	hMemDC = CreateCompatibleDC(hScrDC);      // get points of rectangle to grab  

	nX = lpRect->left;     
	nY = lpRect->top;     
	nX2 = lpRect->right;     
	nY2 = lpRect->bottom;      // get screen resolution      

	xScrn = GetDeviceCaps(hScrDC, HORZRES);     
	yScrn = GetDeviceCaps(hScrDC, VERTRES);      

	//make sure bitmap rectangle is visible      

	if (nX < 0)         
		nX = 0;     

	if (nY < 0)         
		nY = 0;     

	if (nX2 > xScrn)         
		nX2 = xScrn;     

	if (nY2 > yScrn)         
		nY2 = yScrn;      

	nWidth = nX2 - nX;     
	nHeight = nY2 - nY;      

	// create a bitmap compatible with the screen DC     

//	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);      
	hBitmap = MakeDIBSection(hScrDC, nWidth, nHeight);    

	// select new bitmap into memory DC     

	hOldBitmap =   SelectObject (hMemDC, hBitmap);      

	// bitblt screen DC to memory DC     

	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);     

	// select old bitmap back into memory DC and get handle to     
	// bitmap of the screen          

	hBitmap = SelectObject(hMemDC, hOldBitmap);      

	// clean up      

	DeleteDC(hScrDC);     
	DeleteDC(hMemDC);      

	// return handle to the bitmap      

	return (HBITMAP)hBitmap; 

}

BOOL PaintBitmap(HDC hDC, LPRECT lpDCRect, HBITMAP hDDB, LPRECT lpDDBRect, HPALETTE hPal)
{
	HDC         hMemDC;            // Handle to memory DC 
	//    HBITMAP     hOldBitmap;        // Handle to previous bitmap 
	HPALETTE    hOldPal1 = NULL;   // Handle to previous palette 
	HPALETTE    hOldPal2 = NULL;   // Handle to previous palette 
	BOOL        bSuccess = FALSE;  // Success/fail flag 
	HGDIOBJ hOldBitmap;
	// Create a memory DC 

	hMemDC = CreateCompatibleDC (hDC); 

	// If this failed, return FALSE 

	if (!hMemDC) 
		return FALSE; 

	// If we have a palette, select and realize it 

	if (hPal) 
	{ 
		hOldPal1 = SelectPalette(hMemDC, hPal, TRUE); 
		hOldPal2 = SelectPalette(hDC, hPal, FALSE); //TRUE); 
		RealizePalette(hDC); 
	} 

	// Select bitmap into the memory DC 

	hOldBitmap = SelectObject (hMemDC, hDDB); 

	// Make sure to use the stretching mode best for color pictures 

	SetStretchBltMode (hDC, COLORONCOLOR); 

	// Determine whether to call StretchBlt() or BitBlt() 
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDDBRect)) && 
		(RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDDBRect))) 
		bSuccess = BitBlt(hDC, lpDCRect->left, lpDCRect->top, 
		lpDCRect->right - lpDCRect->left, 
		lpDCRect->bottom - lpDCRect->top, hMemDC, lpDDBRect->left, 
		lpDDBRect->top, SRCCOPY); 
	else 
		bSuccess = StretchBlt(hDC, lpDCRect->left,  lpDCRect->top,  
		lpDCRect->right - lpDCRect->left, 
		lpDCRect->bottom - lpDCRect->top, hMemDC, lpDDBRect->left,  
		lpDDBRect->top,  lpDDBRect->right - lpDDBRect->left, 
		lpDDBRect->bottom - lpDDBRect->top, SRCCOPY); 

	// Clean up 

	SelectObject(hMemDC, hOldBitmap); 

	if (hOldPal1) 
		SelectPalette (hMemDC, hOldPal1, FALSE); 

	if (hOldPal2) 
		SelectPalette (hDC, hOldPal2, FALSE); 

	DeleteDC (hMemDC); 

	// Return with success/fail flag 

	return bSuccess; 
}
BOOL CompareBitmap(HBITMAP hDDB1,HBITMAP hDDB2)
{
	if(hDDB1 && hDDB2)
	{
		BITMAP Bitmap1;
		GetObject(hDDB1, sizeof(BITMAP), &Bitmap1);
		BITMAP Bitmap2;
		GetObject(hDDB2, sizeof(BITMAP), &Bitmap2);

		if(Bitmap1.bmWidth != Bitmap2.bmWidth || Bitmap1.bmHeight != Bitmap2.bmHeight)
			return FALSE;

		DWORD* lpBits1 =  (DWORD*)Bitmap1.bmBits;
		DWORD* lpBits2 =  (DWORD*)Bitmap2.bmBits;
		for(int y=0;y<Bitmap1.bmHeight;y++)
			for(int x=0;x<Bitmap1.bmWidth;x++)
			{
				if(lpBits1[y*Bitmap1.bmWidth+x] != lpBits2[y*Bitmap1.bmWidth+x])
					return FALSE;
			}

		return TRUE;
	}
	return FALSE;
}
