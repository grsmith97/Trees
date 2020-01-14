#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tnode.h"

typedef struct tnode {
    void *value;
    TNODE *left, *right, *parent;
    int debug;
    void (*display)(void *value, FILE *fp);
    void (*free)(void *value);
}TNODE;

extern TNODE *newTNODE(void *v, TNODE *l, TNODE *r, TNODE *p) {
    TNODE *newNode = malloc(sizeof(TNODE));
    assert(newNode != 0);
    newNode->value = v;
    newNode->left = l;
    newNode->right = r;
    newNode->parent = p;
    newNode->debug = 0;
    newNode->display = NULL;
    newNode->free = NULL;
    return newNode;
}

extern void setTNODEdisplay(TNODE *n, void (*d)(void *, FILE *)) {
    n->display = d;
}

extern void setTNODEfree(TNODE *n, void (*f)(void *)) {
    n->free = f;
}

extern void *getTNODEvalue(TNODE *n) {
    return n->value;
}

extern void setTNODEvalue(TNODE *n, void *replacement) {
    n->value = replacement;
}

extern TNODE *getTNODEleft(TNODE *n) {
    return n->left;
}

extern void setTNODEleft(TNODE *n, TNODE *replacement) {
    n->left = replacement;
}

extern TNODE *getTNODEright(TNODE *n) {
    return n->right;
}

extern void setTNODEright(TNODE *n, TNODE *replacement) {
    n->right = replacement;
}

extern TNODE *getTNODEparent(TNODE *n) {
    return n->parent;
}

extern void setTNODEparent(TNODE *n, TNODE *replacement) {
    n->parent = replacement;
}

extern void displayTNODE(TNODE *n, FILE *fp) {
    if (n->display != NULL) {
        n->display(n->value, fp);
        if (n->debug == 0) {
            return;
        }
    }
    fprintf(fp, "&%p", n->value);
}

extern int debugTNODE(TNODE *n, int level) {
    int old = n->debug;
    n->debug = level;
    return old;
}

extern void freeTNODE(TNODE *n) {
    if (n->free != NULL) n->free(n->value);
    free(n);
}
