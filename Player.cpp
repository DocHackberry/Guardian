// Player.cpp: implementation of the Player class.
//
//////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "Globals.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Player::Player()
{
	//initial conditions
	name = NULL;
	pause = false;				//by default, game is not paused
	Score = 0;					//Score starts at 0 - OBVIOUSLY
	lives = 3;					//Player Starts with 3 lives
	level = 1;					//Level starts at 1 - AGAIN< OBVIOUS
}

Player::~Player()
{
	if(name)
		delete [] name;
}

int Player::DrawShip(HWND hwnd, HBITMAP hShip, int move, int cyClient, int cxClient, bool moved)
{
	HDC	hdc, hdcShip;						//Need two DCs to draw the ship in the client area
	hdc = GetDC (hwnd);						//Gets a DC for the screen

	shipx = ((cxClient / 2) - 39);			//Find the center point of the screen and subtract half of the ship's width.
											//  This will give the left side of the ship since the bitmap starts in the upper left.
	shipy = cyClient - 154;					//Ship's Y position will never change.

	//If the ship has moved, its old position will need to be repainted
	//	This gets the area around the ship that could be invalid
	if (moved)
	{
		
		RECT	shipRect;
		//account for the right side of the ship for if it is moing left
		shipRect.left = (shipx + move) + 74;
		shipRect.right = ((shipx + move) + 89);
		shipRect.top = shipy;
		shipRect.bottom = shipy + 74;
		InvalidateRect(hwnd, &shipRect, TRUE);

		//account for the left side of the ship if it is moving right
		shipRect.left = (shipx + move) - 15;
		shipRect.right = (shipx + move);
		InvalidateRect(hwnd, &shipRect, TRUE);
	}

	hdcShip = CreateCompatibleDC (hdc) ;			//Gets a DC for the Ship Bitmap
	SelectObject (hdcShip, hShip) ;					//Selects the ship Bitmap

	BitBlt(hdc, (shipx + move), shipy, 74, 74, hdcShip, 0, 0, SRCCOPY) ;	//Draws the Bitmap to the screen

	DeleteDC (hdcShip) ;							//Deletes the DC that was gotten for the ship
	ReleaseDC(hwnd, hdc);							//releases the screen DC
	return 0;
}

int Player::AddScore(HWND hwnd, Rocks &GameRocks, int x)
{
	int level2;
	Score += x;
	//Looks at the player's score, and moves them up one level when it reaches a certain number
	if (Score < 500)
		level2 = 1;

	//Increases the level at a certain point amount
	else if (Score > 500 && Score < 1000)
		level2 = 2;
	else if (Score > 1000 && Score < 1500)
		level2 = 3;
	else if (Score > 1500 && Score < 2000)
		level2 = 4;
	else if (Score > 2000 && Score < 2500)
		level2 = 5;
	else if (Score > 2500 && Score < 3000)
		level2 = 6;
	else if (Score > 3000 && Score < 3500)
		level2 = 7;
	else if (Score > 3500 && Score < 4000)
		level2 = 8;
	else if (Score > 4000 && Score < 4500)
		level2 = 9;
	else if (Score > 4500 && Score < 5000)
		level2 = 10;
	else if (Score > 5000 && Score < 5500)
		level2 = 11;
	else if (Score > 5500)
		level2 = 12;

	//Doesn't allow players to go back a level if their score drops
	if (level2 > level)
	{
		level = level2;
		SendMessage(hwnd, TIMER_UPDATE, NULL, NULL);
		if (level % 3 == 0)
			GameRocks.speed ++;
		if (level % 4 == 0)
			lives++;
	}
	return 0;
}

int Player::GetScore()
{
	return Score;
}

int Player::GetLevel()
{
	return level;
}

int Player::GetLives()
{
	return lives;
}

int Player::Getx()
{
	return shipx;
}

void Player::SetLives(int num)
{
	lives = num;
}

void Player::SetLevel(int num)
{
	level = num;
}


int Player::ShipHit(HWND hwnd, HINSTANCE hInst, int move)
{
	PlaySound("Hit.wav", NULL, SND_FILENAME | SND_ASYNC);

	HDC hdc, hExplode;
	hdc = GetDC (hwnd);
	hExplode = CreateCompatibleDC (hdc);

	HBITMAP	ShipExp1, ShipExp2, ShipExp3, ShipExp4;
	ShipExp1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIPEXP1));
	ShipExp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIPEXP2));
	ShipExp3 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIPEXP3));
	ShipExp4 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIPEXP4));

	//draws an explosion animation, and then clears it up
	SelectObject (hExplode, ShipExp1);
	BitBlt(hdc, (shipx + move), shipy, 74, 74, hExplode, 0, 0, SRCCOPY);
	Sleep(50);
	SelectObject (hExplode, ShipExp2);
	BitBlt(hdc, (shipx + move), shipy, 74, 74, hExplode, 0, 0, SRCCOPY);
	Sleep(50);
	SelectObject (hExplode, ShipExp3 );
	BitBlt(hdc, (shipx + move), shipy, 74, 74, hExplode, 0, 0, SRCCOPY);
	Sleep(50);
	SelectObject (hExplode, ShipExp4 );
	BitBlt(hdc, (shipx + move), shipy, 74, 74, hExplode, 0, 0, SRCCOPY);

	RECT exRect;
	exRect.top = shipy;
	exRect.bottom = shipy + 74;
	exRect.left = (shipx + move);
	exRect.right = (shipx + move) + 74;
	InvalidateRect(hwnd, &exRect, TRUE);

	DeleteDC (hExplode);
	ReleaseDC(hwnd, hdc);
	lives --;
	return 0;
}
