#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cda.h"

typedef struct cda {
    void **array;
    int start;
    int end;
    int size;
    int capacity;
    int debug;
    void (*display)(void *, FILE *);
    void (*free)(void *);
}CDA;

int incrementIndex(CDA *items, int i) {
    if (i == items->capacity - 1) {
        return 0;
    }
    return i + 1;
}

int decrementIndex(CDA *items, int i) {
    if (i == 0) {
        return items->capacity - 1;
    }
    return i - 1;
}

int correctIndex(CDA *items, int i) {
    if (i < 0) {
        return i + items->capacity;
    }
    if (i >= items->capacity) {
        return i - items->capacity;
    }
    return i;
}

void endsCDA(CDA *items) {
    printf("items->start = %d\nitems->end = %d\n", items->start, items->end);
}

CDA *newCDA(void) {
    CDA *items = malloc(sizeof(CDA));
    assert(items != 0);
    items->size = items->start = items->end = items->debug = 0;
    items->capacity = 1;
    items->display = NULL;
    items->free = NULL;
    items->array = malloc(items->capacity * sizeof(void *));
    return items;
}

void setCDAdisplay(CDA *items, void (*d)(void *, FILE *)) {
    items->display = d;
}

void setCDAfree(CDA *items, void (*f)(void *)) {
    items->free = f;
}

void insertCDA(CDA *items, int index, void *value) {
    int i;
    if (items->size == 0) {
        items->array[0] = value;
    }
    else if (items->capacity == items->size) {
        void **temp = malloc((items->capacity * 2) * sizeof(void *));
        assert(temp != 0);
        for (i = 0; i < items->size; ++i) {
            if (i < index) {
                temp[i] = items->array[correctIndex(items, items->start + i)];
            }
            else if (i == index) {
                temp[i] = value;
                temp[i + 1] = items->array[correctIndex(items, items->start + i)];
            }
            else {
                temp[i + 1] = items->array[correctIndex(items, items->start + i)];
            }
        }
        if (index == items->size) {
            temp[items->size] = value;
        }
        items->capacity *= 2;
        free(items->array);
        items->array = temp;
        items->start = 0;
        items->end = items->size;
    }
    else if (index == 0) {
        items->start = decrementIndex(items, items->start);
        items->array[items->start] = value;
    }
    else if (index == items->size) {
        items->end = incrementIndex(items, items->end);
        items->array[items->end] = value;
    }
    else if (index > items->size / 2) {
        for (i = items->size; i > index; --i) {
            items->array[correctIndex(items, items->start + i)] = items->array[correctIndex(items, items->start + i - 1)];
        }
        items->array[correctIndex(items, items->start + index)] = value;
        items->end = incrementIndex(items, items->end);
    }
    else {
        for (i = 0; i < index; ++i) {
            items->array[correctIndex(items, items->start + i - 1)] = items->array[correctIndex(items, items->start + i)];
        }
        items->start = decrementIndex(items, items->start);
        items->array[correctIndex(items, items->start + index)] = value;
    }
    items->size++;
}

void *removeCDA(CDA *items, int index) {
    assert(items->size > 0);
    void *temp = getCDA(items, index);
    int i;
    if (((double)(items->size - 1) / items->capacity) < ((double)0.25)) { 
        if (items->size == 1) {
            items->start = items->end = items->size = 0;
            items->array = realloc(items->array, sizeof(void *));
            items->capacity = 1;
            return temp;
        }
        void **tmp = malloc((items->capacity / 2)*sizeof(void *));
        for (i = 0; i < items->size; ++i) {
            if (i < index) {
                tmp[i] = items->array[correctIndex(items, items->start + i)];
            }
            if (i > index) {
                tmp[i - 1] = items->array[correctIndex(items, items->start + i)];
            }
        }
        items->start = 0;
        items->size--;
        items->end = items->size - 1;
        items->capacity /= 2;
        free(items->array);
        items->array = tmp;
        if (items->capacity == 0) {
            items->capacity = 1;
        }
        return temp;
    }
    if (items->size == 1) {
        items->array[items->start] = NULL;
        items->size = items->start = items->end = 0;
        items->capacity = 1;
        items->array = realloc(items->array, sizeof(void *));
        return temp;
    }
    else if (index == 0) {
        items->array[items->start] = NULL;
        items->start = incrementIndex(items, items->start);
        items->size--;
        return temp;
    }
    else if (index == items->size - 1) {
        items->array[items->end] = NULL;
        items->end = decrementIndex(items, items->end);
        items->size--;
        return temp;
    }
    else if (index > items->size / 2) {
        for (i = correctIndex(items, items->start + index); i != correctIndex(items, items->end + 1); i = incrementIndex(items, i)) {
            items->array[i] = items->array[incrementIndex(items, i)];
        }
        items->size--;
        items->end = decrementIndex(items, items->end);
        return temp;
    }
    else {
        for (i = index; i > 0; --i) {
            items->array[correctIndex(items, items->start + i)] = items->array[correctIndex(items, items->start + i - 1)];
        }
        items->start = incrementIndex(items, items->start);
        items->size--;
        return temp;
    }
    return temp;
}

void unionCDA(CDA *recipient, CDA *donor) {
    int i;
    for (i = 0; i < donor->size; ++i) {
        insertCDAback(recipient, donor->array[correctIndex(donor, donor->start + i)]);
    }
    donor->capacity = 1;
    donor->size = donor->start = donor->end = 0;
}

void *getCDA(CDA *items, int index) {
    assert(index >= 0 && index < items->size);
    return items->array[correctIndex(items, items->start + index)];
}

void *setCDA(CDA *items, int index, void *value) {
    assert(index >= -1 && index <= items->size);
    if (index == items->size) {
        insertCDAback(items, value);
        return NULL;
    }
    if (index == -1) {
        insertCDAfront(items, value);
        return NULL;
    }
    void *tmp = getCDA(items, index);
    items->array[correctIndex(items, items->start + index)] = value;
    return tmp;
}

int sizeCDA(CDA *items) {
    return items->size;
}

void displayCDA(CDA *items, FILE *fp) {
    if (items->size == 0) {
        if (items->debug > 0) {
            fprintf(fp, "((1))");
            return;
        }
        fprintf(fp, "()");
        return;
    }
    int i;
    fprintf(fp, "(");
    for (i = 0; i < items->size; i++) {
        if (items->display == NULL) {
            fprintf(fp, "@%p", items->array[correctIndex(items, items->start + i)]);
        }
        else {
            items->display(items->array[correctIndex(items, items->start + i)], fp);
        }
        if (i < items->size - 1) {
            fprintf(fp, ",");
        }
        else {
            if (items->debug > 0) {
                fprintf(fp, ",(%d)", items->capacity - items->size);
            }
            fprintf(fp, ")");
        }
    }
}

int debugCDA(CDA *items, int level) {
    int old = items->debug;
    items->debug = level;
    return old;
}

void freeCDA(CDA *items) {
    if (sizeCDA(items) == 0) {
        free((void **)items->array);
        free((CDA *) items);
        return;
    }
    int i;
    for (i = 0; i < sizeCDA(items) - 1; ++i) {
        if (items->free != NULL) {            
            items->free(items->array[correctIndex(items, items->start + i)]);
        }
    }
    items->free(items->array[items->end]);
    free(items->array);
    free((CDA *)items);
}
