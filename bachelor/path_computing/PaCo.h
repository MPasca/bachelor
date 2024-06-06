#pragma once

namespace paco
{
    typedef enum {
        WHITE, GRAY, BLACK
    } Color;

    typedef struct Node {
        std::pair<int, int> coordinates;
        int numberOfNeighbors;
        Node** neighbors;
        Color color;
    }Node;

    typedef struct node {
        std::pair<int, int> coordinates;
        struct node* next;
        struct node* prev;
    } ListNode;

    typedef struct DL_List {
        ListNode* first;
        ListNode* last;
    }DL_List;


    DL_List* get_next_move(Node* root, Node* goal, DL_List* existingPath);

    void demo();
}
