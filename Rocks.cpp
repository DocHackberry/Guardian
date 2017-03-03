// Rocks.cpp: implementation of the Rocks class.
//
//////////////////////////////////////////////////////////////////////

#include "Rocks.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Rocks::Rocks()
{
	first = NULL;
	current = NULL;
	index = NULL;
	total = 0;
	speed = 1;
}

Rocks::~Rocks()
{
	while(first)
	{
		index = first;
		first = first->next;
		delete index;
	}
}

int Rocks::CreateRock(int plevel, HWND hwnd, int cxClient)
{															//Must lock list while being deleted.
	if (!first)
	{
		first = new Rock(plevel, cxClient, lRock, sRock, Comet, Alien);
	}
	else if (!first->next)
	{
		current = new Rock(plevel, cxClient, lRock, sRock, Comet, Alien);
		first->next = current;
	}
	else
	{
		Rock* temp = new Rock(plevel, cxClient, lRock, sRock, Comet, Alien);
		current->next = temp;
		current = temp;
	}
	total++;
	return 0;
}

int Rocks::MoveRocks(HWND hwnd, int &countLost, int cxClient, int cyClient)
{
	if (!first)
		return 0;
	Rock* temp = NULL;
	int	temp2 = 0;
	int points = 0;
	index = first;
	while (index)
	{
		temp2 = index->DrawRock(hwnd, speed, cxClient, cyClient);
		if (!temp2)
		{
			if(temp2 != 1)
			{
				if (index->width == 95)
					Explode(hwnd, (index->x1 + 24), index->y1);
				else if (index->height == 90)
					Explode(hwnd, index->x1, (index->y1 + 42));
				else if (index->width != 98)
					Explode(hwnd, index->x1, index->y1);
				points -= (index->pointval) * 2;
			}
			RECT rockRect;
			rockRect.left = index->x1;
			rockRect.right = index->x2 + 8;
			rockRect.top = index->y1;
			rockRect.bottom = index->y2 + 8;
			InvalidateRect(hwnd, &rockRect, TRUE);
			countLost++;

			temp = index;
		}
		index = index->next;
		if(temp)
		{
			RemoveRock(temp);
			temp = NULL;
		}
	}
	return points;
}

int Rocks::DrawRocks(HWND hwnd, int cxClient, int cyClient)
{
	index = first;
	while (index)
	{
		index->DrawRock(hwnd, 0, cxClient, cyClient);
		index = index->next;
	}
	return 0;
}

int Rocks::HitRocks(HWND hwnd, int low, int high, int height, int cyClient)
{
	if (!first)
		return 0;
	int points = 0;
	index = first;
	while(index)
	{
		if (index->HitRock(low, high, height, cyClient))
		{
			if (index->width == 98)
				Explode(hwnd, (low - 20), index->y1);
			else if (index->width == 95)
				Explode(hwnd, (low - 20), index->y1);
			else if (index->height == 90)
				Explode(hwnd, index->x1, (index->y1 + 42));
			else
				Explode(hwnd, index->x1, index->y1);

			RECT rockRect;
			rockRect.left = index->x1 - 1;
			rockRect.right = index->x2 + 8;
			rockRect.top = index->y1 - 1;
			rockRect.bottom = index->y2 + 8;
			InvalidateRect(hwnd, &rockRect, TRUE);

			points = index->pointval;
			
			if (index->width == 95)
				SplitRock(index);
			else
				RemoveRock(index);
			return points;
		}
		index = index->next;
	}
	return 0;
}

bool Rocks::RemoveRock(Rock* remove)
{
	Rock* temp = first;

	if (remove == first)
	{
		first = first->next;
		delete remove;
	}
	else if (remove == first->next)
	{
		first->next = remove->next;
		delete remove;
	}
	else
	{
		while (temp->next != remove)
			temp = temp->next;

		temp->next = remove->next;				//current pointer not moved!!!!!!!
		delete remove;
	}
	total--;
	SetCurrent();
	return true;
}

bool Rocks::SplitRock(Rock* remove)
{
	Rock* temp;
	Rock* temp2;
	index = first;

	if (remove == first)
	{
		temp = new Rock(remove->x1, remove->y1, 1, sRock);
		temp->next = new Rock(remove->x1, remove->y1, 2, sRock);
		temp->next->next = first->next;
		first = temp;
		delete remove;
	}
	else if (remove == first->next)
	{
		temp = new Rock(remove->x1, remove->y1, 1, sRock);
		temp->next = new Rock(remove->x1, remove->y1, 2, sRock);
		temp->next->next = remove->next;
		first->next = temp;
		delete remove;
	}
	else
	{
		temp = new Rock(remove->x1, remove->y1, 1, sRock);				//Make Left Piece
		temp->next = new Rock(remove->x1, remove->y1, 2, sRock);		//Make Right Piece as the next one in the list
		temp->next->next = remove->next;								//Continue the list after the two new rocks

		temp2 = first;
		while (temp2->next != remove)							//Find the rock that is in the list before the original one that was hit
			temp2 = temp2->next;

		temp2->next = temp;
		delete remove;
	}
	total++;
	SetCurrent();
	return true;
}

void Rocks::Explode(HWND hwnd, int x, int y)
{
	PlaySound("Hit.wav", NULL, SND_FILENAME | SND_ASYNC);

	HDC hdc, hExplode;
	hdc = GetDC (hwnd);
	hExplode = CreateCompatibleDC (hdc);
	SelectObject (hExplode, Exp1);
	BitBlt(hdc, x, y, 48, 48, hExplode, 0, 0, SRCCOPY);
	Sleep(50);
	SelectObject (hExplode, Exp2);
	BitBlt(hdc, x, y, 48, 48, hExplode, 0, 0, SRCCOPY);
	Sleep(30);
	SelectObject (hExplode, Exp3 );
	BitBlt(hdc, x, y, 48, 48, hExplode, 0, 0, SRCCOPY);

	RECT exRect;
	exRect.top = y;
	exRect.bottom = y + 48;
	exRect.left = x;
	exRect.right = x + 48;
	InvalidateRect(hwnd, &exRect, TRUE);

	DeleteDC (hExplode);
	ReleaseDC(hwnd, hdc);
}

bool Rocks::HitShip(HWND hwnd, int shipx, int cyClient)
{
	if(!first)
		return false;
	else
	{
		index = first;
		while (index)
		{
			if (index->y2 > (cyClient - 154) && index->y1 < (cyClient - 80))
			{
				if(index->HitShip(shipx))
				{
					if (index->width == 95)
						Explode(hwnd, (index->x1 + 24), index->y1);
					else if (index->height == 90)
						Explode(hwnd, index->x1, (index->y1 + 42));
					else if (index->width != 98)
						Explode(hwnd, index->x1, index->y1);

					RemoveRock(index);
					
					InvalidateRect(hwnd, NULL, TRUE);

					return true;
				}
			}
			index = index->next;
		}
	}
	return false;
}

void Rocks::SetCurrent()
{
	if (first)
	{
		Rock* temp = first;
		while(temp->next)
		{
			temp = temp->next;
		}
		current = temp;
	}
}

void Rocks::LockList ()
{
	WaitForSingleObject(RockSema, INFINITE);
}

void Rocks::UnLockList ()
{
	ReleaseSemaphore(RockSema, 1, NULL); 
}