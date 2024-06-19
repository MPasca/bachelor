#pragma once

namespace mage
{
	typedef enum Color {
		WHITE, GRAY, BLACK
	}Color;


	typedef struct Node {
		int totNeighbors;
		Node* parent;
		Node** neighbors;		// the list of nodes the element is connected with
		std::pair<int, int> coord;		// the coordinates of the node
		Color color;
	} Node;

	Node* fn_create(int height, int width);

	void fn_demo();

	void fn_clean(Node* crtNode, Node* prevNode);

	void fn_print(Node* crtNode, Node* prevNode);
}

