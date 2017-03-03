// HighScores.h: interface for the HighScores class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <windows.h>

class HighScores  
{
public:
	HighScores();
	virtual ~HighScores();

	/*This function Opens scores.xml for reading, and stores the player names, and scores that are in that file
	*  in topName and topScores.  These variables are arrays that hold the five values.  There will never be more
	*  than five high scores in the file.*/
	int Open ();

	/*This function will take an input number called score.  This variable will contain the player's score
	*  The function will take that variable and check against all of the current scores that were stored from
	*  the scores.xml file.  If the score is in the high scores list, it will return the postion.  If not, it will
	*  return 0.*/
	int isHigh (int score);

	/*This function will take an input number called score as well as the name of the player, and the place
	*  the player's score belongs in the list - returned from isHigh().  Then, it places the new score in its
	*  proper position, and bumps all of the scores below it down one position (removing the last one).*/
	void AddHigh (int score, char* name, int place);
	
	/*This function will open scores.xml for writing.  Then, it will rewrite the xml file with new information.
	*  this function recreates the original structure of the XML file, but adds the new data.*/
	void SaveScore (HWND hwnd);

	char* topNames [5];
	int topScores [5];

};

#endif
