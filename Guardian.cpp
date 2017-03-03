
/****************************************************************
CST 238 Final Project -- Guardian

Drew Bryan
Started on Oct 21st, 2001



********************************************************************/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "Rocks.h"
#include "Shots.h"
#include "Player.h"
#include "resource.h"
#include "commctrl.h"
#include "Globals.h"
#include "htmlhelp.h"

//Globals
Rocks			GameRocks;			//Instance of a data structure that holds all of the current falling rocks, and other objects (aliens and comets)
Shots			PlayerShots;		//Instance of a data structure that holds all of the player's shots
Player			player;				//Instance of the player class
HINSTANCE		hInst;				//Instance handle for the program - assigned in WinMain, used in the WinProc
int				move;				//how far from the center of the screen the ship has moved
int				cxClient;
int				cyClient;				
int				shotx;				//each shot's X position
int				shoty;				//each shot's Y position
HBITMAP			hShot;
HBITMAP			hShip;
HBITMAP			hCargo;
bool			wait;				//regulates the ship's shooting speed
bool			waitl;				//regulates the ship's left movement - skips the keydown message if the key is already down
bool			waitr;				//regulates the ship's right movement - skips the keydown message if the key is already down
HMENU			hMenu;				//Menu handle for app
int				countLost;			//Count number of lost rocks
int				countShots;			//Count the number of shots taken
int				countHits;			//Count the number of rocks hit
long			Accuracy;
int				newSpeed;							//This controls the rate at which the new rocks are created
int				rockSpeed;

//MCI data structures that are used to open and play the sound file used for the game's mausic
	MCI_OPEN_PARMS		mciOpen;
    MCI_PLAY_PARMS		mciPlay;
	MCI_GENERIC_PARMS	mciGeneric;
	WORD				wDeviceID;							//Device ID for the sound driver that plays the game's music
	

//Functions
int					PlayMusic			(HWND hwnd, MCI_PLAY_PARMS &mciPlay, MCI_OPEN_PARMS &mciOpen, WORD &wDeviceID);
int					Fire				(HWND hwnd, HBITMAP hShip, HBITMAP hShot, int move, int cyClient, int cxClient);
void				NewGame				(HWND hwnd);
void				EndGame				(HWND hwnd);
LRESULT	CALLBACK	WndProc				(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL	CALLBACK	DlgProc				(HWND hNDlg, UINT message, UINT wParam, LONG lParam);
BOOL	CALLBACK	NameDlgProc			(HWND hNDlg, UINT message, UINT wParam, LONG lParam);
VOID	CALLBACK	TimerProc			(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
BOOL	CALLBACK	ScoreDlgProc		(HWND hSDlg, UINT message, UINT wParam, LONG lParam);


/*********************************************************************************
*		This in the entry point for the program.  Creates the window, and registers it.
*
*
*
*********************************************************************************/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR lpszCmdParam, int nCmdShow)
{
	static char szAppName[] = "Guardian";
	HWND			hwnd;
	MSG 			msg;
	WNDCLASSEX		wndclass;
	HACCEL			hAccel;
	
		wndclass.cbSize			= sizeof(wndclass);
		wndclass.style			= CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc	= WndProc;
		wndclass.cbClsExtra		= 0;
		wndclass.cbWndExtra		= 0;
		wndclass.hInstance		= hInstance;
		wndclass.hIcon			= LoadIcon (NULL, MAKEINTRESOURCE(IDI_ICON1));
		wndclass.hCursor		= LoadCursor (NULL, IDC_ARROW);
		wndclass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+3);
		wndclass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
		wndclass.lpszClassName	= szAppName;
		wndclass.hIconSm		= LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1));
		  
		RegisterClassEx (&wndclass);
		hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
		hInst = hInstance;

		hwnd = CreateWindow (szAppName,		// window class name
			"Guardian - Press P to pause",						// window caption
			WS_OVERLAPPEDWINDOW,			// window style
			CW_USEDEFAULT,					// initial x position
			CW_USEDEFAULT,					// initial y position
			790,							// initial x size -- fits on an 800x600 monitor
			570,							// initial y size -- fits on an 800x600 monitor
			NULL, 							// parent window handle
			NULL, 							// window menu handle
			hInstance,						// program instance handle
			NULL);							// creation parameters
		  
		hMenu = GetMenu(hwnd);
		ShowWindow (hwnd, nCmdShow);
		UpdateWindow (hwnd);
		  
		while (GetMessage (&msg, NULL, 0, 0))
		{
			if(!TranslateAccelerator(hwnd, hAccel, &msg))	//Pulls accelerators out of the message pump
			{
				TranslateMessage (&msg);
				DispatchMessage (&msg);
			}
		}
		return msg.wParam;
}

