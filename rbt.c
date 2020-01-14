#include <stdio.h>
#include <stdlib.h>

#include "tnode.h"
#include "rbt.h"
#include "gst.h"
#include "bst.h"
#include "assert.h"

typedef struct rbt {
    GST *g;
    void (*display)(void *, FILE *);
    void (*swapper)(TNODE *, TNODE *);
    int (*compare)(void *, void *);
    void (*free)(void *);
}RBT;

typedef struct rbtvalue {
    void *value;
    RBT *r;
    char color;
}RBTVALUE;

void setRNODEvalue(RBTVALUE *a, void *b) {
    a->value = b;
}

void setRNODEcolor(RBTVALUE *a, char color) {
    a->color = color;
}

void *getRNODEvalue(RBTVALUE *a) {
   return a->value;
}

char getRNODEcolor(RBTVALUE *a) {
    if (a == NULL) return 'b';
    return a->color;
}

char getTNODEcolor(TNODE *a) {
    if (a == NULL) return 'b';
    if (((RBTVALUE *) unwrapGST(a))->color == 'b') return 'b';
    return 'r';
}

int compareRBTVAL(void *a, void *b) {
    RBTVALUE *x = a;
    RBTVALUE *y = b;
    return x->r->compare(getRNODEvalue(x), getRNODEvalue(y));
}

void displayRBTVAL(void *a, FILE *fp) {
    RBTVALUE *x = a;
    x->r->display(getRNODEvalue(x), fp);
    if (getRNODEcolor(x) == 'r') fprintf(fp, "*");
}

void freeRBTVAL(void *a) {
    RBTVALUE *x = a;
    if (x->r->free != NULL) x->r->free(x->value);
    free(a);
}

void swapRBTVAL(TNODE *a, TNODE *b) {
    void *va = getTNODEvalue(a);
    void *vb = getTNODEvalue(b);
    setTNODEvalue(a, vb);
    setTNODEvalue(b, va);
    RBTVALUE *x = unwrapGST(a);
    RBTVALUE *y = unwrapGST(b);
    char color = x->color;
    x->color = y->color;
    y->color = color;
}

RBT *newRBT(int (*c)(void *, void *)) {
    RBT *r = malloc(sizeof(RBT));
    assert(r != NULL);
    r->g = newGST(compareRBTVAL);
    setGSTdisplay(r->g, displayRBTVAL);
    setGSTswapper(r->g, swapRBTVAL);
    setGSTfree(r->g, freeRBTVAL);
    r->display = NULL;
    r->swapper = swapRBTVAL;
    r->free = NULL;
    r->compare = c;
    return r;
}

void setRBTdisplay(RBT *r, void (*d)(void *, FILE *)) {
    r->display = d;
}

void setRBTswapper(RBT *r, void (*s)(TNODE *, TNODE *)) {
    r->swapper = s;
    setGSTswapper(r->g, s);
}

void setRBTfree(RBT *r, void (*f)(void *)) {
    r->free = f;
}

TNODE *getRBTroot(RBT *r) {
    return getGSTroot(r->g);
}

void setRBTroot(RBT *r, TNODE *replacement) {
    setGSTroot(r->g, replacement);
}

void setRBTsize(RBT *r, int s) {
    setGSTsize(r->g, s);
}

void RightRotate(RBT *r, TNODE *x) {
    TNODE *y = getTNODEleft(x);
    TNODE *p = getTNODEparent(x);
    setTNODEleft(x, getTNODEright(y));
    if (getTNODEright(y) != NULL) setTNODEparent(getTNODEright(y), x);
    setTNODEparent(y, p);
    if (getTNODEparent(x) == x) setRBTroot(r, y);
    else if (x == getTNODEright(p)) setTNODEright(p, y);
    else setTNODEleft(p, y);
    setTNODEright(y, x);
    setTNODEparent(x, y);
}

void LeftRotate(RBT *r, TNODE *x) {
    TNODE *y = getTNODEright(x);
    TNODE *p = getTNODEparent(x);
    setTNODEright(x, getTNODEleft(y));
    if (getTNODEleft(y) != NULL) setTNODEparent(getTNODEleft(y), x);
    setTNODEparent(y, p);
    if (getTNODEparent(x) == x) setRBTroot(r, y);
    else if (x == getTNODEleft(p)) setTNODEleft(p, y);
    else setTNODEright(p, y);
    setTNODEleft(y, x);
    setTNODEparent(x, y);
}

