#pragma once
#include "linked_list.h"

extern int global_time;

extern int sram_wport_busy;
extern int dram_busy;
extern int nand_busy;

extern int pcieq_empty;
extern int sram_empty;

extern int nand_enable; //0 : dram write, 1 : nand write

void update_global_time(ListNode* sramq, ListNode* dramq, ListNode* nandq);
void process_pcieq(ListNode* pcieq, ListNode* sramq);
void process_sramq(ListNode* sramq, ListNode* dramq);
void process_dramq(ListNode* sramq, ListNode* dramq, ListNode* nandq);