/******************************************************************************
*		This is the windows procedure for this app.  It takes and handles all
*			messages to the window.
*
******************************************************************************/

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, 
						  WPARAM wParam, LPARAM lParam)
{
	HDC					hdc, Image;
	PAINTSTRUCT			ps;
	TEXTMETRIC			tm;
	RECT				rect, statusRect;
	TCHAR				sBuffer  [50];						//This buffer is for the score that appears in the status bar
	TCHAR				leBuffer [50];						//This buffer is for level that appears in the status bar
	TCHAR				liBuffer [50];						//This buffer is for player's lives that are left (displayed in the status bar)
	TCHAR				aBuffer  [50];
	char*				nBuffer;
	static HBITMAP		lRock, sRock, Comet, Alien;	//handles to bitmaps for several on the graphics that are used
	static HWND			statusBar;
	static int			parts [5];							//Array of ints, this contains the position of each element in the status bar

	try
	{
		switch (message)
		{
		case WM_CREATE:

			Accuracy = 0;
			countShots = 0;
			countHits = 0;
			countLost = 0;
			wait = false;				//ship is not shooting
			waitl = false;				//ship is not moving left
			waitr = false;				//ship is not moving right

			//PlaySound("Guardian Music.wav", NULL, SND_FILENAME | SND_ASYNC);
			hdc = GetDC(hwnd);

			//Load bitmaps as resources
				//player items
				hShip = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIP));
				hShot = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHOT2));
				hCargo = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CARGO));
				//explosions
				GameRocks.Exp1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EXPLODE1));
				GameRocks.Exp2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EXPLODE2));
				GameRocks.Exp3 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EXPLODE3));
				//enemies
				GameRocks.lRock = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BIG));
				GameRocks.sRock = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SMALL));
				GameRocks.Comet = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_COMET));
				GameRocks.Alien = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ALIENS));
			//End Bitmap Stuff

			//Status Bar stuff
				//Set the sizes of the sections
				parts[0] = 150;
				parts[1] = 300;
				parts[2] = 450;
				parts[3] = 600;
				parts[4] = 790;
				//Create it
				statusBar = CreateStatusWindow(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|CCS_BOTTOM, NULL, hwnd, STATUS_ID);
				//Set number of sections with the sizes already given:  Level, score, and lives.
				SendMessage(statusBar, SB_SETPARTS, (WPARAM)5, (LPARAM)parts);
				//Get a RECT for the status bar area
				GetClientRect (statusBar, &statusRect);
			//End Status Bar stuff

			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			GetTextMetrics(hdc, &tm);

			GetClientRect(hwnd, (LPRECT) &rect);				//get Client area
			cxClient = rect.right - rect.left;					//get Client area width
			cyClient = rect.bottom - rect.top;					//get Client area height

			GameRocks.CreateRock(player.GetLevel(), hwnd, cxClient);
			
			player.name = new char [44];
			player.pause = true;
			DialogBox (hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, NameDlgProc);
			player.pause = false;

			nBuffer = new char [50];
			strcpy (nBuffer, "Name: ");
			strcat(nBuffer, player.name);
			SendMessage (statusBar, SB_SETTEXT, (WPARAM) 3, (LPARAM) nBuffer);

			rockSpeed = 20;
			newSpeed = 3000;			//Sets rocks to be created evey 3 seconds
			SetTimer (hwnd, NEW_ROCK, newSpeed, TimerProc);					//starts timer to create new rocks
			SetTimer (hwnd, MOVE_ROCK, rockSpeed, TimerProc);					//starts timer for moving rocks.  Each time this timer goes off, it moves each rock on the screen down on level.
			SetTimer (hwnd, MOVE_SHOTS, 10, TimerProc);

			PlayMusic (hwnd, mciPlay, mciOpen, wDeviceID);

			ReleaseDC(hwnd, hdc);
			rect.top = 3 * tm.tmHeight / 2 ;
			return 0;
			
		case WM_PAINT:
				hdc = BeginPaint (hwnd, &ps);

			  //Background stuff
				Image = CreateCompatibleDC (hdc) ;
			
			  //Set Cargo Ship place
				SelectObject (Image, hCargo) ;
				BitBlt(hdc, (cxClient / 2) - 200, cyClient - 70, 400, 50, Image, 0, 0, SRCCOPY) ;
				DeleteDC (Image) ;

			  //Set Status Bar items to update	
				wsprintf(sBuffer, TEXT("Score:%5d"), player.GetScore());
				SendMessage (statusBar, SB_SETTEXT, (WPARAM) 0, (LPARAM) sBuffer);
				wsprintf(leBuffer, TEXT("Level:%5d"), player.GetLevel());
				SendMessage (statusBar, SB_SETTEXT, (WPARAM) 1, (LPARAM) leBuffer);
				wsprintf(liBuffer, TEXT("Lives:%5d"), player.GetLives());
				SendMessage (statusBar, SB_SETTEXT, (WPARAM) 2, (LPARAM) liBuffer);
				if(countShots > 0)
					Accuracy = (countHits * 100) / countShots;
				wsprintf(aBuffer, "Accuracy: %2lu%% (%d Shots/%d Hits)", Accuracy, countShots, countHits);
				SendMessage (statusBar, SB_SETTEXT, (WPARAM) 4, (LPARAM) aBuffer);

				GetWindowRect(hwnd, &rect);
				
				SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT));	
				//GameRocks.DrawRocks(hwnd, cxClient, cyClient);
				player.DrawShip(hwnd, hShip, move, cyClient, cxClient, false);

				EndPaint (hwnd, &ps);
				return 0;
			break;

		case TIMER_UPDATE:						//This message changes the speed at which rocks are created
			KillTimer(hwnd, NEW_ROCK);						//Kills Rock Creation Timer
			if ((newSpeed - 250) > 300)
				newSpeed -= 250;							//Reduces Rock Speed Variable
			SetTimer(hwnd, NEW_ROCK, newSpeed, TimerProc);	//Resets timer with new (lower) value
			if((player.GetLevel() % 3) == 0)
			{
				KillTimer(hwnd, MOVE_ROCK);						//Kills Rock Creation Timer
					rockSpeed += 5;							//Reduces Rock Speed Variable
				SetTimer(hwnd, MOVE_ROCK, rockSpeed, TimerProc);	//Resets timer with new (lower) value	
			}
			return 0;

		case WM_LBUTTONDOWN:				//left button was pressed - not needed, but may be used later
		case WM_RBUTTONDOWN:				//right button was pressed - not needed, but may be used later
			return 0;

		case WM_KEYDOWN:
			switch (wParam)
			{

				case VK_LEFT:
					if (!player.pause)						//Only move if game is not paused
					{
						if (!waitl)							//Don't do this as long as the key is depressed (waitl = false)
						{
							KillTimer (hwnd, MOVE_RIGHT);				//Stops Right Movement
							waitr = false;
							SetTimer (hwnd, MOVE_LEFT, 35, TimerProc);	//Starts Left Movement Timer
							waitl = true;
						}
					}
					return 0;

				case VK_RIGHT:
					if (!player.pause)						//Only more if game is not paused
					{
						if (!waitr) 						//Don't do this as long as the key is depressed (waitr = false)
						{
							KillTimer (hwnd, MOVE_LEFT);				//stops left movement with the right button is pressed
							waitl = false;
							SetTimer (hwnd, MOVE_RIGHT, 35, TimerProc);	//starts right movement on timer
							waitr = true;
						}
					}
					return 0;

				case VK_SPACE:							//Creates a shot, and sets a new one to be created in 4/10 of a second while the buton is depressed
					if (!player.pause)
					{
						if(!wait)						//Doesn't let this be executed if it is already running
						{
							SetTimer(hwnd, SHOOT, 400, TimerProc);					//shoots a new shot every 4/10 of a second as long as the key is depressed
							SetTimer(hwnd, WAIT_FIRE, 300, TimerProc);				//wait between shots to space them out a little
							PlaySound("Shot.wav", NULL, SND_FILENAME | SND_ASYNC);	//Plays sound for shot
							countShots++;
							Fire (hwnd, hShip, hShot, move, cyClient, cxClient);	//Starts one shot since it will be 4/10 of a second before next shot
							wait = true;
						}
					}
					return 0;
			}
			break;

		case WM_KEYUP:					//ends each timer that has an is executing a player function
			switch (wParam)
			{
				case VK_LEFT:
					if (waitl)
					{
						KillTimer (hwnd, MOVE_LEFT);
						waitl = false;
					}
					break;

				case VK_RIGHT:
					if (waitr)
					{
						KillTimer (hwnd, MOVE_RIGHT);
						waitr = false;
					}
					break;
					
				case VK_SPACE:
					KillTimer(hwnd, SHOOT);
					break;

			return 0;
			}
			
		case MM_MCINOTIFY:
			switch (wParam)
			{
				case MCI_NOTIFY_SUCCESSFUL:				//repeat music when it ends
				{
					mciGeneric.dwCallback = 0;
					mciSendCommand (wDeviceID, MCI_STOP, MCI_WAIT, (DWORD) (LPMCI_GENERIC_PARMS) &mciGeneric) ;
					mciSendCommand (wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD) (LPMCI_GENERIC_PARMS) &mciGeneric) ;
					PlayMusic (hwnd, mciPlay, mciOpen, wDeviceID);
				}
			}
			return 0;

		case WM_COMMAND:					//This section handles any messages that are received from the menu
			switch (LOWORD (wParam))
			{
				case ID_FILE_PAUSEGAME:
					//Pause the game if it is not already paused
					if (!player.pause)
					{
						player.pause = true;
						CheckMenuItem (hMenu, ID_FILE_PAUSEGAME, MF_CHECKED);
					}
					//Unpause the game if it has previously been paused
					else
					{
						player.pause = false;
						CheckMenuItem (hMenu, ID_FILE_PAUSEGAME, MF_UNCHECKED);
					}
					return 0;

				case ID_FILE_HIGHSCORES:	//Show the list of highscores
					player.pause = true;
					DialogBoxParam( hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, ScoreDlgProc,(LPARAM) &player.hScores);
					player.pause = false;
					return 0;

				case ID_FILE_NEWGAME:			//reset al values to start a new game
					EndGame(hwnd);
					NewGame(hwnd);
					return 0;

				case ID_HELP_ABOUT:
					player.pause = true;
					DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, DlgProc, NULL);
					player.pause = false;
					return 0;

				case ID_HELP_HELP:
					player.pause = true;
					HtmlHelp(hwnd, "GuardianHelp.chm", HH_DISPLAY_TOPIC, 0);
					InvalidateRect(hwnd, NULL, TRUE);
					return 0;

				case ID_FILE_EXIT:
					SendMessage(hwnd, WM_DESTROY, NULL, NULL);
					return 0;
			}

		case WM_MOVE:
			//Get new client area RECT size, and invalidate the entire window
			rect.left	= LOWORD (lParam) ;
			rect.top	= HIWORD (lParam) ;
			InvalidateRect (hwnd, NULL, TRUE);
			return 0;

		case WM_SIZE:
			//Get new client area size, and invalidate the entire window
			cxClient	= LOWORD (lParam) ;
			cyClient	= HIWORD (lParam) ;
			InvalidateRect (hwnd, NULL, TRUE);
			SendMessage (statusBar, WM_SIZE, wParam, lParam);		//redraw status bar to fit nre size
			return 0 ;
		
		case WM_DESTROY:
			EndGame(hwnd);
			Sleep(400);
			PostQuitMessage (0);
			return 0;
		}
	}
	catch(...)
	{
		EndGame(hwnd);
		Sleep(400);
		PostQuitMessage (0);
	}

	return DefWindowProc (hwnd, message, wParam, lParam);
}