TNODE *getUncle(TNODE *a) {
    if (a && getTNODEparent(a) && getTNODEparent(getTNODEparent(a))) {
        if (getTNODEleft(getTNODEparent(getTNODEparent(a))) == getTNODEparent(a)) {
            return getTNODEright(getTNODEparent(getTNODEparent(a)));
        }
        else {
            return getTNODEleft(getTNODEparent(getTNODEparent(a)));
        } 
    }
    return NULL;
}

TNODE *getSibling(TNODE *a) {
    if (a && getTNODEparent(a) && getTNODEright(getTNODEparent(a)) && getTNODEleft(getTNODEparent(a))) {
        if (a == getTNODEright(getTNODEparent(a))) return getTNODEleft(getTNODEparent(a));
        else return getTNODEright(getTNODEparent(a));
    }
    return NULL;
}

TNODE *getNephew(TNODE *a) {
    if (a == NULL) return NULL;
    if (getTNODEparent(a) == a) return NULL;
    if (getSibling(a)) {
        if (a == getTNODEright(getTNODEparent(a))) return getTNODEleft(getSibling(a));
        else return getTNODEright(getSibling(a));
    }
    return NULL;
}

TNODE *getNeice(TNODE *a) {
 	TNODE *s = getSibling(a);
	if (s == 0) return NULL;
	if (a == getTNODEleft(getTNODEparent(a))) {
		return getTNODEleft(s);
	}
	return getTNODEright(s);
}

int LinearHelper(TNODE *x, TNODE *parent) {
    if (!parent) return 1;
    if (getTNODEleft(parent) == x)
        if (getTNODEleft(getTNODEparent(parent)) == parent)
            return 1;
    if (getTNODEright(parent) == x)
        if (getTNODEright(getTNODEparent(parent)) == parent)
            return 1;
    return 0;
}

void insertionFixup(RBT *r, TNODE *x) {
    while(1) {
        if (x == getGSTroot(r->g)) break; //if x is root leave loop
        TNODE *p = getTNODEparent(x); //set a to x->parent
        if (getTNODEcolor(p) == 'b') break; //if a is black leave loop
        if (getUncle(x) != NULL && getTNODEcolor(getUncle(x)) == 'r') { //if uncle's red
            setRNODEcolor(unwrapGST(p), 'b'); //color parent black
            setRNODEcolor(unwrapGST(getUncle(x)), 'b'); //color uncle black
            setRNODEcolor(unwrapGST(getTNODEparent(p)), 'r'); //color grandparent red
            x = getTNODEparent(p); //set x to grandparent
        }
        else {
            // uncle must be black
            if (LinearHelper(x, p) == 0) { // x and parent are not linear
                if (getTNODEleft(p) == x) RightRotate(r, p); //rotate x to parent
                else LeftRotate(r, p);
                TNODE *temp = x; //swap x and a
                x = p;
                p = temp;
            }
            setRNODEcolor(unwrapGST(p), 'b'); //color parent black
            setRNODEcolor(unwrapGST(getTNODEparent(p)), 'r'); //color grandparent red
            if (getTNODEleft(getTNODEparent(p)) && getTNODEleft(getTNODEparent(p)) == p) RightRotate(r, getTNODEparent(p)); //rotate parent to grandparent
            else LeftRotate(r, getTNODEparent(p));
            break;
        }
    }
    setRNODEcolor(unwrapGST(getGSTroot(r->g)), 'b');
}

TNODE *insertRBT(RBT *r, void *value) {
    RBTVALUE *rval = malloc(sizeof(RBTVALUE));
    rval->value = value;
    rval->color = 'r';
    rval->r = r;
    int num = sizeRBT(r);
    TNODE *z = insertGST(r->g, rval);
    if (sizeRBT(r) != num) insertionFixup(r, z);
    return z;
}

void *findRBT(RBT *r, void *key) {
    RBTVALUE *rval = malloc(sizeof(RBTVALUE));
    rval->value = key;
    rval->r = r;
    void *n = findGST(r->g, rval);
    free(rval);
    if (n == NULL) return NULL;
    return getRNODEvalue(n); //check return 
}

