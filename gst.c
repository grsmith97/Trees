#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "gst.h"
#include "bst.h"
#include "tnode.h"

typedef struct gst {
    BST *t;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*swapper)(TNODE *, TNODE *);
    void (*free)(void *);
    int duplicates;
}GST;

typedef struct gstvalue {
    GST *g;
    void *value;
    int freq;
}GSTVALUE;

void swapGST(TNODE *a, TNODE *b) {
    void *x = getTNODEvalue(a);
    void *y = getTNODEvalue(b);
    setTNODEvalue(a, y);
    setTNODEvalue(b, x);
}

void *getGNODEvalue(GSTVALUE *gn) {
    return gn->value;
}

int getGNODEfreq(GSTVALUE *gn) {
    return gn->freq;
}

void setGNODEvalue(GSTVALUE *gn, void *a) {
    gn->value = a;
}

void setGNODEfreq(GSTVALUE *gn, int b) {
    gn->freq = b;
}

int compareGSTVAL(void *a, void *b) {
    GSTVALUE *x = a;
    GSTVALUE *y = b;
    return x->g->compare(getGNODEvalue(x), getGNODEvalue(y));
}

void displayGSTVAL(void *a, FILE *fp) {
    GSTVALUE *x = a;
    x->g->display(getGNODEvalue(x), fp);
    if (getGNODEfreq(x) > 1) fprintf(fp, "<%d>", getGNODEfreq(x));
}

void freeGSTVAL(void *a) {
    GSTVALUE *x = a;
    if (x->g->free != NULL) x->g->free(x->value);
    free(a);
}

GST *newGST(int (*c)(void *, void *)) {
    GST *g = malloc(sizeof(GST));
    g->t = newBST(compareGSTVAL);
    setBSTdisplay(g->t, displayGSTVAL);
    setBSTfree(g->t, freeGSTVAL);
    setBSTswapper(g->t, swapGST);
    g->duplicates = 0;
    g->display = NULL;
    g->swapper = swapGST;
    g->free = NULL;
    g->compare = c;
    return g;
}

void setGSTdisplay(GST *g, void (*d)(void *, FILE *)) {
    g->display = d;
}

void setGSTswapper(GST *g, void (*s)(TNODE *, TNODE *)) {
    g->swapper = s;
    setBSTswapper(g->t, s);
}

void setGSTfree(GST *g, void (*f)(void *)) {
    g->free = f;
}

TNODE *getGSTroot(GST *g) {
    return getBSTroot(g->t);
}

void setGSTroot(GST *g, TNODE *replacement) {
    setBSTroot(g->t, replacement);
}

void setGSTsize(GST *g, int s) {
    setBSTsize(g->t, s);
}

TNODE *insertGST(GST *g, void *value) {
    GSTVALUE *gstval = malloc(sizeof(GSTVALUE));
    gstval->value = value;
    gstval->g = g;
    if (sizeBST(g->t) > 0 && findBST(g->t, gstval)) {
        TNODE *temp = locateBST(g->t, gstval);
        setGNODEfreq(getTNODEvalue(temp), getGNODEfreq(getTNODEvalue(temp)) + 1);
        g->duplicates++;
        freeGSTVAL(gstval);
        return NULL;
    }
    gstval->freq = 1;
    return insertBST(g->t, gstval);
}

void *findGST(GST *g, void *value) {
    GSTVALUE *gn = malloc(sizeof(GSTVALUE));
    gn->value = value;
    gn->g = g;
    void *n = findBST(g->t, gn);
    free(gn);
    if (n == NULL) return NULL;
    return getGNODEvalue(n);
}

TNODE *locateGST(GST *g, void *value) {
    GSTVALUE *gn = malloc(sizeof(GSTVALUE));
    gn->value = value;
    gn->g = g;
    TNODE *n = locateBST(g->t, gn);
    free(gn);
    return n;
}

int deleteGST(GST *g, void *value) {
    GSTVALUE *gstval = malloc(sizeof(GSTVALUE));
    gstval->value = value;
    gstval->g = g;
    if (findBST(g->t, gstval) == NULL) {
        free(gstval);
        return -1;
    }
    GSTVALUE *temp = findBST(g->t, gstval);
    free(gstval);
    if (getGNODEfreq(temp) > 1) {
        setGNODEfreq(temp, getGNODEfreq(temp) - 1);
        g->duplicates--;
        return getGNODEfreq(temp);
    }
    else {
        deleteBST(g->t, temp);
        free(temp);
        return 0;
    }
}

TNODE *swapToLeafGST(GST *g, TNODE *node) {
    return swapToLeafBST(g->t, node);
}

void pruneLeafGST(GST *g, TNODE *leaf) {
    free(getTNODEvalue(leaf)); //freeGSTVAL or free?
    pruneLeafBST(g->t, leaf);
}

int sizeGST(GST *g) {
    return sizeBST(g->t);
}

void statisticsGST(GST *g, FILE *fp) {
    fprintf(fp, "Duplicates: %d\n", g->duplicates);
    statisticsBST(g->t, fp);
}

void displayGST(GST *g, FILE *fp) {
    displayBST(g->t, fp);
}

int debugGST(GST *g, int level) {
    return debugBST(g->t, level);
}

void freeGST(GST *g) {
    freeBST(g->t);
    free(g);
}

void *unwrapGST(TNODE *n) {
    return getGNODEvalue(getTNODEvalue(n));
}

int freqGST(GST *g, void *key) {
    GSTVALUE *gn = malloc(sizeof(GSTVALUE));
    gn->value = key;
    gn->g = g;
    if (findBST(g->t, gn) == NULL) {
        free(gn);
        return 0;
    }
    GSTVALUE *x = findBST(g->t, gn);
    int n = x->freq;
    free(gn);
    return n;
}

int duplicatesGST(GST *g) {
    return g->duplicates;
}