#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "cda.h"
#include "queue.h"
#include "tnode.h"
#include "bst.h"
#include "gst.h"
#include "rbt.h"
#include "scanner.h"

char type;
char *fileName = 0, *fileName2 = 0;

static int compareSTRING(void *a, void *b);
static void displaySTRING(void *value, FILE *fp);
static void freeSTRING(void *value);
static RBT *makeRBT();
static GST *makeGST();
static void commandRBT(void *t, char command, char *str);
static void commandGST(void *t, char command, char *str);
static char *cleanString(char *str);
static void readStream(void *t);


int main(int argc,char **argv) {
	int i;
	if (argc <= 1) exit(0);
	for (i = 1; i <= argc - 1; i++) {
    	if (strcmp(argv[i], "-v") == 0) {
            printf("Written by Glenn Smith\n");
            return 0;
        }
	    else if (strcmp(argv[i], "-g") == 0) type = 'g';
	    else if (strcmp(argv[i], "-r") == 0) type = 'r';
	    else if (i > 1) {
		    fileName = argv[i];
		    fileName2 = argv[i + 1];
		    break;
	    }
    }	
    if (type == 'g') {
        GST *t = makeGST();
        readStream((void *)t);
		freeGST(t);
    }
    else {
		type = 'r';
        RBT *t = makeRBT();
        readStream((void *)t);
		freeRBT(t);
    }
    return 0;
}

RBT *makeRBT() {
	RBT *t = newRBT(compareSTRING);
	setRBTdisplay(t, displaySTRING);
	setRBTfree(t, freeSTRING);
	FILE *fp = fopen(fileName, "r");
	char *s, *str;
	while (!feof(fp)) {
		if (stringPending(fp)) {
			s = readString(fp);
			if (s == 0) break;
			str = cleanString(s);
		}
		else {
		    s = readToken(fp);
		    if (s == 0) break;
		    str = cleanString(s);
		}
		if (str[0] != '\0' && str[0] != ' ') {
			insertRBT(t, str);
		}
	}
	fclose(fp);
	return t;
}

GST *makeGST() {
	GST *t = newGST(compareSTRING);
	setGSTdisplay(t, displaySTRING);
	setGSTfree(t, freeSTRING);
	FILE *fp = fopen(fileName, "r");
	char *s, *str;
	while (!feof(fp)) {
		if (stringPending(fp)) {
			s = readString(fp);
			if (s == 0) break;
			str = cleanString(s);
		}
		else {
			s = readToken(fp);
			if (s == 0) break;
			str = cleanString(s);
		}
		if (str[0] != '\0' && str[0] != ' ') {
			insertGST(t, str);
		}
	}
	fclose(fp);
	return t;
}

int compareSTRING(void *a, void *b) 
{
	char *s1 = a, *s2 = b;
	return strcmp(s1, s2);
}

void displaySTRING(void *value, FILE *fp) 
{
	char *v = value;
	fprintf(fp, "%s", v);
}

void freeSTRING(void *value) 
{
	free(value);
}

void commandRBT(void *t, char command, char *str) {
	RBT *r = t;
	char *a = "";
	switch (command) {
    case 'i':
		insertRBT(r, str);
		break;
    case 'd':
		if (freqRBT(r, str) > 1)
			deleteRBT(r, str);
		else if (freqRBT(r, str) == 1) {
			a = findRBT(r, str);
			setRBTfree(r, 0);
			deleteRBT(r, a);
			setRBTfree(r, freeSTRING);
			freeSTRING(str);
		}
		else {
			printf("Value %s not found.\n", str);
        }
        break;
    case 'f':
		fprintf(stdout, "Frequency of %s: %d\n", str, freqRBT(r, str));
		break;
    case 'r':
		statisticsRBT(r, stdout);
		break;
    case 's':
		if (sizeRBT(r) > 0) displayRBT(r, stdout);
		else fprintf(stdout, "EMPTY\n");
		break;
	}
}

void commandGST(void *t, char command, char *str) {
	GST *r = t;
	char *a = "";
    switch (command) {
		case 'i':
			insertGST(r, str);
			break;
		case 'd':
			if (freqGST(r, str) > 1) deleteGST(r, str);
            else if (freqGST(r, str) == 1) {
			    a = findGST(r, str);
    			deleteGST(r, a);
	    		freeSTRING(str);
            }
            else printf("Value %s not found.\n", str);
			break;
		case 'f':
			fprintf(stdout, "Frequency of %s: %d\n", str, freqGST(r, str));
			break;
		case 'r':
			statisticsGST(r, stdout);
			break;
		case 's':
			if (sizeGST(r) > 0) displayGST(r, stdout);
            else fprintf(stdout, "EMPTY\n");
		break;
    }
}

void readStream(void *t) {
	FILE *fp = fopen(fileName2, "r");
	char *str, *s;
	char command = '\0';

	while (!feof(fp)) {
		if (stringPending(fp)) {
			s = readString(fp);
			if (s == 0) break;
			str = cleanString(s);
			if (str[0] == '\0' || str[0] == ' ') {
				command = '\0';
				str[0] = '\0';
				continue;
			}
			if (type == 'r') commandRBT(t, command, str);
			else commandGST(t, command, str);
			command = '\0';
		}
		else {
			s = readToken(fp);
			if (s == 0)	break;
			if ((strlen(s) == 1) &&  (command == '\0') && (s[0] == 'i' || s[0] == 'd' || s[0] == 'f')) {
				command = s[0];
				continue;
			}
			if ((strlen(s) == 1) &&  (command == '\0') && (s[0] == 's' || s[0] == 'r')) command = s[0];
			else {
				str = cleanString(s);
				if (str[0] == '\0' || str[0] == ' ') {
					command = '\0';
					str[0] = '\0';
					continue;
				}
			}
			if (type == 'r') commandRBT(t, command, str);
			else commandGST(t, command, str);
			command = '\0';
		}
	}
	fclose(fp);
}

char *cleanString(char *str) {
	if (!str) return NULL;
	unsigned int i = 0;
	unsigned int j = 0;
	while (i < strlen(str)) {
	    while (i < strlen(str) && !isalpha(str[i])) ++i;
    	while (i < strlen(str) && isalpha(str[i])) str[j++] = tolower(str[i++]);
	    while (i < strlen(str) && !isalpha(str[i]) && !isspace(str[i])) ++i;
	    if (i < strlen(str) && isspace(str[i])) {
            ++i; 
            str[j++] = ' ';
        }
    }
	str[j] = '\0';
	if (j > 0 && isspace(str[j - 1])) str[j - 1] = '\0';
    return str;
}