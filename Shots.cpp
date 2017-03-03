// Shots.cpp: implementation of the Shots class.
//
//////////////////////////////////////////////////////////////////////

#include "Shots.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Shots::Shots()
{
	first = NULL;
	current = NULL;
	index = NULL;
	CreateSemaphore(NULL, 1, 1, NULL);
}

Shots::~Shots()
{
	while(first)
	{
		index = first;
		first = first->next;
		delete index;
	}
}

int Shots::CreateShot(HWND hwnd, int cyClient, int xShot, HBITMAP bShot)
{															//Must lock list while being deleted.
	if (!first)
	{
		first = new Shot(cyClient, xShot, bShot);
	}
	else if (!first->next)
	{
		current = new Shot(cyClient, xShot, bShot);
		first->next = current;
	}
	else
	{
		Shot* temp = new Shot(cyClient, xShot, bShot);
		current->next = temp;
		current = temp;
	}
	return 0;
}

int Shots::MoveShots(HWND hwnd, int cxClient, int cyClient)
{
	if (!first)
		return 0;
	Shot* temp = NULL;
	index = first;
	while (index)
	{
		if (!index->DrawShot(hwnd, cxClient, cyClient))
		{
			temp = index;
		}
		index = index->next;
		if(temp)
		{
			RemoveShot(temp);
			temp = NULL;
		}
	}
	return 1;
}

int Shots::DrawShots(HWND hwnd, int cxClient, int cyClient)
{
	Shot* temp = NULL;
		index = first;
		while (index)
		{
			if(!index->DrawShot(hwnd, cxClient, cyClient))
				temp = index;
			index = index->next;
			if(temp)
				RemoveShot(temp);
			temp = NULL;
		}
	return 0;
}

int Shots::HitTest(HWND hwnd, Rocks& GameRocks, int cyClient, int &countHits)
{
	if (!first)
		return 0;
	int temp = 0;
	Shot* tempS;
	int points = 0;
	index = first;
	while(index)
	{
		temp = GameRocks.HitRocks(hwnd, index->xPos, (index->xPos + 8), index->yPos,  cyClient);
		if (temp)
		{
			points += temp;
			tempS = index;
			RECT shotRect;
			shotRect.left = tempS->xPos;
			shotRect.right = tempS->xPos + 8;
			shotRect.top = tempS->yPos - 20;
			shotRect.bottom = tempS->yPos + 40;
			InvalidateRect(hwnd, &shotRect, TRUE);
			RemoveShot(tempS);
			temp = 0;
			countHits++;
			return points;
		}
		index = index->next;
	}
	return points;
}

bool Shots::RemoveShot(Shot* remove)
{
 	Shot* temp = first;

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

		temp->next = remove->next;
		delete remove;
	}
	SetCurrent();
	return true;
}

void Shots::LockList ()
{
	WaitForSingleObject(ShotSema, INFINITE);
}

void Shots::UnLockList ()
{
	ReleaseSemaphore(ShotSema, 1, NULL); 
}

void Shots::SetCurrent()
{
	if (first)
	{
		Shot* temp = first;
		while(temp->next)
		{
			temp = temp->next;
		}
		current = temp;
	}
}