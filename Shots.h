// Shots.h: interface for the Shots class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SHOTS_H
#define SHOTS_H

#include "Shot.h"
#include "Rocks.h"
#include <windows.h>

class Shots
{
public:
	Shots();
	virtual ~Shots();

	/* Creates a new Shot and adds it to the list
	*	Arguments:
	*		hwnd: handle to the parent (main) window
	*		cyClient: Total Client height
	*		xShot: Shot's xposition, based on the ship's position when it is fired
	*		bShot: Bitmap of the shot image
    */
	int CreateShot (HWND, int cyClient, int xShot, HBITMAP bShot);

	/* Goes through each shot in the list and moves them up a certian distance
	*	Arguments:
	*		hwnd: handle to the parent (main) window
	*		cxClient: Total Client width
	*		cyClient: Total Client height
    */
	int MoveShots (HWND hwnd, int cxClient, int cyClient);

	/* Goes through each shot in the list and redraws them in their spot on the client area
	*	Arguments:
	*		hwnd: handle to the parent (main) window
	*		cxClient: Total Client width
	*		cyClient: Total Client height
    */
	int DrawShots (HWND hwnd, int cxClient, int cyClient);

	/* Goes through each shot and tests to see if they have hit a rock
	*	Arguments:
	*		hwnd: handle to the parent (main) window
	*		GameRocks: Instance of a linked list that stores all of the rocks that are in the game
	*		cyClient: Total Client height
	*		countHits: counts the total number of rocks that have been hit for statistics
    */
	int HitTest (HWND hwnd, Rocks& GameRocks, int cyClient, int &countHits);

	//Removes the shot being pointed to by remove from the list
	bool RemoveShot (Shot* remove);
	
	//locks the list when it is being used so that it cannot be accessed twice at the same time
	void LockList();
	//frees the list up for alterations again
	void UnLockList();

	HANDLE ShotSema;	//semaphore for locking an unlocking list

private:
	void SetCurrent ();		//sets the current pointer to the end of the list

	//pointers to objects in the linked list
	Shot* first;
	Shot* current;
	Shot* index;

};

#endif