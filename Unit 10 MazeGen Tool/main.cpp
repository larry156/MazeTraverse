/* Name: Larry Y.
 * Date: November 21, 2018
 * Desc: This program will create a maze that is meant to be used as input for the main program. */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>

using namespace std;

int main()
{
	srand(time(NULL));

	// Input basic properties
	const int DEF_WIDTH = 50, DEF_HEIGHT = 40;
	int width = 0, height = 0, startX = 1, startY = 1;
	string tempNum = "", fileName = "";
	bool startValid = false;
    cout << "This tool will allow you to create a maze for the \"Searching for a Solution\" assignment." << endl;

    cout << "Please specify the width (Max. 200): ";
    getline(cin, tempNum);
    if (tempNum != "")
	{
		width = stoi(tempNum);
	}
	else
	{
		cout << "Width set to default value of " << DEF_WIDTH << "." << endl;
		width = DEF_WIDTH;
	}
    if (width > 200)
	{
		cout << "Input exceeded maximum boundaries. Width set to 200." << endl;
		width = 200;
	}

    cout << "Please specify the height (Max. 100): ";
    getline(cin, tempNum);
    if (tempNum != "")
	{
		height = stoi(tempNum);
	}
	else
	{
		cout << "Height set to default value of " << DEF_HEIGHT << "." << endl;
		height = DEF_HEIGHT;
	}
    if (height > 100)
	{
		cout << "Input exceeded maximum boundaries. Height set to 100." << endl;
		height = 100;
	}

    cout << "Would you like to specify a starting location? (Yes/No) ";
    getline(cin, tempNum);
    if (tempNum == "yes" || tempNum == "Yes")
    {
        while (!startValid)
        {
            cout << "Please enter the starting location (X coordinate): ";
            getline(cin, tempNum);
            startX = stoi(tempNum);
            cout << "Please enter the starting location (Y coordinate): ";
            getline(cin, tempNum);
            startY = stoi(tempNum);
            if (startX != 0 && startX != width-1 && startY != 0 && startY != height-1)
                startValid = true;
            else
            {
                cout << "Start location cannot be on the border." << endl;
            }
        }
    }

    cout << "Please name the output file." << endl << "Please note that any existing file with the same name will be overwritten" << endl;
    cout << "Enter file name: ";
    getline(cin, fileName);
    if (fileName == "")
	{
		fileName = "tooLazyToTypeAName";
	}

    // Create the maze
    string theMaze[width][height];
    int goalX = startX, goalY = startY;
    theMaze[startX][startY] = "B";
    while (goalX == startX && goalY == startY)
    {
        goalX = rand() % (width-1) + 1;
        goalY = rand() % (height-1) + 1;
    }
    theMaze[goalX][goalY] = "$";
    for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Create the outer border
			if (x == 0 || y == 0 || x == width-1 || y == height-1)
				theMaze[x][y] = "X";
			else if (theMaze[x][y] != "$" && theMaze[x][y] != "B") // Don't replace the goal
			{
				int spaceRoll = rand() % 3; // 67% chance of being empty
				if (spaceRoll < 2 || (x == 1 && y == 1)) // (1,1) should always be empty.
					theMaze[x][y] = " ";
				else
					theMaze[x][y] = "X";
			}
		}
	}
	// Spit it out to the file
	stringstream nameStream;
	nameStream << fileName << ".txt";
	ofstream outFile(nameStream.str());
	// First output width and height, then the actual maze
	outFile << width << "\n" << height << "\n";
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			outFile << theMaze[x][y];
		}
		outFile << "\n";
	}
	cout << "Maze of width " << width << " and height " << height << " created successfully." << endl;

    return 0;
}
