// Rocks.h: interface for the Rocks class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ROCKS_H
#define ROCKS_H


#include "Rock.h"
#include <windows.h>

class Rocks  
{
public:
	Rocks();
	virtual ~Rocks();

	/* Creates a new Rock and adds it to the list
	*	Arguments:
	*		plevel: current level of play
	*		hwnd: handle to the parent (main) window
	*		cxClient: Total Client width
    */
	int CreateRock(int plevel, HWND hwnd, int cxClient);

	/* Goes through each rock in the list and redraws them
	*	Arguments:
	*		hwnd: handle to the parent (main) window
	*		cxClient: Total Client width
	*		cyClient: Total Client height
    */
	int DrawRocks(HWND hwnd, int cxClient, int cyClient);

	/* Goes through each rock in the list and redraws them in a new position when the move
	*	Arguments:
	*		hwnd: handle to the parent (main) window
	*		countLost:  This is for statistics, is counts each time a rock is missed, and hits the bottom
	*		cxClient: Total Client width
	*		cyClient: Total Client height
    */
	int MoveRocks(HWND hwnd, int &countLost, int cxClient, int cyClient);

	/* Goes through each rock in the list and tests them with a shot's coordinates to see if it was hit.  The function is called for each shot each time they move.
	*	Arguments:
	*		hwnd: handle to the parent (main) window
	*		low: the left-most coordinate of the shot tested
	*		shot: the right-most coordinate of the shot being tested
	*		height: height of the shot
	*		cyClient: Total Client height
    */
	int HitRocks (HWND hwnd, int low, int high, int height, int cyClient);

	/* If a large rock is hit, this splits it into two smaller rocks then removes it from the list
	*	Arguments:
	*		remove: pointer to the rock that needs to be split and removed from the list
    */
	bool SplitRock (Rock* remove);

	/* If a rock is hit, this  removes it from the list
	*	Arguments:
	*		remove: pointer to the rock that needs to be removed from the list
    */
	bool RemoveRock (Rock* remove);
	/* This function displays a little explosion animation when a rock is hit
	*	Attributes:
	*		hwnd: handle to the parent (main) window
	*		x: left position to start the animation
	*		y: top position to start the animation
	*/
	void Explode (HWND hwnd, int x, int y);

	/* Goes through each rock in the list and tests them to see if they have hit the player's ship
	*	Arguments:
	*		hwnd: handle to the parent (main) window
	*		shipx: ship's x position(left)
	*		cyClient: Total Client height
    */
	bool HitShip (HWND hwnd, int shipx, int cyClient);

	//locks the list when it is being used so that it cannot be accessed twice at the same time
	void LockList();
	//frees the list up for alterations again
	void UnLockList();

	int total;			//total number of rocks
	int speed;			//speed of all rocks

	//Bitmaps for each type of rock
	HBITMAP lRock;		
	HBITMAP sRock;
	HBITMAP Comet;
	HBITMAP Alien;
	HBITMAP Exp1;
	HBITMAP Exp2;
	HBITMAP Exp3;

	HANDLE RockSema;	//semaphore for locking an unlocking list

private:
	void SetCurrent ();		//sets the current pointer to the end of the list

	//pointers to objects in the linked list
	Rock* first;
	Rock* current;
	Rock* index;

};

#endif
