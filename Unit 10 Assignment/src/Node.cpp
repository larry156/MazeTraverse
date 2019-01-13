#include "Node.h"
#include <sstream>

using namespace std;

Node::Node(int xPos, int yPos, string value) : x(xPos), y(yPos), visited(false), value(value)
{
	stringstream posTemp;
	posTemp << "(" << x << ", " << y << ")";
	position = posTemp.str();
}

Node::~Node()
{

}

int Node::getX()
{
	return x;
}

int Node::getY()
{
	return y;
}

string Node::getValue()
{
	return value;
}

string Node::getPosition()
{
	return position;
}

bool Node::isVisited()
{
	return visited;
}

void Node::visit()
{
	visited = true;
}

void Node::markPath(string marker)
{
    value = marker;
}
