#include "linked_list.h"
#define SRAM_WRITE_DELAY 4096
#define DRAM_WRITE_DELAY 4416
#define NAND_WRITE_DELAY 1402750

//전역변수
int global_time = 0;

int sram_wport_busy = 0;
int dram_busy = 0;
int nand_busy = 0;

int pcieq_empty = 0;
int sram_empty = 1;

int nand_enable = 0; //0 : dram에 write, 1 : nand에 write

//함수 정의

void update_global_time(ListNode* sramq, ListNode* dramq, ListNode* nandq) {
	//nand write 이전
	if (!nand_enable) {
		if (sramq->link != NULL && dramq->link != NULL) {
			global_time = (sramq->link->end_time < dramq->link->end_time) ? sramq->link->end_time : dramq->link->end_time;
			//예외상황
			if (sramq->link->link == NULL) global_time = dramq->link->end_time;
		}
		else if (sramq->link != NULL && dramq->link == NULL) {
			global_time = sramq->link->end_time;
		}
		else if (sramq->link == NULL && dramq->link != NULL) {
			global_time = dramq->link->end_time;
		}
	}
	else { //nand write 중
		if (dramq->link != NULL && nandq->link != NULL) {
			global_time = nandq->link->end_time;
		}
	}
}

void process_pcieq(ListNode* pcieq, ListNode* sramq) {
	if (!pcieq_empty && !sram_wport_busy) {
		if (pcieq->end_time <= global_time) {
			//p에 pcieq저장 후 마지막 노드의 값을 sramq에 넣는 과정
			ListNode* p = pcieq;
			while (p->link != NULL)
			{
				p = p->link;
			}
			add_node(sramq, p->file_name, p->wr, p->bytes, p->end_time);
			sram_wport_busy = 1;
			sramq->link->end_time = global_time + SRAM_WRITE_DELAY;

			printf("sram\t%d\t\t %d\t %d\t\t %d\n", sramq->link->file_name, sramq->link->wr, sramq->link->bytes, sramq->link->end_time);

			delete_node(pcieq);

			if (pcieq->link == NULL) {
				pcieq_empty = 1;
			}
		}
	}
}

void process_sramq(ListNode* sramq, ListNode* dramq) {
	//sramq가 비면 nand write를 할 수 있도록 nand_enable신호 제어.
	if (sramq->link != NULL) nand_enable = 0;
	else {
		sram_empty = 1;
		nand_enable = 1;
	}
	//sramq가 비어있지 않은 동안 dram write 진행
	if (!sram_empty && !dram_busy && !nand_enable) {
		ListNode* p = sramq;
		while (p->link != NULL) {
			p = p->link;
		}
		add_node(dramq, p->file_name, p->wr, p->bytes, p->end_time);
		dram_busy = 1;
		dramq->link->end_time = global_time + DRAM_WRITE_DELAY;

		printf("dram\t%d\t\t %d\t %d\t\t %d\n", dramq->link->file_name, dramq->link->wr, dramq->link->bytes, dramq->link->end_time);

		delete_node(sramq);
	}
}

void process_dramq(ListNode* sramq, ListNode* dramq, ListNode* nandq) {
	if (nand_enable && !nand_busy) {
		ListNode* p = dramq;
		while (p->link != NULL)
		{
			p = p->link;
		}
		add_node(nandq, p->file_name, p->wr, p->bytes, p->end_time);
		nand_busy = 1;
		nandq->link->end_time = global_time + NAND_WRITE_DELAY;

		printf("nand\t%d\t\t %d\t %d\t\t %d\n", nandq->link->file_name, nandq->link->wr, nandq->link->bytes, nandq->link->end_time);

		delete_node(dramq);
	}
}
