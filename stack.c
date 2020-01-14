#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "da.h"
#include "stack.h"

typedef struct stack {
    void (*display)(void *, FILE *);
    void (*free)(void *);
    DA *da;
    int debug;
}STACK;

STACK *newSTACK(void) {
    STACK *items = malloc(sizeof(STACK));
    assert(items != 0);
    items->debug = 0;
    items->da = newDA();
    return items;
}

void setSTACKdisplay(STACK *items, void (*d)(void *, FILE *)) {
    items->display = d;
    setDAdisplay(items->da, d);
}

void setSTACKfree(STACK *items, void (*f)(void *)) {
    items->free = f;
    setDAfree(items->da, f);
}

void push(STACK *items, void *value) {
    insertDAback(items->da, value);
}

void *pop(STACK *items) {
    assert(sizeDA(items->da) > 0);
    return removeDAback(items->da);
}

void *peekSTACK(STACK *items) {
    assert(sizeDA(items->da) > 0);
    return getDA(items->da, sizeDA(items->da) - 1);
}

void displaySTACK(STACK *items, FILE *fp) {
    if (items->debug > 0) {
        debugDA(items->da, items->debug - 1);
        displayDA(items->da, fp);
        return;
    }
    fprintf(fp, "|");
    int i;
    for (i = sizeDA(items->da) - 1; i >= 0; --i) {
        if (items->display == NULL) {
            fprintf(fp, "@%p", getDA(items->da, i));
        }
        else {
            items->display(getDA(items->da, i), fp);
        }
        if (i != 0) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "|");
}

int debugSTACK(STACK *items, int level) {
    int old = items->debug;
    items->debug = level;
    return old;
}

void freeSTACK(STACK *items) {
    freeDA(items->da);
    free(items);
}

int sizeSTACK(STACK *items) {
    return sizeDA(items->da);
}
