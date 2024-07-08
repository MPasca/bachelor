#include<iostream>
#include<queue>
#include<cmath>

#include "PaCo.h"

namespace paco
{
    void print_path(DL_List* path)
    {
        ListNode* crtNode = path->first;
        while (crtNode != nullptr)
        {
            std::cout << crtNode->coordinates.first << " " << crtNode->coordinates.second << "\n";
            crtNode = crtNode->next;
        }
    }

    void destroy_nodes(Node* root, Node* parent)
    {
        if (root != nullptr)
        {
            for (int i = 0; i < root->numberOfNeighbors; i++)
            {
                if (parent == nullptr || root->neighbors[i] != parent)
                {
                    destroy_nodes(root->neighbors[i], root);
                }
            }
            free(root->neighbors);
            free(root);
        }
    }




    ListNode* search(DL_List* existingPath, std::pair<int, int> searchedCoordinates) {
        ListNode* searchedNode = existingPath->first;

        if (existingPath->last->coordinates == searchedCoordinates)
        {
            return existingPath->last;
        }

        while (searchedNode != existingPath->last)
        {
            if (searchedNode->coordinates == searchedCoordinates)
            {
                return searchedNode;
            }

            searchedNode = searchedNode->next;
        }

        return NULL;
    }

    void insert_first(DL_List* existingPath, Node crtNode)
    {
        ListNode* p = (ListNode*)malloc(sizeof(ListNode));
        if (p)
        {
            p->coordinates = crtNode.coordinates;
            p->prev = NULL;
            p->next = NULL;

            if (existingPath->first == NULL)
            {
                existingPath->first = existingPath->last = p;
            }
            else
            {
                p->next = existingPath->first;
                existingPath->first->prev = p;
                existingPath->first = p;
            }
        }
        else
        {
            std::cerr << "Error allocating memory to p\n";
        }
    }

    void insert_last(DL_List* existingPath, Node crtNode)
    {
        ListNode* p = (ListNode*)malloc(sizeof(ListNode));
        if (p)
        {
            p->coordinates = crtNode.coordinates;
            p->next = NULL;
            p->prev = NULL;
            if (existingPath->last == NULL)
            {
                existingPath->first = existingPath->last = p;
            }
            else
            {
                p->prev = existingPath->last;
                existingPath->last->next = p;
                existingPath->last = p;
            }
        }
        else
        {
            std::cerr << "Error allocating memory to p\n";
        }
    }

    void delete_after_node(ListNode* root)
    {
        if (root == NULL) return;

        delete_after_node(root->next);
        root->next = NULL;
    }

    DL_List* insert_after_key(DL_List* crtPath, ListNode* afterKey, DL_List* newPath) {
        newPath->first->prev = afterKey;
        delete_after_node(afterKey->next);
        afterKey->next = newPath->first;

        crtPath->last = newPath->last;
        return crtPath;
    }

    void delete_first(DL_List* existingPath) {
        ListNode* auxNode = existingPath->first;

        if (existingPath->first == existingPath->last)
        {
            existingPath->first = existingPath->last = NULL;
        }
        else
        {
            existingPath->first->next->prev = NULL;
            existingPath->first = existingPath->first->next;
        }
        free(auxNode);
    }


    bool dfs_visit(Node* root, Node searchedNode, DL_List* newPath)
    {
        root->color = GRAY;
        if (root->coordinates == searchedNode.coordinates)
        {
            insert_first(newPath, *root);
            return true;
        }
        else
        {
            for (int i = 0; i < root->numberOfNeighbors; i++)
            {
                if (root->neighbors[i]->color == WHITE)
                {
                    if (dfs_visit(root->neighbors[i], searchedNode, newPath) == true)
                    {
                        insert_first(newPath, *root->neighbors[i]);
                        return true;
                    }
                }
            }
        }

        root->color = BLACK;
        return false;
    }

    bool dfs_visit(Node* root, DL_List* existingPath, DL_List* newPath)
    {
        root->color = GRAY;
        if (search(existingPath, root->coordinates) != NULL)
        {
            insert_last(newPath, *root);
            return true;
        }
        else
        {
            for (int i = 0; i < root->numberOfNeighbors; i++)
            {
                if (root->neighbors[i]->color == WHITE)
                {
                    if (dfs_visit(root->neighbors[i], existingPath, newPath) == true)
                    {
                        insert_last(newPath, *root->neighbors[i]);
                        return true;
                    }
                }
            }
        }

        root->color = BLACK;
        return false;
    }

    void refresh_dfs(Node* root)
    {
        root->color = WHITE;

        for (int i = 0; i < root->numberOfNeighbors; i++)
        {
            if (root->neighbors[i]->color != WHITE)
            {
                refresh_dfs(root->neighbors[i]);
            }
        }
    }

    DL_List* path_from_node(Node* root, Node searchedNode)
    {
        DL_List* resultedPath = (DL_List*)malloc(sizeof(DL_List));
        if (resultedPath)
        {
            resultedPath->first = NULL;
            resultedPath->last = NULL;
            dfs_visit(root, searchedNode, resultedPath);
            insert_first(resultedPath, *root);
            refresh_dfs(root);
        }

        return resultedPath;
    }

    DL_List* concat_lists(DL_List* previousPath, DL_List* newPath)
    {
        ListNode* sharedNode = search(previousPath, newPath->first->coordinates);
        return insert_after_key(previousPath, sharedNode, newPath);
    }

