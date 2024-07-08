// MaGe.cpp : Defines the functions for the static library.
//
#include<iostream>
#include <cstdlib>
#include<cmath>

#include "MaGe.h"
#include "../ErrorCodes.h"


namespace mage
{
	Node* make_node(std::pair<int, int> coord)
	{
		Node* newNode = (Node*)calloc(1, sizeof(Node));

		if (newNode == nullptr)
		{
			std::cerr<<"Node not created!";
			exit(-1);
		}

		newNode->coord = coord;
		newNode->color = WHITE;
		newNode->parent = newNode;
		newNode->totNeighbors = 0;
		newNode->neighbors = (Node**)calloc(4, sizeof(Node*));

		return newNode;
	}

	bool find_node(Node* toFind, Node* collection)
	{
		for (int i = 0; i < collection->totNeighbors; i++)
		{
			if (collection->neighbors[i] == toFind)
			{
				return true;
			}
		}
		return false;
	}

	void update_parents(Node* toUpdate, Node* newParent)
	{
		toUpdate->parent = newParent;
		for (int i = 0; i < toUpdate->totNeighbors; i++)
		{
			if (toUpdate->neighbors[i]->parent != newParent)
			{
				update_parents(toUpdate->neighbors[i], newParent);
			}
		}
	}

	void descend(Node* nodeUp, Node* nodeDown)
	{
		update_parents(nodeDown, nodeUp->parent);

		nodeUp->neighbors[nodeUp->totNeighbors] = nodeDown;
		nodeDown->neighbors[nodeDown->totNeighbors] = nodeUp;

		nodeUp->totNeighbors += 1;
		nodeDown->totNeighbors += 1;
	}

	void concat(Node* nodeLeft, Node* nodeRight)
	{
		if (nodeLeft->parent == nodeRight->parent)
		{
			return;
		}

		update_parents(nodeRight, nodeLeft->parent);

		nodeRight->neighbors[nodeRight->totNeighbors] = nodeLeft;
		nodeLeft->neighbors[nodeLeft->totNeighbors] = nodeRight;

		nodeRight->totNeighbors += 1;
		nodeLeft->totNeighbors += 1;
	}

	Node* fn_generate(int n, int m)
	{
		// current row
		int  i = 0;

		Node** crtRow = (Node**)calloc(m, sizeof(Node*));

		if (crtRow == NULL)
		{
			std::cerr << "Node not created!";
			exit(-1);
		}

		for (int j = 0; j < m; j++)
		{
			crtRow[j] = make_node({ i, j });
			if (crtRow[j] == NULL)
			{
				std::cerr<<"Node not created!";
				exit(-1);
			}
		}

		do {
			for (int j = 0; j < m; j++)
			{
				// concat? yes or no
				if (rand() % 2)
				{
					if (j == 0)
					{
						concat(crtRow[0], crtRow[1]);
					}
					else if (j == m - 1)
					{
						concat(crtRow[m - 2], crtRow[m - 1]);
					}
					else
					{
						// left or right? yes: right, no: left
						if (rand() % 2)
						{
							concat(crtRow[j], crtRow[j + 1]);
						}
						else
						{
							concat(crtRow[j - 1], crtRow[j]);
						}
					}
				}
			}

			i++;

			Node** nextRow = (Node**)calloc(m, sizeof(Node*));
			if (nextRow == NULL)
			{
				std::cerr << "Node not created!";
				exit(-1);
			}

			for (int j = 0; j < m; j++)
			{
				nextRow[j] = make_node({ i, j });
				if (nextRow[j] == NULL)
				{
					std::cerr<<"Node not created!";
					exit(-1);
				}
			}

			bool mustDescend = false;
			for (int j = 0; j < m; j++)
			{
				if (j == 0 && crtRow[j]->parent != crtRow[j + 1]->parent)
				{
					mustDescend = true;
				}

				if ((j != 0 && crtRow[j - 1]->parent != crtRow[j]->parent) || mustDescend)
				{
					descend(crtRow[j], nextRow[j]);
					mustDescend = false;
					continue;
				}

				if (rand() % 2)
				{
					mustDescend = false;
					descend(crtRow[j], nextRow[j]);
				}
			}

			crtRow = nextRow;
		} while (i < n - 1);

		for (int j = 0; j < m - 1; j++)
		{
			if (crtRow[j]->parent != crtRow[j + 1]->parent)
			{
				concat(crtRow[j], crtRow[j + 1]);
			}
		}

		return crtRow[m - 1];
	}

	// pseudocode from Cormen adjusted to our needs: no time variables,
	// but with a counter for visited elements used at the end to check whether or not all elements have been visited
	// checks to see whether or not there are cycles in the generated maze
	int counter = 0;
	bool dfs_visit(Node* crtNode, Node* prevNode)
	{
		counter++;
		crtNode->color = GRAY;
		for (int i = 0; i < crtNode->totNeighbors; i++)
		{
			if (prevNode == nullptr || crtNode->neighbors[i] != prevNode)
			{
				if (crtNode->neighbors[i]->color != WHITE)
				{
					return false;
				}
				if (crtNode->neighbors[i]->color == WHITE)
				{
					if (!dfs_visit(crtNode->neighbors[i], crtNode))
					{
						return false;
					}
				}
			}
		}
		crtNode->color = BLACK;
		return true;
	}

	bool fn_validate(Node* root, int n, int m)
	{
		counter = 0;
		return dfs_visit(root, nullptr) && counter == n * m;
	}


	void fn_print(Node* crtNode, Node* prevNode)
	{
		crtNode->color = GRAY;
		std::cout << "{ " << crtNode->coord.first << ", " << crtNode->coord.second << " }:\n";
		std::cout << "neighbors: ";
		for (int i = 0; i < crtNode->totNeighbors; i++)
		{
			std::cout << "{ " << crtNode->neighbors[i]->coord.first << ", " << crtNode->neighbors[i]->coord.second << " }, ";
		}
		std::cout << "\n";
		for (int i = 0; i < crtNode->totNeighbors; i++)
		{
			if (crtNode->neighbors[i]->color == BLACK)
			{
				fn_print(crtNode->neighbors[i], crtNode);
			}
		}
		crtNode->color = WHITE;
	}

	Node* fn_create(int height, int width)
	{
		if (height < 1 || width < 1)
		{
			std::cerr << "Height and width cannot be less than 1!\n";
			exit(ERR_INCORRECT_DIMENSIONS);
		}

		srand(time(NULL));
		Node* maze;
		do
		{
			maze = fn_generate(height, width);
		} while (!fn_validate(maze, height, width));

		return maze;
	}

	void fn_clean(Node* crtNode, Node* prevNode)
	{
		if (crtNode != nullptr)
		{
			for (int i = 0; i < crtNode->totNeighbors; i++)
			{
				if (prevNode == nullptr || crtNode->neighbors[i] != prevNode)
				{
					fn_clean(crtNode->neighbors[i], crtNode);
				}
			}
			free(crtNode->neighbors);
			free(crtNode);
		}
	}

	void fn_demo()
	{
		Node* maze = fn_generate(100, 100);

		std::cout << "success";
	}

}


