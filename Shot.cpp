// Shot.cpp: implementation of the Shot class.
//
//////////////////////////////////////////////////////////////////////

#include "Shot.h"
#include "random.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Shot::Shot ()
{

}

Shot::Shot(int cyClient, int xShot, HBITMAP bShot)
{
	xPos = xShot;
	yPos = cyClient - 175;
	next = NULL;
	hShot = bShot;
}


Shot::~Shot()
{

}

int Shot::DrawShot(HWND hwnd, int cxClient, int cyClient)
{
	HDC	  hdc, hdcShot;
	
	if (yPos > 0)						//add actual client area height
	{
		RECT shotRect;
		hdc = GetDC (hwnd);
		yPos -= 20;

		hdcShot = CreateCompatibleDC (hdc) ;
		SelectObject (hdcShot, hShot) ;
		shotRect.left = xPos;
		shotRect.right = xPos + 8;
		shotRect.top = yPos + 30;
		shotRect.bottom = yPos + 50;
		InvalidateRect(hwnd, &shotRect, TRUE);

		BitBlt(hdc, xPos, yPos, 8, 29, hdcShot, 0, 0, SRCCOPY) ;

		DeleteDC (hdcShot) ;
		ReleaseDC(hwnd, hdc);
		return 1;
	}
	else
	{
		RECT shotRect;
		shotRect.left = xPos;
		shotRect.right = xPos + 8;
		shotRect.top = yPos;
		shotRect.bottom = yPos + 30;
		InvalidateRect(hwnd, &shotRect, TRUE);
		return 0;
	}
}




