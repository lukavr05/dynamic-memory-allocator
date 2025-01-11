#include <stdio.h>
#include <stdlib.h>
#define BATCH 10
#define FREE '_'
#define STOP '@'
#define REMOVE 8
#include "functions.c"
int main () {
	struct space mem;
	initialize(&mem);
	struct node *head = NULL;
	char c = '\0';
	while (c != EOF) {
		int t = 0;
		char buf[BATCH];
		while ((c = getchar()) != ' ' && c != '\n' && c != EOF && t < BATCH - 1) {
			*(buf + t) = c;
			t++;
		}
		int p = -1;
		p = allocatorNew(&mem, t + 1, &head);
		if (p >= 0) {
			for (int i = 0; i < t; i++) *((&mem)->memory + p + i) = *(buf + i);
			*((&mem)->memory + p + t) = STOP;
		}
		printf("new word saved: memory=%s(%d)\n", (&mem)->memory, (&mem)->len);
		if (c == '\n'){ 
			struct node *cur = head;
			while (cur) {
				struct node *temp = cur;
				cur = cur->next;
				int size = 0;
				while (*((&mem)->memory + temp->p + size) != STOP) size++;
				if (size < REMOVE) deallocator(&mem, temp->p, &head);
			}
		}
	}
	struct node *cur = head;
	while (head) {
		cur = head;
		head = head->next;
		deallocator(&mem, cur->p, &cur);
	}
	clear(&mem);
}


