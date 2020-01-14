#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bst.h"
#include "tnode.h"

typedef struct bst {
    TNODE *root;
    int size;
    int debug;
    void (*display)(void *, FILE *);
    void (*swapper)(TNODE *, TNODE *);
    int (*compare)(void *, void *);
    void (*free)(void *);
}BST;

void swapBST(TNODE *a, TNODE *b) {
    void *temp = getTNODEvalue(a);
    setTNODEvalue(a, getTNODEvalue(b));
    setTNODEvalue(b, temp);
}

BST *newBST(int (*c)(void *, void *)) {
    BST *t = malloc(sizeof(BST));
    assert(t != 0);
    t->root = NULL;
    t->size = 0;
    t->debug = 0;
    t->compare = c;
    t->display = NULL;
    t->free = NULL;
    t->swapper = swapBST;
    return t;
}

void setBSTdisplay(BST *t, void (*d)(void *, FILE *)) {
    t->display = d;
}

void setBSTswapper(BST *t, void (*s)(TNODE *, TNODE *)) {
    t->swapper = s;
}

void setBSTfree(BST *t, void (*f)(void *)) {
    t->free = f;
}

TNODE *getBSTroot(BST *t) {
    return t->root;
}

void setBSTroot(BST *t, TNODE *replacement) {
    t->root = replacement;
    setTNODEparent(replacement, replacement);
}

void setBSTsize(BST *t, int s) {
    t->size = s;
}

TNODE *insertR(BST *t, void *value, TNODE *r) {
    if (t->compare(value, getTNODEvalue(r)) < 0) {
        if (getTNODEleft(r) == NULL) {
            setTNODEleft(r, newTNODE(value, NULL, NULL, r));
            return getTNODEleft(r);
        }
        else return insertR(t, value, getTNODEleft(r));
    }
    else if (t->compare(value, getTNODEvalue(r)) > 0) {
        if (getTNODEright(r) == NULL) {
            setTNODEright(r, newTNODE(value, NULL, NULL, r));
            return getTNODEright(r);
        }
        else return insertR(t, value, getTNODEright(r));
    }
    return r; //hits if already in tree
}

TNODE *insertBST(BST *t, void *value) {
    if (t->size == 0) {
        t->size = 1;
        t->root = newTNODE(value, NULL, NULL, NULL);
        setTNODEparent(t->root, t->root);
        setTNODEdisplay(t->root, t->display);
        setTNODEfree(t->root, t->free);
        return t->root;
    }
    t->size++;
    return insertR(t, value, t->root);
}

void *findR(BST *t, void *key, TNODE *r) {
    if (r == NULL) {
        return NULL;
    }
    if (t->compare(getTNODEvalue(r), key) == 0) { //found
        return getTNODEvalue(r);
    }
    else if (t->compare(getTNODEvalue(r), key) > 0) { //traverse left
        return findR(t, key, getTNODEleft(r));
    }
    else if (t->compare(getTNODEvalue(r), key) < 0) { //traverse right
        return findR(t, key, getTNODEright(r));
    }
    return NULL; //not found
}

void *findBST(BST *t, void *key) {
    return findR(t, key, t->root);
}

TNODE *locateR(BST *t, void *value, TNODE *r) {
    if (r == NULL) {
        return NULL;
    }
    if (t->compare(getTNODEvalue(r), value) == 0) {
        return r;
    }
    else if (t->compare(getTNODEvalue(r), value) > 0) {
        return locateR(t, value, getTNODEleft(r));
    }
    else if (t->compare(getTNODEvalue(r), value) < 0) {
        return locateR(t, value, getTNODEright(r));
    }
    return NULL;
}

TNODE *locateBST(BST *t, void *key) {
    return locateR(t, key, t->root);
}

int deleteBST(BST *t, void *key) {
    if (findBST(t, key) == NULL) {
        return -1;
    }
    TNODE *node = swapToLeafBST(t, locateBST(t, key));
    pruneLeafBST(t, node);
    free(node); //FIXME (maybe just free)
    t->size--;
    return 0;
}

TNODE *swapToLeafBST(BST *t, TNODE *node) {
    if (t->size == 1) return node;
    if (getTNODEleft(node) == NULL && getTNODEright(node) == NULL) return node;
    TNODE *temp = getTNODEleft(node);
    if (temp == NULL) { //successor
        temp = getTNODEright(node);
        while (getTNODEleft(temp) != NULL) temp = getTNODEleft(temp);
        t->swapper(node, temp);
        return swapToLeafBST(t, temp);
    }
    else { //predecessor
        while (getTNODEright(temp) != NULL) temp = getTNODEright(temp);
        t->swapper(node, temp);
        return swapToLeafBST(t, temp);
    }
}

void pruneLeafBST(BST *t, TNODE *leaf) {
    if (t->size == 0) return;
    if (leaf == t->root) t->root = NULL;
    if (leaf == getTNODEleft(getTNODEparent(leaf))) {
        setTNODEleft(getTNODEparent(leaf), NULL);
    }
    if (leaf == getTNODEright(getTNODEparent(leaf))) {
        setTNODEright(getTNODEparent(leaf), NULL);
    }
    setTNODEparent(leaf, NULL);
}