TNODE *locateRBT(RBT *r, void *key) {
    RBTVALUE *rn = malloc(sizeof(RBTVALUE));
    rn->value = key;
    rn->r = r;
    TNODE *n = locateGST(r->g, rn);
    free(rn);
    return n;
}

void deletionFixUp(RBT *r, TNODE *x) {
    while (1) {
        if (x == getGSTroot(r->g)) break;
        if (getTNODEcolor(x) == 'r') break;
        if (getTNODEcolor(getSibling(x)) == 'r') {
            //printf("sibling is red\n");
            setRNODEcolor(unwrapGST(getTNODEparent(x)), 'r');
            setRNODEcolor(unwrapGST(getSibling(x)), 'b');
            if (getTNODEleft(getTNODEparent(x)) == x) LeftRotate(r, getTNODEparent(x));
            else RightRotate(r, getTNODEparent(x));
            //should have black sibling now
        }
        else if (getTNODEcolor(getNephew(x)) == 'r') {
            //printf("newphew is red\n"); //delete
            setRNODEcolor(unwrapGST(getSibling(x)), getTNODEcolor(getTNODEparent(x)));
            setRNODEcolor(unwrapGST(getTNODEparent(x)), 'b');
            setRNODEcolor(unwrapGST(getNephew(x)), 'b');
            if (getTNODEleft(getTNODEparent(x)) == x) LeftRotate(r, getTNODEparent(x));
            else RightRotate(r, getTNODEparent(x));
            //subtree and tree is BH balanced
            break;
        }
        else if (getTNODEcolor(getNeice(x)) == 'r') {
            //printf("niece is red\n");
            //nephew must be black
            setRNODEcolor(unwrapGST(getNeice(x)), 'b');
            setRNODEcolor(unwrapGST(getSibling(x)), 'r');
            if (getNeice(x) == getTNODEleft(getSibling(x))) RightRotate(r, getSibling(x));
            else LeftRotate(r, getSibling(x));
            //should have a red nephew now
        }
        else {
            //printf("sibling, niece, and nephew all black\n");
            if (getSibling(x)) setRNODEcolor(unwrapGST(getSibling(x)), 'r');
            x = getTNODEparent(x);
            //this subtree is BH balanced, but the tree is not
        }
    }
    setRNODEcolor(unwrapGST(x), 'b');
}

int deleteRBT(RBT *r, void *key) {
    TNODE *z = locateRBT(r, key);
    if (z == NULL) {
        return -1;
    }
    if (sizeGST(r->g) == 1 || freqGST(r->g, unwrapGST(z)) > 1) return deleteGST(r->g, unwrapGST(z));
    z = swapToLeafGST(r->g, z);
    void *a = unwrapGST(z);
    deletionFixUp(r, z);   
    pruneLeafGST(r->g, z);
    setGSTsize(r->g, sizeGST(r->g) - 1);
    free(a);
    free(z);
    return 0;
}

TNODE *swapToLeafRBT(RBT *r, TNODE *node) {
    return swapToLeafGST(r->g, node);
}

void pruneLeafRBT(RBT *r, TNODE *leaf) {
    free(unwrapGST(leaf));
    pruneLeafGST(r->g, leaf);
}

int sizeRBT(RBT *r) {
    return sizeGST(r->g);
}

void statisticsRBT(RBT *r, FILE *fp) {
    statisticsGST(r->g, fp);
}

void displayRBT(RBT *r, FILE *fp) {
    displayGST(r->g, fp);
}

int debugRBT(RBT *r, int level) {
    return debugGST(r->g, level);
}

void freeRBT(RBT *r) {
    freeGST(r->g);
    free(r);
}

void *unwrapRBT(TNODE *n) {
    return getRNODEvalue(unwrapGST(n));
}

int freqRBT(RBT *r, void *key) {
    RBTVALUE *rval = malloc(sizeof(RBTVALUE));
    rval->value = key;
    rval->r = r;
    int num = freqGST(r->g, rval);
    free(rval);
    return num;
}

int duplicatesRBT(RBT *r) {
    return duplicatesGST(r->g);
}