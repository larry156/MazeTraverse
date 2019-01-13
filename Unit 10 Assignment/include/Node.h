#ifndef NODE_H
#define NODE_H

#include <string>

class Node
{
	public:
		Node(int xPos, int yPos, std::string value);
		virtual ~Node();
		int getX();
		int getY();
		std::string getValue();
		std::string getPosition();
		bool isVisited();
		void visit();
		void markPath(std::string marker);

	protected:

	private:
		int x;
		int y;
		bool visited;
		std::string value;
		std::string position;
};

#endif // NODE_H