/*****************************************************************
*	Message handler for a Dialog procedure that tells the user about
*	 the game.
*
******************************************************************/
BOOL CALLBACK DlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDOK:
					EndDialog (hDlg, 0);
					return TRUE;
				case WM_DESTROY:
					EndDialog (hDlg, 0);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

/*****************************************************************
*	Message handler for a Dialog procedure that gets the player's
*	 name.  This is sent back to the parent window, and used in the
*	 status bar, and the high scores list
*
******************************************************************/

BOOL CALLBACK NameDlgProc (HWND hNDlg, UINT message, UINT wParam, LONG lParam)
{
	static HWND		hEdit;
	switch (message)
	{
		case WM_INITDIALOG:
			hEdit = GetDlgItem (hNDlg, IDC_EDIT1);
			SetFocus(hEdit);
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDOK:
					GetDlgItemText(hNDlg, IDC_EDIT1, player.name, 50);
					EndDialog (hNDlg, 0);
					return TRUE;
				case WM_DESTROY:
					GetDlgItemText(hNDlg, IDC_EDIT1, player.name, 50);
					EndDialog (hNDlg, 0);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

/*****************************************************************
*	Message handler for a Dialog procedure that displays all of the
*	 top scores that are stored in the high scores file.
*
******************************************************************/

BOOL CALLBACK ScoreDlgProc (HWND hSDlg, UINT message, UINT wParam, LONG lParam)
{
	static HWND		hStaticN, hStaticS;
	static TCHAR	Names[511], Scores[511];
	switch (message)
	{
		case WM_INITDIALOG:
			hStaticN = GetDlgItem (hSDlg, IDC_NAMES);
			hStaticS = GetDlgItem (hSDlg, IDC_SCORES);
			wsprintf(Names, "%hs\n\n%hs\n\n%hs\n\n%hs\n\n%hs", player.hScores.topNames[0], player.hScores.topNames[1], player.hScores.topNames[2], player.hScores.topNames[3], player.hScores.topNames[4]);
			wsprintf(Scores, "%d\n\n%d\n\n%d\n\n%d\n\n%d", player.hScores.topScores[0], player.hScores.topScores[1], player.hScores.topScores[2], player.hScores.topScores[3], player.hScores.topScores[4]);
			SetWindowText (hStaticN, Names);
			SetWindowText (hStaticS, Scores);
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDOK:
					EndDialog (hSDlg, 0);
					return TRUE;
				case WM_DESTROY:
					EndDialog (hSDlg, 0);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

/*********************************************************************************
*  This funtion will handle all of the timers that are created for this program.
*	They will all be sent to this function in order to keep them from getting stuck
*	behind other messages in the main WinProc function for this program.  Since the
*	timers are so important for the movement and gameplay, they must be a high priority.
*	They cannot wait for other message or the look of the movement can greatly decrease
*
*********************************************************************************/

VOID CALLBACK TimerProc (HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	static HDC	hdc, hdcShot;				//hanles to Device Contexts for the main window, and for the shot image
	hdc	= GetDC (hwnd);

	switch (iTimerID)
	{
		case MOVE_LEFT:
				if ((((cxClient / 2) - 39) + move) > SHIP_MOVE)		//keep movement within the constraints of the window
					move -= SHIP_MOVE;								//sets the ship's offset from the center to a greater negative number
				player.DrawShip(hwnd, hShip, move, cyClient, cxClient, true);	//redraws ship in its new position
			break;

		case MOVE_RIGHT:
				if ((((cxClient / 2) + 39) + move) < (cxClient - SHIP_MOVE))		//keep movement within the constraints of the window
					move += SHIP_MOVE;								//sets the ship's  offset from the center to a greater positie number
				player.DrawShip(hwnd, hShip, move, cyClient, cxClient, true);	//redraws the ship in its new position
			break;

		case WAIT_FIRE:
			wait = false;					//flips this flag so that the player can shoot again after a short delay
			KillTimer(hwnd, WAIT_FIRE);
			break;

		case SHOOT:
			PlaySound("Shot.wav", NULL, SND_FILENAME | SND_ASYNC);	//Shot's shound
			countShots++;
			Fire (hwnd, hShip, hShot, move, cyClient, cxClient);	//Creates a new shot
			break;

		case MOVE_SHOTS:	//Locks the linked list of shots, and moves them all a certain distance
			PlayerShots.LockList();
				PlayerShots.DrawShots(hwnd, cxClient, cyClient);
				GameRocks.LockList();
					player.AddScore(hwnd, GameRocks, PlayerShots.HitTest(hwnd, GameRocks, cyClient, countHits));		//checks during each movement to see if a shot has hit a rock.  If a rock is hit, it adds that rock's point value to the player's score.
				GameRocks.UnLockList();
			PlayerShots.UnLockList();
			break;

		case MOVE_ROCK:		//Locks the linked list of roks, and moves them all a certain distance
			if (!player.pause)
			{
				GameRocks.LockList();
				player.AddScore (hwnd, GameRocks, GameRocks.MoveRocks(hwnd, countLost, cxClient, cyClient));
				//make each rock test for hitting the ship
				if (GameRocks.HitShip(hwnd, player.Getx() + move, cyClient) || countLost >= 3)
				{
					player.ShipHit(hwnd, hInst, move);
					Sleep (80);
					move = 0;
					countLost = 0;
					GameRocks.~Rocks();						//Delete all of the rocks
					InvalidateRect(hwnd, NULL, TRUE);		//redraw all of the client area
						
					if(player.GetLives() <= 0)
					{
						player.pause = true;
						EndGame(hwnd);
						NewGame(hwnd);
					}
				}

				GameRocks.UnLockList();
			}
			break;

	 	case NEW_ROCK:		//Creates a new Rock in the linked list
			if (!player.pause)
			{
				GameRocks.LockList();
				GameRocks.CreateRock(player.GetLevel(), hwnd, cxClient);			//creates a new rock on a timer
				GameRocks.UnLockList();
			}
			break;
	}

	ReleaseDC(hwnd, hdc);
	//InvalidateRect (hwnd, NULL, FALSE);
}

/*********************************************************************************
*  This funtion will create a new shot in the shots data structure, and will give
*	the shots its inital position based on te position of the ship.
*
*********************************************************************************/

int Fire (HWND hwnd, HBITMAP hShip, HBITMAP hShot, int move, int cyClient, int cxClient)
{
	shotx = (cxClient / 2) + (move - 6);		//shot's original position
	shoty = (cyClient -  175);
	
	PlayerShots.LockList();				//Lock the list while inserting a new shot
	PlayerShots.CreateShot(hwnd, cyClient, shotx, hShot);
	PlayerShots.UnLockList();

	return 0;
}

/*********************************************************************************
*  This funtion is given a data structure for playig a sound.  It fills the structure
*	with all of the neccessary items, and then commenses playing the given sound file.
*
*********************************************************************************/

int PlayMusic (HWND hwnd, MCI_PLAY_PARMS &mciPlay, MCI_OPEN_PARMS &mciOpen, WORD &wDeviceID)
{
	
	TCHAR		music [] = TEXT("Guardian Music.wav");

	mciOpen.dwCallback       = 0 ;
	mciOpen.wDeviceID        = 0 ;
	mciOpen.lpstrDeviceType  = NULL ;
	mciOpen.lpstrElementName = music;
	mciOpen.lpstrAlias       = NULL ;
	mciSendCommand (0, MCI_OPEN, MCI_WAIT | MCI_OPEN_ELEMENT, (DWORD) (LPMCI_OPEN_PARMS) &mciOpen) ;
	wDeviceID = mciOpen.wDeviceID ;

	// Begin playing
               
	mciPlay.dwCallback = (DWORD) hwnd ;
	mciPlay.dwFrom     = 0 ;
	mciPlay.dwTo       = 0 ;
               
	mciSendCommand (wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD) (LPMCI_PLAY_PARMS) &mciPlay) ;

	return 0;
}

/*********************************************************************************
*  This funtion will decide if you are in the top score list or not.  If so, it will
*	have you add your name to the list, and save it to the file.
*
*********************************************************************************/

void NewGame (HWND hwnd)
{
	player.pause = true;					//Pause game so nothing can happen during this time
	player.AddScore(hwnd, GameRocks, -(player.GetScore()) );	//clear score
	GameRocks.~Rocks(); 					//Delete all of the rocks
	GameRocks.speed = 1;
	player.SetLives(3);
	player.SetLevel(1);
	move = 0;
	InvalidateRect(hwnd, NULL, TRUE);

	//Reset Timers
	rockSpeed = 20;
	newSpeed = 3000;			//Sets rocks to be created evey 3 seconds
	SetTimer (hwnd, NEW_ROCK, newSpeed, TimerProc);					//starts timer to create new rocks
	SetTimer (hwnd, MOVE_ROCK, rockSpeed, TimerProc);
	countHits = 0;
	countShots = 0;
	PlayMusic (hwnd, mciPlay, mciOpen, wDeviceID);
	GameRocks.LockList();
	GameRocks.CreateRock(player.GetLevel(), hwnd, cxClient);			//creates a new rock on a timer
	GameRocks.UnLockList();
	player.pause = false;
}

void EndGame (HWND hwnd)
{
	TCHAR			szBuffer [255];
	int temp;
	//Stop music, and restart it
		mciGeneric.dwCallback = 0;    
		mciSendCommand (wDeviceID, MCI_STOP, MCI_WAIT, (DWORD) (LPMCI_GENERIC_PARMS) &mciGeneric) ;  
		mciSendCommand (wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD) (LPMCI_GENERIC_PARMS) &mciGeneric) ;

	player.pause = true;
	KillTimer(hwnd, MOVE_LEFT);
	KillTimer(hwnd, MOVE_RIGHT);
	KillTimer(hwnd, SHOOT);
	InvalidateRect(hwnd, NULL, TRUE);		//redraw all of the client area
	KillTimer(hwnd, NEW_ROCK);
	KillTimer(hwnd, MOVE_ROCK);

	wsprintf(szBuffer, "Game Over\nYour Final Score Was: %5d\nAccuracy: %5d%%", player.GetScore(), Accuracy);
	MessageBox(hwnd, szBuffer, "Game Over", MB_OK);


	temp = player.hScores.isHigh(player.GetScore());
	if (temp)
	{
		player.hScores.AddHigh(player.GetScore(),player.name, temp);
		if (temp > 0)
			MessageBox(hwnd, "You have gotten a new high score!", "New High Score", MB_OK);
	}
	DialogBoxParam( hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, ScoreDlgProc,(LPARAM) &player.hScores);
	player.hScores.SaveScore(hwnd);				//Save it to a file when the dialog box it exited
}
