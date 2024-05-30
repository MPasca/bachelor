#include<iostream>
#include<queue>
#include<cmath>

#include "PaCo.h"

namespace paco
{
    void print_path(DL_List path)
    {
        ListNode* crtNode = path.first;
        while (crtNode != path.last)
        {
            std::cout << crtNode->coordinates.first << " " << crtNode->coordinates.second << "\n";
            crtNode = crtNode->next;
        }
    }


    ListNode* search(DL_List existingPath, std::pair<int, int> searchedCoordinates) {
        ListNode* searchedNode = existingPath.first;

        if (existingPath.last->coordinates == searchedCoordinates)
        {
            return existingPath.last;
        }

        while (searchedNode != existingPath.last)
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

    void insert_last(DL_List* existingPath, Node crtNode)
    {
        ListNode* p = (ListNode*)malloc(sizeof(ListNode));
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

    void delete_after_node(ListNode* root)
    {
        if (root == NULL) return;

        delete_after_node(root->next);
        free(root);
    }

    void insert_after_key(DL_List* crtPath, ListNode* afterKey, DL_List newPath) {
        newPath.first->prev = afterKey;
        delete_after_node(afterKey->next);
        afterKey->next = newPath.first;

        crtPath->last = newPath.last;
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


    void dfs_visit(Node* root, std::pair<int, int> searchedCoordinates, DL_List* newPath)
    {
        root->color = GRAY;
        if (root->coordinates == searchedCoordinates)
        {
            insert_first(newPath, *root);
        }
        else
        {
            for (int i = 0; i < root->numberOfNeighbors; i++)
            {
                if (root->neighbors[i]->color == WHITE)
                {
                    dfs_visit(root->neighbors[i], searchedCoordinates, newPath);
                    if (newPath->first != NULL)
                    {
                        insert_first(newPath, *root->neighbors[i]);
                        break;
                    }
                }
            }
        }

        root->color = BLACK;
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

    void dfs_visit(Node* root, DL_List existingPath, DL_List* newPath)
    {
        root->color = GRAY;
        if (search(existingPath, root->coordinates) != NULL)
        {
            insert_last(newPath, *root);
        }
        else
        {
            for (int i = 0; i < root->numberOfNeighbors; i++)
            {
                if (root->neighbors[i]->color == WHITE)
                {
                    dfs_visit(root->neighbors[i], existingPath, newPath);
                    if (newPath->first != NULL)
                    {
                        insert_last(newPath, *root->neighbors[i]);
                        break;
                    }
                }
            }
        }

        root->color = BLACK;
    }

    DL_List* path_from_node(Node* root, std::pair<int, int> searchedCoordinates)
    {
        DL_List resultedPath;
        resultedPath.first = NULL;
        resultedPath.last = NULL;
        dfs_visit(root, searchedCoordinates, &resultedPath);
        insert_first(&resultedPath, *root);
        refresh_dfs(root);

        return &resultedPath;
    }

    DL_List path_from_list(Node* root, DL_List searchedList)
    {
        DL_List resultedPath;
        resultedPath.first = NULL;
        resultedPath.last = NULL;
        dfs_visit(root, searchedList, &resultedPath);
        insert_last(&resultedPath, *root);
        refresh_dfs(root);

        return resultedPath;
    }

    void concat_lists(DL_List* previousPath, DL_List newPath)
    {
        ListNode* sharedNode = search(*previousPath, newPath.last->coordinates);
        insert_after_key(previousPath, sharedNode, newPath);
    }

    std::pair<int, int> get_next_move(Node* root, Node* goal, DL_List* existingPath)
    {
        if (existingPath->first == NULL)
        {
            existingPath = path_from_node(root, goal->coordinates);
        }
        else if (search(*existingPath, goal->coordinates) != NULL)
        {
            ListNode* newTail = search(*existingPath, goal->coordinates);
            delete_after_node(newTail->next);
            existingPath->last = newTail;
        }
        else
        {
            DL_List newPath = path_from_list(goal, *existingPath);
            concat_lists(existingPath, newPath);
        }

        if (root->coordinates == (existingPath)->first->coordinates)
        {
            delete_first(existingPath);
        }

        return existingPath->first->coordinates;
    }


    void demo()
    {
        Node* nodes = (Node*)calloc(12, sizeof(Node));
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

        DL_List resultedPath;
        resultedPath.first = NULL;
        resultedPath.last = NULL;
        std::pair<int, int> nextCoordinates = get_next_move(&nodes[0], &nodes[5], &resultedPath);
        std::cout << "next move: " << nextCoordinates.first << ", " << nextCoordinates.second << "\n";
        print_path(resultedPath);

        nextCoordinates = get_next_move(&nodes[4], &nodes[5], &resultedPath);
        std::cout << "next move: " << nextCoordinates.first << ", " << nextCoordinates.second << "\n";
        print_path(resultedPath);

        nextCoordinates = get_next_move(&nodes[10], &nodes[5], &resultedPath);
        std::cout << "next move: " << nextCoordinates.first << ", " << nextCoordinates.second << "\n";
        print_path(resultedPath);

        nextCoordinates = get_next_move(&nodes[8], &nodes[5], &resultedPath);
        std::cout << "next move: " << nextCoordinates.first << ", " << nextCoordinates.second << "\n";
        print_path(resultedPath);
    }
}

