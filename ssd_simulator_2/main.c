#define _CRT_SECURE_NO_WARNINGS
#include "linked_list.h"
#include "ssd_sim.h"


int main() {
	//file read from "files.txt"
	int files[3][4] = { {0,},};
	FILE* fp = NULL;
	fp = fopen("files.txt", "r");
	int i = 0;
	for (i = 0; i < 3; i++) {
		fscanf(fp, "%d %d %d %d", &files[i][0], &files[i][1], &files[i][2], &files[i][3]);
	}
	fclose(fp);

	//linked list for each devices
	ListNode* pcieq = init_head();
	ListNode* sramq = init_head();
	ListNode* dramq = init_head();
	ListNode* nandq = init_head();

	//pcieq에 files.txt의 내용 넣기
	for (i = 0; i < 3; i++) {
		add_node(pcieq, files[i][0], files[i][1], files[i][2], files[i][3]);
	}

	//시뮬레이션 화면 상단 제목
	printf("device \tfile name \t w/r \t size(byte) \t end time(ns)\n");

	int count = 0;
	while (count<100) {
		
		process_pcieq(pcieq,sramq);
		process_sramq(sramq,dramq);
		process_dramq(sramq, dramq,nandq);

		update_global_time(sramq, dramq, nandq);

		//chech if sram is empty
		if (sramq->link != NULL) sram_empty = 0;

		//check each devices if they are busy
		if (sramq->link != NULL && sramq->link->end_time <= global_time) sram_wport_busy = 0;
		if (dramq->link != NULL && dramq->link->end_time <= global_time) dram_busy = 0;
		if (nandq->link != NULL && nandq->link->end_time <= global_time) nand_busy = 0;

		count++;
	}

	//heap영역의 할당 공간 해제
	free_list(pcieq);
	free_list(sramq);
	free_list(dramq);
	free_list(nandq);

	return 0;
}
