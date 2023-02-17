#include "linked_list.h"

//��� ����
ListNode* create_node() {
	ListNode* p = (ListNode*)malloc(sizeof(ListNode));
	p->link = NULL;
	return p;
}

// link�� NULL�� ��� ���� �� head ����
ListNode* init_head() {
	ListNode* head = create_node();
	return head;
}

//��� ���ʿ� ���ο� ��� �߰�
void add_node(ListNode* head, int file_name, int wr, int bytes, int end_time) {
	ListNode* new_node = create_node();

	new_node->file_name = file_name;
	new_node->wr = wr;
	new_node->bytes = bytes;
	new_node->end_time = end_time;

	new_node->link = head->link;
	head->link = new_node;

}

//�� ���� ��� ����
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