int sizeBST(BST *t) {
    return t->size;
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int maxDepth(BST *t, TNODE *node) {
    if (node == NULL) return 0;
    else {
        int lDepth = maxDepth(t, getTNODEleft(node));
        int rDepth = maxDepth(t, getTNODEright(node));
        return max(lDepth, rDepth) + 1;
    }
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

int minDepth(BST *t, TNODE *node) {
    if (node == NULL) return -1;
    int ldepth = minDepth(t, getTNODEleft(node));
    int rdepth = minDepth(t, getTNODEright(node));
    return (1 + min(rdepth, ldepth));
}

void statisticsBST(BST *t, FILE *fp) {
    if (sizeBST(t) == 0) {
        fprintf(fp, "Nodes: 0\nMinimum depth: -1\nMaximum depth: -1\n");
        return;
    }
    if (sizeBST(t) == 1) {
        fprintf(fp, "Nodes: 1\nMinimum depth: 0\nMaximum depth: 0\n");
        return;
    }
    fprintf(fp, "Nodes: %d\nMinimum depth: %d\nMaximum depth: %d\n", sizeBST(t), minDepth(t, t->root), maxDepth(t, t->root) - 1);
}

void preorder(BST *t, FILE *fp, TNODE *r) {
    if (sizeBST(t) == 0) {
        return;
    }
    if (r == NULL) return;
    if (getTNODEleft(r) != NULL) {
        fprintf(fp, "[");
        preorder(t, fp, getTNODEleft(r));
        fprintf(fp, "] ");
    }
    if (getTNODEright(r) != NULL) {
        fprintf(fp, "[");
        preorder(t, fp, getTNODEright(r));
        fprintf(fp, "] ");
    }
    t->display(getTNODEvalue(r), fp);
}

void inorder(BST *t, FILE *fp, TNODE *r) {
    if (sizeBST(t) == 0) {
        return;
    }
    if (r == NULL) return;
    if (getTNODEleft(r) != NULL) {
        fprintf(fp, "[");
        inorder(t, fp, getTNODEleft(r));
        fprintf(fp, "] ");
    }
    t->display(getTNODEvalue(r), fp);
    fprintf(fp, " ");
    if (getTNODEright(r) != NULL) {
        fprintf(fp, "[");
        inorder(t, fp, getTNODEright(r));
        fprintf(fp, "]");
    }
}

void postorder(BST *t, FILE *fp, TNODE *r) {
    if (sizeBST(t) == 0) {
        return;
    }
    if (r == NULL) return;
    t->display(getTNODEvalue(r), fp);
    fprintf(fp, " ");
    if (getTNODEleft(r) != NULL) {
        fprintf(fp, "[");
        postorder(t, fp, getTNODEleft(r));
        fprintf(fp, "] ");
    }
    if (getTNODEright(r) != NULL) {
        fprintf(fp, "[");
        postorder(t, fp, getTNODEright(r));
        fprintf(fp, "]");
    }
}

void printLevel(BST *t, TNODE *n, int i, FILE *fp) {
    if (n == NULL) return;
    if (i == 1) {
        fprintf(fp, " ");
        if (getTNODEleft(n) == NULL && getTNODEright(n) == NULL) fprintf(fp, "=");
        t->display(getTNODEvalue(n), fp);
        fprintf(fp, "(");
        t->display(getTNODEvalue(getTNODEparent(n)), fp);
        fprintf(fp, ")");
        if (getTNODEparent(n) == n) fprintf(fp, "X");
        if (getTNODEleft(getTNODEparent(n)) == n) fprintf(fp, "L");
        if (getTNODEright(getTNODEparent(n)) == n) fprintf(fp, "R");
    }
    else if (i > 1) {
        printLevel(t, getTNODEleft(n), i - 1, fp);
        printLevel(t, getTNODEright(n), i - 1, fp);
    }
}

void displayBST(BST *t, FILE *fp) {
    if (t->debug == 1) {
        fprintf(fp, "[");
        preorder(t, fp, t->root);
        fprintf(fp, "]");
        return;
    }
    if (t->debug == 2) {
        fprintf(fp, "[");
        inorder(t, fp, t->root);
        fprintf(fp, "]");
        return;
    }
    if (t->debug == 3) {
        fprintf(fp, "[");
        postorder(t, fp, t->root);
        fprintf(fp, "]");
        return;
    }
    if (sizeBST(t) == 0) {
        fprintf(fp, "0:\n");
        return;
    }
    int i;
    for (i = 1; i <= maxDepth(t, t->root); ++i) {
        if (i != 1) fprintf(fp, "\n");
        fprintf(fp, "%d:", i - 1);
        printLevel(t, getBSTroot(t), i, fp);
    }
    if (maxDepth(t, t->root) == 0) {
        fprintf(fp, "0:");
        printLevel(t, getBSTroot(t), 1, fp);
    }
    fprintf(fp, "\n");
}

int debugBST(BST *t, int level) {
    int old = t->debug;
    t->debug = level;
    return old;
}

void freeR(BST *t, TNODE *r) {
    if (r == NULL) return;
    freeR(t, getTNODEleft(r));
    freeR(t, getTNODEright(r));
    if (t->free != NULL) setTNODEfree(r, t->free);
    freeTNODE(r);
}

void freeBST(BST *t) {
    freeR(t, t->root);
    free((BST *)t);
}