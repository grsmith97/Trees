#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "da.h"

typedef struct da {
    void **array;
    int size;
    int capacity;
    int debug;
    void (*display)(void *, FILE *);
    void (*free)(void *);
}DA;

DA *newDA(void) {
    DA *items = malloc(sizeof(DA));
    assert(items != 0);
    items->size = 0;
    items->display = NULL;
    items->free = NULL;
    items->capacity = 1;
    items->debug = 0;
    items->array = malloc(items->capacity * sizeof(void *));
    return items;
}

void setDAdisplay(DA *items, void (*d)(void *, FILE *)) {
    items->display = d;
}

void setDAfree(DA *items, void (*f)(void *)) {
    items->free = f;
}

void insertDA(DA *items, int index, void *value) {
    if (items->size == 0) {
        items->size = 1;
        items->array[0] = value;
        return;
    }
    if (items->size == items->capacity) {
        items->capacity *= 2;
        items->array = realloc(items->array, (items->capacity*sizeof(void *)));
        assert(items->array != 0);
    }
    int i;
    for (i = items->size; i > index; --i) {
        items->array[i] = items->array[i - 1];
    }
    items->array[index] = value;
    items->size++;
}

void *removeDA(DA *items, int index) {
    assert(items->size > 0);
    assert(index >= 0 && index < items->size);
    void *temp = items->array[index];
    if (items->size == 1) {
        items->size = 0;
        items->capacity = 1;
        items->array[0] = NULL;
        items->array = realloc(items->array, sizeof(void *));
        return temp;
    }
    int i;
    for (i = index; i < items->size - 1; ++i) {
            items->array[i] = items->array[i + 1];
    }
    items->size--;
    items->array[items->size] = NULL;
    if (((double)items->size / items->capacity) < ((double)1/4)) {
        items->capacity /= 2;
        if (items->capacity == 0) {
            items->capacity = 1;
        }
        items->array = realloc(items->array, items->capacity*sizeof(void *));
    }
    return temp;
}

void unionDA(DA *recipient, DA *donor) {
    int i;
    for (i = 0; i < donor->size; ++i) {
        insertDA(recipient, sizeDA(recipient), getDA(donor, i));
    }
    donor->capacity = 1;
    donor->size = 0;
}

void *getDA(DA *items, int index) {
    assert(index >= 0 && index < items->size);
    return items->array[index];
}

void *setDA(DA *items, int index, void *value) {
    assert(index >= 0 && index <= items->size);
    if (index == items->size) {
        insertDA(items, index, value);
        return NULL;
    }
    void *tmp = items->array[index];
    items->array[index] = value;
    return tmp;
}

int sizeDA(DA *items) {
    return items->size;
}

void displayDA(DA *items, FILE *fp) {
    if (items->size == 0) {
        if (items->debug > 0) {
            fprintf(fp, "[[1]]");
            return;
        }
        fprintf(fp, "[]");
        return;
    }
    int i;
    fprintf(fp, "[");
    for (i = 0; i < items->size; ++i) {
        if (items->display == NULL) {
            fprintf(fp, "@%p", items->array[i]);
        }
        else {
            items->display(items->array[i], fp);
        }
        if (i < items->size - 1) {
            fprintf(fp, ",");
        }
        else {
            if (items->debug > 0) {
                fprintf(fp, ",[%d]", items->capacity - items->size);
            }
            fprintf(fp, "]");
        }
    }
}

int debugDA(DA *items, int level) {
    int old = items->debug;
    items->debug = level;
    return old;
}

void freeDA(DA *items) {
    int i = 0;
    for (i = 0; i < items->size; ++i) {
        if (items->free != NULL) {
            items->free(items->array[i]);
        }
    }
    free(items->array);
    free((DA *)items);
}
