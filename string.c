#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "string.h"

struct STRING {
    char *value;
};

STRING *newSTRING(char *x) {
    STRING *s = malloc(sizeof(STRING));
    assert(s != 0);
    s->value = x;
    return s;
}

char *getSTRING(STRING *s) {
    return s->value;
}

void setSTRING(STRING *s, char *x) {
    s->value = x;
}

void displaySTRING(void *v, FILE *fp) {
    fprintf(fp, "%s", getSTRING((STRING *) v));
}

int compareSTRING(void *v, void *w) {
    return strcmp(getSTRING(w), getSTRING(v)); //maybe switch the order??
}

void freeSTRING(void *v) {
    STRING *s = v;
    free(s->value);
    free(s);
}
