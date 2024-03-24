#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include "stack.h"
#include "function.h"

number *calculate(number *a, number *b, char op) {
    switch (op) {
        case '+': return add(a, b);
        case '-': return sub(a, b);
        case '*': return mult(a, b);
        case '/': return division(a, b);
    }
    return a;
}

int check_precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

char ctop(cstack *s){
	char ch;
	ch = cpop(s);
	cpush(s, ch);
	return ch;
}

number *evaluate_infix(char *tokens) {
    int i;

    istack n;
    cstack op;
    iinit(&n);
	cinit(&op);

    for (i = 0; tokens[i] != '\0'; i++) {
        number *a, *b, *z;
        a = (number *)malloc(sizeof(number));
        b = (number *)malloc(sizeof(number));
        z = (number *)malloc(sizeof(number));
        initNumber(a);
        initNumber(b);
        initNumber(z);

        if (tokens[i] == ' ')
            continue;
        else if (tokens[i] == '(') {
            cpush(&op, tokens[i]);
        }
        else if (isdigit(tokens[i])) {
			int decstart = 0;
			int decpoint = 0;
			while (isdigit(tokens[i])) {
				if (decstart == 1) {
				    decpoint = decpoint + 1;
				    z->dec = decpoint;
				}
				addDigit(z, tokens[i]);
				i++;
				if (tokens[i] == '.') {
				    decstart = 1;
				    i++;
				}
			}
			ipush(&n, z);
			i--;
		}

        else if (tokens[i] == ')') {
            while (op != NULL && ctop(&op) != '(') { 
                a = ipop(&n);
                b = ipop(&n);
                ipush(&n, calculate(b, a, cpop(&op)));
            }
            if (op != NULL)
                cpop(&op);
        }
        else {
            while (op != NULL && check_precedence(ctop(&op)) >= check_precedence(tokens[i])) {  
                a = ipop(&n);
                b = ipop(&n);
                ipush(&n, calculate(b, a, cpop(&op)));
            }
            cpush(&op, tokens[i]);
        }
    }

    while (op != NULL) {
        number *a, *b, *z;
        a = (number *)malloc(sizeof(number));
        b = (number *)malloc(sizeof(number));
        initNumber(a);
        initNumber(b);

        a = ipop(&n);
        b = ipop(&n);
        ipush(&n, calculate(b, a, cpop(&op)));
    }

    number *r;
    r = (number *)malloc(sizeof(number));
    initNumber(r);
    r = ipop(&n);
    return r;
}

int readline(char *line, int len){
    int i;
    char ch;
    i = 0;
    while (i < len - 1)
    {
        ch = getchar();
        if (ch == '\n')
        {
            line[i++] = '\0';
            return i - 1;
        }
        else
            line[i++] = ch;
    }
    line[len - 1] = '\0';
    return len - 1;
}


int main() {
    char line[2048];
    while(readline(line, 2048)){
        number *result = (number *)malloc(sizeof(number));
        initNumber(result);
        result = evaluate_infix(line);
    
        printNumber(*result);
    }

    return 0;
}