    DL_List* path_from_list(Node* root, DL_List* searchedList)
    {
        DL_List* resultedPath = (DL_List*)malloc(sizeof(DL_List));
        if (resultedPath)
        {
            resultedPath->first = NULL;
            resultedPath->last = NULL;
            dfs_visit(root, searchedList, resultedPath);
            insert_last(resultedPath, *root);
            refresh_dfs(root);
        }

        return concat_lists(searchedList, resultedPath);
    }

    DL_List* get_next_move(Node* root, Node* goal, DL_List* existingPath)
    {
        if (existingPath == NULL)
        {
            existingPath = path_from_node(root, *goal);
            if (existingPath == NULL)
            {
                std::cerr << "Error creating the path for npc!\n";
                exit(-1);
            }
        }
        else
        {
            ListNode* newTail = search(existingPath, goal->coordinates);
            if (newTail != NULL)
            {
                delete_after_node(newTail->next);
                existingPath->last = newTail;
            }
            else
            {
                DL_List* newPath = path_from_list(goal, existingPath);
                if (newPath == NULL)
                {
                    std::cerr << "Error creating the path for npc!\n";
                    exit(-1);
                }
            }
        }

        if (root->coordinates == existingPath->first->coordinates)
        {
            delete_first(existingPath);
        }

        return existingPath;
    }


    void demo()
    {
        Node* nodes = (Node*)calloc(12, sizeof(Node));
        if (!nodes)
        {
            std::cerr << "Error allocating memory for nodes\n";
        }
        else
        {
            nodes[0].numberOfNeighbors = 1;
            nodes[1].numberOfNeighbors = 2;
            nodes[2].numberOfNeighbors = 1;
            nodes[3].numberOfNeighbors = 3;
            nodes[4].numberOfNeighbors = 3;
            nodes[5].numberOfNeighbors = 1;
            nodes[6].numberOfNeighbors = 1;
            nodes[7].numberOfNeighbors = 2;
            nodes[8].numberOfNeighbors = 2;
            nodes[9].numberOfNeighbors = 1;
            nodes[10].numberOfNeighbors = 3;
            nodes[11].numberOfNeighbors = 2;

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    nodes[i * 3 + j].coordinates = { i, j };
                    nodes[i * 3 + j].neighbors = (Node**)calloc(nodes[i * 3 + j].numberOfNeighbors, sizeof(Node*));
                }
            }

            if (nodes[0].neighbors && nodes[1].neighbors && nodes[2].neighbors && nodes[3].neighbors && nodes[4].neighbors && nodes[5].neighbors
                && nodes[6].neighbors && nodes[7].neighbors && nodes[8].neighbors && nodes[9].neighbors && nodes[10].neighbors && nodes[11].neighbors)
            {
                nodes[0].neighbors[0] = &nodes[3];
                nodes[1].neighbors[0] = &nodes[4];
                nodes[1].neighbors[1] = &nodes[2];

                nodes[2].neighbors[0] = &nodes[1];

                nodes[3].neighbors[0] = &nodes[0];
                nodes[3].neighbors[1] = &nodes[4];
                nodes[3].neighbors[2] = &nodes[6];

                nodes[4].neighbors[0] = &nodes[3];
                nodes[4].neighbors[1] = &nodes[1];
                nodes[4].neighbors[2] = &nodes[7];

                nodes[5].neighbors[0] = &nodes[8];

                nodes[6].neighbors[0] = &nodes[3];

                nodes[7].neighbors[0] = &nodes[4];
                nodes[7].neighbors[1] = &nodes[10];

                nodes[8].neighbors[0] = &nodes[11];
                nodes[8].neighbors[1] = &nodes[5];

                nodes[9].neighbors[0] = &nodes[10];

                nodes[10].neighbors[0] = &nodes[7];
                nodes[10].neighbors[1] = &nodes[9];
                nodes[10].neighbors[2] = &nodes[11];

                nodes[11].neighbors[0] = &nodes[10];
                nodes[11].neighbors[1] = &nodes[8];

                DL_List* resultedPath = (DL_List*)malloc(sizeof(DL_List));
                if (resultedPath)
                {
                    resultedPath->first = NULL;
                    resultedPath->last = NULL;

                    resultedPath = get_next_move(&nodes[0], &nodes[5], resultedPath);
                    std::pair<int, int> nextCoordinates = resultedPath->first->coordinates;
                    delete_first(resultedPath);
                    std::cout << "next move: " << nextCoordinates.first << ", " << nextCoordinates.second << "\n";
                    print_path(resultedPath);

                    resultedPath = get_next_move(&nodes[4], &nodes[5], resultedPath);
                    nextCoordinates = resultedPath->first->coordinates;
                    delete_first(resultedPath);
                    std::cout << "next move: " << nextCoordinates.first << ", " << nextCoordinates.second << "\n";
                    print_path(resultedPath);

                    resultedPath = get_next_move(&nodes[10], &nodes[5], resultedPath);
                    nextCoordinates = resultedPath->first->coordinates;
                    delete_first(resultedPath);
                    std::cout << "next move: " << nextCoordinates.first << ", " << nextCoordinates.second << "\n";
                    print_path(resultedPath);

                    resultedPath = get_next_move(&nodes[8], &nodes[0], resultedPath);
                    nextCoordinates = resultedPath->first->coordinates;
                    delete_first(resultedPath);
                    std::cout << "next move: " << nextCoordinates.first << ", " << nextCoordinates.second << "\n";
                    print_path(resultedPath);
                }
            }
        }
    }
}

