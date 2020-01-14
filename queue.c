#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cda.h"
#include "queue.h"

typedef struct queue {
    void (*display)(void *, FILE *);
    void (*free)(void *);
    int debug;
    CDA *cda;
}QUEUE;

QUEUE *newQUEUE(void) {
    QUEUE *items = malloc(sizeof(QUEUE));
    assert(items != 0);
    items->cda = newCDA();
    items->debug = 0;
    return items;
}

void setQUEUEdisplay(QUEUE *items, void (*d)(void *, FILE *)) {
    items->display = d;
    setCDAdisplay(items->cda, d);
}

void setQUEUEfree(QUEUE *items, void (*f)(void *)) {
    items->free = f;
    setCDAfree(items->cda, f);
}

void enqueue(QUEUE *items, void *value) {
    insertCDAback(items->cda, value);
}

void *dequeue(QUEUE *items) {
    assert(sizeCDA(items->cda) > 0);
    return removeCDAfront(items->cda);
}

void *peekQUEUE(QUEUE *items) {
    assert(sizeCDA(items->cda) > 0);
    return getCDA(items->cda, 0);
}

void displayQUEUE(QUEUE *items, FILE *fp) {
    if (items->debug == 1) {
        debugCDA(items->cda, 0);
        displayCDA(items->cda, fp);
        return;
    }
    if (items->debug == 2) {
        debugCDA(items->cda, 1);
        displayCDA(items->cda, fp);
        return;
    }
    fprintf(fp, "<");
    int i;
    for (i = 0; i < sizeCDA(items->cda); ++i) {
        if (items->display == NULL) {
            fprintf(fp, "@%p", getCDA(items->cda, i));
        }
        else {
            items->display(getCDA(items->cda, i), fp);
        }
        if (i != sizeCDA(items->cda) - 1) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, ">");
}

int debugQUEUE(QUEUE *items, int level) {
    int old = items->debug;
    items->debug = level;
    return old;
}

void freeQUEUE(QUEUE *items) {
    freeCDA(items->cda);
    free(items);
}

int sizeQUEUE(QUEUE *items) {
    return sizeCDA(items->cda);
}
