#include <fstream>
#include <iostream>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <cstdlib>
#include <time.h>
#include <windows.h>

#include "Node.h"

using namespace std;

/* This function will search through a 2D array of Nodes, checking if it can reach a Node whose value is goal, without
 * going through any blockers. It keeps track of which Nodes have been visited, as well as the path it takes.
 * It will search through Nodes clockwise, starting with the Node to the right of the current one. */
bool DepthSearch(Node*** searchGrid, stack<Node*> &path, Node* curNode, string goal, string blocker, int width, int height)
{
	//cout << "working..." << endl;
	int curX = curNode->getX(), curY = curNode->getY();
	curNode->visit();
	if (searchGrid[curX][curY]->getValue() == goal)
	{
		path.push(curNode);
		return true;
	}
	else if (searchGrid[curX][curY]->getValue() == blocker)
		return false;
	else if (curX != width-1 && !searchGrid[curX+1][curY]->isVisited() && DepthSearch(searchGrid, path, searchGrid[curX+1][curY], goal, blocker, width, height))
	{
		//cout << "(" << curX << ", " << curY << ")" << endl;
		path.push(curNode);
		return true;
	}
	else if (curY != height-1 && !searchGrid[curX][curY+1]->isVisited() && DepthSearch(searchGrid, path, searchGrid[curX][curY+1], goal, blocker, width, height))
	{
		//cout << "(" << curX << ", " << curY << ")" << endl;
		path.push(curNode);
		return true;
	}
	else if (curX != 0 && !searchGrid[curX-1][curY]->isVisited() && DepthSearch(searchGrid, path, searchGrid[curX-1][curY], goal, blocker, width, height))
	{
		//cout << "(" << curX << ", " << curY << ")" << endl;
		path.push(curNode);
		return true;
	}
	else if (curY != 0 && !searchGrid[curX][curY-1]->isVisited() && DepthSearch(searchGrid, path, searchGrid[curX][curY-1], goal, blocker, width, height))
	{
		//cout << "(" << curX << ", " << curY << ")" << endl;
		path.push(curNode);
		return true;
	}
	else
		return false;
}

bool BreadthSearch(Node*** searchGrid, stack<Node*> &path, Node* startNode, string goal, string blocker, string marker, int width, int height)
{
	map <Node*, Node*> cameFrom;
	cameFrom[startNode] = startNode;
	int x = startNode->getX(), y = startNode->getY();
	queue<Node*> nodes;
	nodes.push(startNode);
	searchGrid[x][y]->visit();
	while (!nodes.empty())
	{
	    x = nodes.front()->getX();
	    y = nodes.front()->getY();
		nodes.pop();
		if (searchGrid[x][y]->getValue() == goal)
		{
			Node* curNode = searchGrid[x][y];
			int numNodes = 0;
			path.push(curNode);
			while (curNode != startNode)
			{
				curNode = cameFrom[curNode];
				if (curNode->getValue() != goal && curNode != startNode)
                    curNode->markPath(marker);
				path.push(curNode);
				numNodes++;
				//cout << curNode->getPosition() << " ";
			}
			//cout << numNodes << endl;
			return true;
		}
		if (x < width-1 && searchGrid[x+1][y]->getValue() != blocker && !searchGrid[x+1][y]->isVisited())
		{
		    nodes.push(searchGrid[x+1][y]);
		    cameFrom[searchGrid[x+1][y]] = searchGrid[x][y];
		    searchGrid[x+1][y]->visit();
		}
		if (y < height-1 && searchGrid[x][y+1]->getValue() != blocker && !searchGrid[x][y+1]->isVisited())
		{
		    nodes.push(searchGrid[x][y+1]);
		    cameFrom[searchGrid[x][y+1]] = searchGrid[x][y];
		    searchGrid[x][y+1]->visit();
		}
		if (x > 0 && searchGrid[x-1][y]->getValue() != blocker && !searchGrid[x-1][y]->isVisited())
		{
		    nodes.push(searchGrid[x-1][y]);
		    cameFrom[searchGrid[x-1][y]] = searchGrid[x][y];
		    searchGrid[x-1][y]->visit();
		}
		if (y > 0 && searchGrid[x][y-1]->getValue() != blocker && !searchGrid[x][y-1]->isVisited())
		{
		    nodes.push(searchGrid[x][y-1]);
		    cameFrom[searchGrid[x][y-1]] = searchGrid[x][y];
		    searchGrid[x][y-1]->visit();
		}
	}
	return false;
}

