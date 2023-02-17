#include "linked_list.h"

//노드 생성
ListNode* create_node() {
	ListNode* p = (ListNode*)malloc(sizeof(ListNode));
	p->link = NULL;
	return p;
}

// link가 NULL인 노드 생성 후 head 리턴
ListNode* init_head() {
	ListNode* head = create_node();
	return head;
}

//노드 앞쪽에 새로운 노드 추가
void add_node(ListNode* head, int file_name, int wr, int bytes, int end_time) {
	ListNode* new_node = create_node();

	new_node->file_name = file_name;
	new_node->wr = wr;
	new_node->bytes = bytes;
	new_node->end_time = end_time;

	new_node->link = head->link;
	head->link = new_node;

}

//맨 뒤의 노드 삭제
void delete_node(ListNode* head) {
	ListNode* p = head;
	if (p->link != NULL) {
		while (p->link->link != NULL)
		{
			p = p->link;
		}
		p->link = NULL;
	}
}

void free_list(ListNode* head) {
	ListNode* target = head;
	ListNode* p = target;
	while (target != NULL)
	{
		p = p->link;
		free(target);
		target = p;
	}
}