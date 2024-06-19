#pragma once

#include<iostream>

namespace paco
{
    typedef enum {
        WHITE, GRAY, BLACK
    } Color;

    typedef struct Node {
        std::pair<int, int> coordinates;
        int numberOfNeighbors = 0;
        Node** neighbors = nullptr;
        Color color = WHITE;
    } Node;

    typedef struct ListNode {
        std::pair<int, int> coordinates;
        struct ListNode* next;
        struct ListNode* prev;
    } ListNode;

    typedef struct DL_List {
        ListNode* first;
        ListNode* last;
    } DL_List;


    DL_List* get_next_move(Node* root, Node* goal, DL_List* existingPath);

    void delete_first(DL_List* existingPath);

    void print_path(DL_List* path);

    void demo();
}
