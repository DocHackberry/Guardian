// Player.h: interface for the Player class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PLAYER_H 
#define PLAYER_H 


#include "Rocks.h"
#include <windows.h>
#include "HighScores.h"

class Player  
{
public:
	Player();
	virtual ~Player();

	/* Draw the player's ship on the screen:
	*	Variables:
	*		hwnd: Handle to the parent window (the main game's window).
	*		hShip: The ship's bitmap that has been loaded from a resource file when the window was created.
	*		move: The distance that the ship has moved from its original starting position.
	*		cyClient: Height of the total client area.
	*		cyClient: Width of the total client area.
	*		moved:  This is a true/false values specifying weather or not the ship has moved.  If it has moved, it must erase its old image, if not, it can just be redrawn
	*/
	int DrawShip (HWND hwnd, HBITMAP hShip, int move, int cyClient, int cxClient, bool moved);

	/* Add points to the player's score
	*	Variables:
	*		hwnd: Handle to the parent window (the main game's window).
	*		GameRocks: An instance of a class that contains the linked list holding all of the current rocks.
	*		x:  The amount to add to the player's score.  A negative value will subtract points.
	*		
	*/
	int AddScore (HWND hwnd, Rocks &GameRocks, int x);

	/* This is called when the ship is hit.  It runs a small animation with GDI functions that blows the ship up
	*	Variables:
	*		hwnd: Handle to the parent window (the main game's window).
	*		hInst: This is an instance handle to the main window.  This is needed when loading the bitmaps from the resource file and displaying them to the screen.
	*		move: The ship's position.  X-postion relative to the center of the scree (negative is left, positive is right)
	*/
	int ShipHit (HWND hwnd, HINSTANCE hInst, int move);

	//Returns the player's score
	int GetScore();
	//Returns the current level
	int GetLevel();
	//Returns the current number of lives the player has remaining
	int GetLives();
	//Sets the number of lives the player has to num.  This way, I could easily change the amount a player starts with.
	void SetLives(int num);
	//Sets the current level of play.  This is incremented as the game goes on (every 500 points)
	void SetLevel(int num);
	//Gets the ship's xposition -- This is used for determining the ship's position to see if a rock has hit it.
	int Getx();

	char* name;					//player's name
	bool pause;					//keeps track of weather or not the game is paused
	HighScores	hScores;		//instance of a class that controls the high scoring information

private:
	int			Score;			//Player's score
	int			lives;			//Number of lives the player has reamaining
	int			level;			//current level of play
	int			shipx;			//ship's x position
	int			shipy;			//ship's y position

};

#endif