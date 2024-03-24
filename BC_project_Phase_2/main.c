#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include"list.h"
#include"functions.h"


number *applyOp(number *a, number *b, char op) {
    switch (op) {
        case '+': return add(a, b);
        case '-': return sub(a, b);
        case '*': return mult(a, b);
        //case '/': return division(a, b);
    }
    return a;
}

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

number *evaluate(char *tokens) {
    int i;

    number values[5000]; 
    int valuesTop = -1;

    char ops[5000]; 
    int opsTop = -1;

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
            ops[++opsTop] = tokens[i];
        }
        else if (isdigit(tokens[i])) {
            while (isdigit(tokens[i])) {
                addDigit(z, tokens[i]);// for store infinite number
                i++;
            }
            values[++valuesTop] = *z; //insert number to number array
            i--;
        }
        else if (tokens[i] == ')') {
            while (opsTop != -1 && ops[opsTop] != '(') {
                a = &values[valuesTop--];
                b = &values[valuesTop--];
                char op = ops[opsTop--];
                values[++valuesTop] = *applyOp(b, a, op);
            }
            // Pop opening brace.
            if (opsTop != -1)
                opsTop--;
        }
        else {
            while (opsTop != -1 && precedence(ops[opsTop]) >= precedence(tokens[i])) {
                a = &values[valuesTop--];
                b = &values[valuesTop--];
                char op = ops[opsTop--];
                values[++valuesTop] = *applyOp(b, a, op);
            }
            ops[++opsTop] = tokens[i];
        }
    }

    // Process any remaining operators in the stacks
    while (opsTop != -1) {
        number *a, *b, *z;
        a = (number *)malloc(sizeof(number));
        b = (number *)malloc(sizeof(number));
        z = (number *)malloc(sizeof(number));
        initNumber(a);
        initNumber(b);
        initNumber(z);

        a = &values[valuesTop--];
        b = &values[valuesTop--];
        char op = ops[opsTop--];
        values[++valuesTop] = *applyOp(b, a, op);
    }

    number *r;
    r = (number *)malloc(sizeof(number));
    initNumber(r);
    r = &values[valuesTop];
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
        result = evaluate(line);
    
        printNumber(*result);
    }
    

    return 0;
}

