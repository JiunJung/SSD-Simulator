#define _CRT_SECURE_NO_WARNINGS
#include "../header/linked_list.h"
#include "../header/ssd_sim.h"


int main() {
	//"files.txt" stores 3x4 matrix.
	//   file name(as number)  |  R(1)/W(0)  |  file size (bytes)  |  arrival time (ns)
	//(example)  3243                 0               4096                   0
	//            101                 0               4096                   1
	//           2222                 0               4096                   2
	//(notice) you can actually ignore about arrival time. It doesn't matter in this project.
	         
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

	//put the information from files.txt into pcieq
	for (i = 0; i < 3; i++) {
		add_node(pcieq, files[i][0], files[i][1], files[i][2], files[i][3]);
	}

	//simulation display
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

	//make free the heap storage
	free_list(pcieq);
	free_list(sramq);
	free_list(dramq);
	free_list(nandq);

	return 0;
}
