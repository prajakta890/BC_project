#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "list.h"
#include "functions.h"

//Function to equal the digits after decimal point.
void decimalEqual(number *a, number *b) {
	int i;
	if(a->dec > b->dec) {
		int diff = a->dec - b->dec;
		for(i = 0; i < diff; i++) {
			addDigit(b, '0');
			b->dec++;
		}
	}
	else if(b->dec > a->dec) {
		int diff = b->dec - a->dec;
		for(i = 0; i < diff; i++) {
			addDigit(a, '0');
			a->dec++;
		}
	}
}

//equals the length of both numbers by prepending zeros to small number.
void lengthEqual(number *a, number *b) {
	int gap; 
	gap = length(*a) - length(*b);
	if(gap > 0) {
		int i = 0;
		while(i < gap) {
			appendleft(b, 0);
			i++;
		}
	}
	else if(gap < 0) {
		int i = 0;
		gap = -gap;
		while(i < gap) {
			appendleft(a, 0);
			i++;
		}
	}
}

//checks whether the number is zero or not. returns 0 if it is zero number. function check whether the it is zero or not 
int zeroNumber(number a) {
	int i, flag = 0; //A flag variable to indicate whether a non-zero digit is encountered.
	node *p = a.head;
	for(i = 0; i < length(a); i++) {
		if(p->num != 0)
			flag = 1;
		 p = p->next;
	}
	return flag;
}


//Addition Operation
number *add(number *a, number *b) {
	number *ans;
	ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	decimalEqual(a, b);
	if(a->sign != b->sign) {
		if(a->sign == MINUS) {
			a->sign = PLUS;
			ans = sub(b, a);
		}
		else if(b->sign == MINUS) {
			b->sign = PLUS;
			ans = sub(a, b);
		}
	}
	
	else if(a->sign == b->sign) {
		int i, n1, n2, carry = 0, sum;
		int len_a, len_b;
		node *t1 = a->tail;
		node *t2 = b->tail;
		len_a = length(*a);
		len_b = length(*b);
		if(a->sign == MINUS)
			ans->sign = MINUS;
		else
			ans->sign = PLUS;
		if(len_a >= len_b) {
			for(i = 1; i <= len_b; i++) {
				n1 = t1->num;
				n2 = t2->num;
				sum = n1 + n2 +carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
				t2 = t2->prev;
			}
			for(i = 1; i <= len_a - len_b; i++) {
				n1 = t1->num;
				sum = n1 + carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
			}
		}else {
			for(i = 1; i <= len_a; i++) {
				n1 = t1->num;
				n2 = t2->num;
				sum = n1 + n2 +carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
				t2 = t2->prev;
			}
			for(i = 1; i <= len_b - len_a; i++) {
				n1 = t2->num;
				sum = n1 + carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t2 = t2->prev;
			}
		}
		ans->dec = a->dec;	
		if(carry != 0)
			appendleft(ans, carry);
	}
	return ans;
}

/*Function for finding bigger number among equal length numbers.*/
int compareEqual(number a, number b) {
	lengthEqual(&a, &b);
	decimalEqual(&a, &b);
	node *p, *q;
	int len;
	int i;
	len = length(a);
	p = a.head;
	q = b.head;
	for(i = 1; i <= len; i++) {
		if(p->num > q->num)
			return 1;  //i.e. number a greater than number b.
		else if(p->num < q->num)
			return -1;  //i.e. a is less than b.
		p = p->next;
		q = q->next;
	}
	return 0;    //i.e. both numbers are equal.
}
//Substraction operation.
number *sub(number *a, number *b) {
	number *ans;
	ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	//decimal digits and length made equal.
	decimalEqual(a, b);
	engthEqual(a, b);
	if(a->sign != b->sign) {
		if(a->sign == MINUS) {
			a->sign = PLUS;
			ans = add(a, b);
			ans->sign = MINUS;
		}
		else if(b->sign == MINUS) {
			b->sign = PLUS;
			ans = add(a, b);
			ans->sign = PLUS;
		}
	}
	else if(a->sign == b->sign) {
		if(a->sign == MINUS) {
			a->sign = b->sign = PLUS;
			ans = sub(b, a);
		}
		else if(a->sign == PLUS) {
			int n1, n2, diff, borrow = 0, i, len;
			node *t1 = a->tail;
			node *t2 = b->tail;
			//length of both numbers is same now because of lengthEqual.
			len = length(*b);	
			if(compareEqual(*a, *b) == 1) {
				for(i = 1; i <= len; i++) {
					n1 = t1->num;
					n2 = t2->num;
					n1 = n1 - borrow;
					if(n1 >= n2) {
						diff = n1 - n2;
						borrow = 0;
						appendleft(ans, diff);
					}
					else {
						n1 = n1 + 10;
						diff = n1 - n2;
						borrow = 1;
						appendleft(ans, diff);
					}
					t1 = t1->prev;
					t2 = t2->prev;
				}	
			}
			else if(compareEqual(*a, *b) == -1) {
				ans->sign = MINUS;
				for(i = 1; i <= len; i++) {
					n1 = t1->num;
					n2 = t2->num;
					n2 = n2 - borrow;
					if(n2 >= n1) {
						diff = n2 - n1;
						borrow = 0;
						appendleft(ans, diff);
					}
					else {
						n2 = n2 + 10;
						diff = n2 - n1;
						borrow = 1;
						appendleft(ans, diff);
					}
					t1 = t1->prev;
					t2 = t2->prev;
				}	
			}
			else {
				if(compareEqual(*a, *b) == 0) {
					appendleft(ans, 0);
				}
			}
		}
	}
	ans->dec = a->dec;
	return ans;
}

