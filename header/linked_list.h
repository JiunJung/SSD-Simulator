#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
	int file_name;
	int wr;
	int bytes;
	int end_time;
	struct ListNode* link;
}ListNode;

ListNode* create_node();

ListNode* init_head();

void add_node(ListNode* head, int file_name, int wr, int bytes, int end_time);

void delete_node(ListNode* head);

void free_list(ListNode* head);
