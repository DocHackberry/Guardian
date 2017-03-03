// Shot.h: interface for the Shot class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SHOT_H
#define SHOT_H

#include <windows.h>
#include "Globals.h"


class Shot  
{
public:
	//constructors
	Shot();
	Shot(int cyClient, int xShot, HBITMAP bShot);

	//destructors
	virtual ~Shot();

	/* Draws a shot
	*	Arguments:
	*		hwnd: handle to the main program window
	*		cxClient: Total client area width
	*		cyClient: Total client area height
	*/
	int DrawShot (HWND hwnd, int cxClient, int cyClient);

	//pointer to the next shot in the list
	Shot* next;
	//right x position of the shot
	int xPos;
	//top y position of the shot
	int yPos;
	//bitmap of the shot
	HBITMAP hShot;


};

#endif

