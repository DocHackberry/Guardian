// Rock.h: interface for the Rock class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ROCK_H
#define ROCK_H


#include <windows.h>


class Rock  
{
public:
	//constructors
	Rock(int plevel, int cxClient, HBITMAP Big, HBITMAP Small, HBITMAP Comet, HBITMAP Alien);
	Rock(int xPos, int yPos, int piece, HBITMAP Small);
	//destructor
	virtual ~Rock();

	/* Draws an object
	*	Arguments:
	*		hwnd: handle to the main program window
	*		move: the distance to move the objcet
	*		cxClient: Total client area width
	*		cyClient: Total client area height
	*/
	int DrawRock(HWND hwnd, int move, int cxClient, int cyClient);

	/*Tests to see if a shot has hit this rock (object)
	*	Arguments:
	*		low: the left-most side of the shot bitmap
	*		high: the right-most value of the shot bitmap
	*		shoty: the y-position of the shot (top)
	*		cyClient: Total client area height
	*/
	bool HitRock (int low, int high, int shoty, int cyClient);

	//Tests to see if a rock has hit the ship.  Shipx is the ship's left x coordinate.  It will calculate the right side.
	bool HitShip (int shipx);

	Rock* next;		//pointer to the next rock in the list
	int pointval;	//point value of the rock
	int width;		//width of the rock(object's) bitmap
	int height;		//height of the rock(object's) bitmap
	int x1;			//top coordinate
	int x2;			//bottom coordinate
	int y1;			//left coordinate
	int y2;			//right coordinate

	HBITMAP		Image;	//the bitmap that this object uses

private:
	int			rspeed;			//the speed at which it falls (stays constant, but separates each object).  When speed increases, it is the game speed which affects all objects.
	int			breakpoint;		//The y position where a large rock breaks
	int			half;			//Is this a left or right half after a break.  0 = neither 1 = left, 2 = right

};

#endif