int main()
{
	// Set up colours, and a key/legend
	HANDLE consHandle = GetStdHandle(STD_OUTPUT_HANDLE); // This is used to colour the console text.
    const int C_GRAY = 8, C_GREEN = 2, C_TURQUOISE = 3, C_YELLOW = 14, C_WHITE = 15; // Colours
    const string VAL_WALL = "X", VAL_GOAL = "$", VAL_PATH = "O", VAL_START = "B"; // Everything else is assumed to be empty.
	SetConsoleTextAttribute(consHandle, C_WHITE);

	// Get name of file and explain symbols.
	// Please do not abuse the fact that this doesn't check whether or not a file exists.
	string fileName;
	cout << "What is name of the maze?" << endl << "Enter file name: ";
	getline(cin, fileName);

	// Width should be on the first line, followed by height, and finally the maze.
	ifstream mazeFile(fileName + ".txt");
	string tempInput;

	// Get width and height
	getline(mazeFile, tempInput);
	if (tempInput == "")
	{
		cout << "Maze file is invalid. Exiting..." << endl;
		return 1;
	}
	int width = stoi(tempInput);
	getline(mazeFile, tempInput);
	// It is important to note that
	if (tempInput == "")
	{
		cout << "Maze file is invalid. Exiting..." << endl;
		return 1;
	}
	int height = stoi(tempInput);

	// Get the maze and store it in a 2D array.
	cout << "Moist von Lipwig will be exploring the maze below:" << endl;
	// First set up the array.
	Node ***theGrid = new Node**[width];
	Node *startNode;
	for (int x = 0; x < width; x++)
		theGrid[x] = new Node*[height];
	// Then, read in the array.
	for (int y = 0; y < height; y++)
	{
		getline(mazeFile, tempInput); // This sets tempInput to all maze cells on the current line.
		for (int x = 0; x < width; x++)
		{
			string curVal = tempInput.substr(x,1); // The value of the current cell.
			theGrid[x][y] = new Node(x, y, curVal);
			// Colour the text differently depending on the object at each node.
			if (theGrid[x][y]->getValue() == VAL_WALL)
				SetConsoleTextAttribute(consHandle, C_TURQUOISE);
			else if (theGrid[x][y]->getValue() == VAL_GOAL)
				SetConsoleTextAttribute(consHandle, C_YELLOW);
            else if (theGrid[x][y]->getValue() == VAL_START)
            {
                startNode = theGrid[x][y];
                SetConsoleTextAttribute(consHandle, C_WHITE);
            }
			cout << theGrid[x][y]->getValue();
		}
		cout << endl;
		// Reset the colours at the end of each line.
		SetConsoleTextAttribute(consHandle, C_WHITE);
	}
	cout << endl;

	// See if the goal is reachable and output a path.
	stack<Node*> pathOfMoist; // The path Moist should take on his journey through the maze, if the goal is reachable.
	if (BreadthSearch(theGrid, pathOfMoist, startNode, VAL_GOAL, VAL_WALL, VAL_PATH, width, height))
	{
		cout << "A path to the treasure has been revealed:" << endl;
		//cout << pathOfMoist.size() << endl;

		// Start node is turquoise, while end node is yellow. Everything else is white.
		SetConsoleTextAttribute(consHandle, C_TURQUOISE);
		while (!pathOfMoist.empty())
		{
			if (pathOfMoist.size() == 1)
				SetConsoleTextAttribute(consHandle, C_YELLOW);
			cout << pathOfMoist.top()->getPosition() << endl;
			pathOfMoist.pop();
			SetConsoleTextAttribute(consHandle, C_WHITE);
		}

		string outputVisual;
		cout << endl << "Would you like to see a visual representation of the path? (Yes/No) ";
		getline(cin, outputVisual);
		cout << endl;
		if (outputVisual == "yes" || outputVisual == "Yes")
        {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    if (theGrid[x][y]->getValue() == VAL_WALL)
                        SetConsoleTextAttribute(consHandle, C_TURQUOISE);
                    else if (theGrid[x][y]->getValue() == VAL_GOAL)
                        SetConsoleTextAttribute(consHandle, C_YELLOW);
                    else if (theGrid[x][y]->getValue() == VAL_PATH)
                        SetConsoleTextAttribute(consHandle, C_GREEN);
                    else if (theGrid[x][y]->getValue() == VAL_START)
                        SetConsoleTextAttribute(consHandle, C_WHITE);
                    cout << theGrid[x][y]->getValue();
                }
                cout << endl;
                SetConsoleTextAttribute(consHandle, C_WHITE);
            }
        }
	}
	else
	{
		SetConsoleTextAttribute(consHandle, C_GRAY);
		cout << "The treasure in this labyrinth will be forever undisturbed..." << endl;
	}

	// Clean stuff up
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			delete theGrid[x][y];
		}
		delete theGrid[x];
	}
	delete theGrid;

	SetConsoleTextAttribute(consHandle, C_WHITE);
    return 0;
}
