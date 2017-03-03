// Rock.cpp: implementation of the Rock class.
//
//////////////////////////////////////////////////////////////////////

#include "Rock.h"
#include "random.h"
#include "Globals.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Rock::Rock(int plevel, int cxClient, HBITMAP Big, HBITMAP Small, HBITMAP Comet, HBITMAP Alien)
{
	int x = randnum(1,100);
	if ((plevel >= 4) && ((x % 25) == 0))		//Creates an Alien Space Ship
	{
		rspeed = 1;
		pointval = 25;
		height = 45;
		width = 98;
		Image = Alien;
		x1 = 0;
		y1 = 10;
	}
	else if((plevel >= 2) && ((x % 17) == 0))	//Creates a Comet
	{
		rspeed = 1;
		pointval = 30;
		height = 90;
		width = 40;
		Image = Comet;
		x1 = randnum(20, cxClient - (width + 20));
		y1 = 0;
	}
	else if ((plevel >= 1) && ((x % 2) == 0))	//Creates a large rock
	{
		rspeed = 0;
		height = 45;
		width = 95;
		pointval = 15;
		Image = Big;
		x1 = randnum(20, cxClient - (width + 20));
		y1 = 0;
	}
	else			//Creates a small Rock
	{
		rspeed = 0;
		height = 45;
		width = 50;
		pointval = 10;
		Image = Small;
		x1 = randnum(20, cxClient - (width + 20));
		y1 = 0;
	}
	half = 0;
	breakpoint = 0;
	x2 = (x1 + width);
	y2 = (y1 + height);
	next = NULL;
}

Rock::Rock(int xPos, int yPos, int piece, HBITMAP Small)
{
	Image = Small;
	width = 50;
	height = 45;
	half = piece;
	if (piece == 1)
		x1 = xPos - 20;
	else if (piece == 2)
		x1 = xPos + 20;

	rspeed = 0;
	breakpoint = yPos;
	x2 = (x1 + width);
	y1 = yPos;
	y2 = (y1 + 40);
	next = NULL;
	pointval = 10;
}

Rock::~Rock()
{

}

int Rock::DrawRock(HWND hwnd, int move, int cxClient, int cyClient)
{
	HDC				hdc, hRock;
	RECT			rockRect;

	if (y2 < cyClient - 30)						//add actual client area height
	{
		if(x1 > cxClient)
			return 1;
		else if(x2 < 0)
			return 1;

		if (width != 98)
		{
			y1 += move + rspeed;
			y2 += move + rspeed;
			rockRect.left = x1;
			rockRect.right = x2;
			rockRect.top = y1 - (move + rspeed);
			rockRect.bottom = y1;
			InvalidateRect(hwnd, &rockRect, TRUE);
		}
		else
		{
			x1 += move + rspeed;
			x2 += move + rspeed;
			rockRect.left = (x1 - (move + rspeed));
			rockRect.right = x1;
			rockRect.top = y1;
			rockRect.bottom = y2;
			InvalidateRect(hwnd, &rockRect, TRUE);
		}

		if (half == 1)				//left half moves diagonally down and to the left
		{
			x1 -= move + rspeed; 
			x2 = x1 + width + rspeed;
			RECT Rock2;
			Rock2.right = x2 + (move + rspeed);
			Rock2.left = x2;
			Rock2.top = y1;
			Rock2.bottom = y2 + 10;
			InvalidateRect(hwnd, &Rock2, TRUE);
		}
		else if (half == 2)			//right half moves diagonally down and to the right
		{
			x1 += move + rspeed; 
			x2 = x1 + width + rspeed;
			RECT Rock2;
			Rock2.right = x1;
			Rock2.left = x1 - (move + rspeed);
			Rock2.top = y1;
			Rock2.bottom = y2 + 10;
			InvalidateRect(hwnd, &Rock2, TRUE);	
		}

		hdc = GetDC (hwnd);
		hRock = CreateCompatibleDC (hdc) ;
		SelectObject (hRock, Image) ;

		BitBlt(hdc, x1, y1, width, height, hRock, 0, 0, SRCCOPY) ;

		DeleteDC (hRock) ;
		ReleaseDC(hwnd, hdc);
		return 2;
	}
	return 0;
}

bool Rock::HitRock(int low, int high, int shoty, int cyClient)
{
	if (shoty <= y2 && shoty >= (y1 - 20))
	{
		if ((high > x1 && high < x2) || (low > x1 && low < x2))
		{
			if (y2 < (cyClient - 155))
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

bool Rock::HitShip (int shipx)
{
	if (x2 > shipx && x1 < shipx + 74)
	{
		return true;
	}
	return false;
}
