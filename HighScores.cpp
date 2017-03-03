// HighScores.cpp: implementation of the HighScores class.
//
//////////////////////////////////////////////////////////////////////

#include "HighScores.h"
#include <fstream.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 80


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HighScores::HighScores()
{
	for (int i = 0; i < 5; i++)
	{
		topNames[i] = new char[75];
	}
	Open();
}

HighScores::~HighScores()
{
	for (int i = 0; i < 5; i++)
	{
		delete [] topNames[i];
	}
}

int HighScores::Open ()
{
	char* token = new char [MAX_LENGTH];
	char* buffer = new char [MAX_LENGTH];
	ifstream infile;
	infile.open("scores.xml", ios::nocreate);		//opens file containing high scores
	if (infile.is_open())						 	//File is open
	{
		
		infile.getline(buffer, MAX_LENGTH, '\n');		//reads in first line
		token = strtok (buffer, "\t<>");					//tokenizes the first line
		if (!strcmp(token, "scorelist"))
		{
			for (int i = 0; i < 5; i++)
			{	
				infile.getline(buffer, MAX_LENGTH, '\n');		//reads in first line
				token = strtok(buffer, "\t<>");
				if (!strcmp(token, "info"))
				{
					infile.getline(buffer, MAX_LENGTH, '\n');
					token = strtok(buffer, "\t<>");
					if (!strcmp(token, "name"))
					{
						token = strtok(NULL, "\t<>");
						strcpy (topNames[i], token);
						token = strtok(NULL, "\t<>");
						//token should be /name
					}
					infile.getline(buffer, MAX_LENGTH, '\n');
					token = strtok(buffer, "\t<>");
					if (!strcmp(token, "score"))
					{
						token = strtok(NULL, "\t<>");
						topScores[i] = atoi(token);
						token = strtok(NULL, "\t<>");
						//token should be /score
					}
				infile.getline(buffer, MAX_LENGTH, '\n');		//reads in first line
				token = strtok(buffer, "\t<>");
				//token should be /info
				}
			}
		infile.getline(buffer, MAX_LENGTH, '\n');		//reads in first line
		token = strtok(buffer, "\t<>");
		//token should be /scorelist
		}
		infile.close();
	}
	else
		return 0;			//display message box that says file was not opened
	delete [] buffer;
	return 1;
}

int HighScores::isHigh (int score)
{
	for (int i = 0; i < 5; i++)
	{
		if (score > topScores[i])
			return i+1;
	}

	return 0;
}

void HighScores::AddHigh (int score, char* name, int place)
{
	int temp;
	temp = place - 1;
	if (temp < 4)
	{
		for (int i = 4; i > temp; i--)
		{
			topScores[i] = topScores[i-1];
			strcpy(topNames[i], topNames[i-1]);
		}
	}
	strcpy(topNames[temp], name);
	topScores[temp] = score;
}

void HighScores::SaveScore (HWND hwnd)
{
	ofstream outfile;
	outfile.open("scores.xml", ios::nocreate);		//opens file containing high scores
	if (outfile.is_open())
	{
		outfile << "<scorelist>\n";
		for (int i = 0; i < 5; i++)
		{
			outfile << "\t<info>\n";
			outfile << "\t\t<name>" << topNames[i] << "</name>\n";
			outfile << "\t\t<score>" << topScores[i] << "</score>\n";
			outfile << "\t</info>\n";
		}
		outfile << "</scorelist>\n";
	}
}
