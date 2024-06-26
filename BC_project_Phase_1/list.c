#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void initNumber(number *a) {
	a->head = a->tail = NULL;
	a->dec = 0;
	a->sign = PLUS;
}

void addDigit(number *a, char ch) { //add node to last
	if(ch < '0' || ch > '9')
		return;
	node *tmp = (node *)malloc(sizeof(node));
	if(tmp == NULL)
		return;
	tmp->num = ch - '0'; //convert char to int
	tmp->next = NULL;
	if((a->head == NULL) && (a->tail == NULL)) {
		tmp->prev = NULL;
		a->head = tmp;
		a->tail = tmp;
		return;
	}
	tmp->prev = a->tail;
	a->tail->next = tmp;
	a->tail = tmp;
	return;
}

int length(number c) {
	node *p;
    p = c.head;
	int count = 0;
	while(p != NULL) {
		count++;
		p = p->next;
	}
	return count;
}

void appendleft(number *c, int no) {
	if(no < 0 || no > 9)
		return;
	node *tmp = (node *)malloc(sizeof(node));
	if(!tmp)
		return;
	tmp->num = no;
	tmp->prev = NULL;
	if((c->head == NULL) && (c->tail == NULL)){
		tmp->next = NULL;
		c->head = c->tail = tmp;
		return;
	}
	tmp->next = c->head;
	c->head->prev = tmp;
	c->head = tmp;
	return;
}

//Function to remove extra zeroes on left of number.
void zeroRemov(number *a) {
	node *p = a->head;
	int i, len = length(*a);
	for(i = 0; i < len - a->dec -1; i++) {
		if(p->num == 0) {
			a->head = p->next;
			p->next = NULL;
			a->head->prev = NULL;
			free(p);
			p = a->head;
		}else {
			break;
		}
	}
	p = a->tail;
	int decimal = a->dec;
	for(i = 0; i < decimal - 1; i++) {
		if(p->num == 0) {
			a->tail = p->prev;
			p->prev = NULL;
			a->tail->next = NULL;
			free(p);
			p = a->tail;
			a->dec--;
			
		}else {
			break;
		}
	}
	return;
}
void printNumber(number c) {
	node *p;
	int pos = 0;
	zeroRemov(&c);
	p = c.head;
	if(c.sign == MINUS) {
		printf("-");
		c.sign = PLUS;
	}
	while(p != NULL) {
		pos++;
		if(pos == (length(c) - c.dec + 1))
			printf(".");
		printf("%d", p->num);
		p = p->next;
	}
	printf("\n");
}
