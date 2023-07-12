#include "../header/linked_list.h"
#define SRAM_WRITE_DELAY 4096
#define DRAM_WRITE_DELAY 4416
#define NAND_WRITE_DELAY 1402750

//global variables
int global_time = 0;

int sram_wport_busy = 0;
int dram_busy = 0;
int nand_busy = 0;

int pcieq_empty = 0;
int sram_empty = 1;

int nand_enable = 0; //0 : write into dram, 1 : write into nand flash

//functions
void update_global_time(ListNode* sramq, ListNode* dramq, ListNode* nandq) {
	//before the nand write
	if (!nand_enable) {
		if (sramq->link != NULL && dramq->link != NULL) {
			global_time = (sramq->link->end_time < dramq->link->end_time) ? sramq->link->end_time : dramq->link->end_time;
			//exception
			if (sramq->link->link == NULL) global_time = dramq->link->end_time;
		}
		else if (sramq->link != NULL && dramq->link == NULL) {
			global_time = sramq->link->end_time;
		}
		else if (sramq->link == NULL && dramq->link != NULL) {
			global_time = dramq->link->end_time;
		}
	}
	else { //during nand write
		if (dramq->link != NULL && nandq->link != NULL) {
			global_time = nandq->link->end_time;
		}
	}
}

void process_pcieq(ListNode* pcieq, ListNode* sramq) {
	if (!pcieq_empty && !sram_wport_busy) {
		if (pcieq->end_time <= global_time) {
			//store pcieq into p. and then, put the last node into sramq.
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
	//if sramq is empty, enables nand write using nand_enable.
	if (sramq->link != NULL) nand_enable = 0;
	else {
		sram_empty = 1;
		nand_enable = 1;
	}
	//dram write during sramq isn't empty
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
