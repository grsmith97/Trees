OBJS = integer.o bst.o tnode.o test-bst.o gst.o rbt.o main.o string.o 
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g

all : trees

trees : main.o bst.o gst.o rbt.o tnode.o string.o scanner.o
		gcc $(LOPTS) main.o bst.o gst.o rbt.o tnode.o string.o scanner.o -o trees

test-bst : test-bst.o integer.o tnode.o bst.o 
		gcc $(LOPTS) test-bst.o integer.o tnode.o bst.o -o test-bst

bst-0-0 : bst-0-0.o integer.o tnode.o bst.o 
		gcc $(LOPTS) bst-0-0.o integer.o tnode.o bst.o -o bst-0-0

gst-0-0 : gst-0-0.o integer.o tnode.o bst.o gst.o
		gcc $(LOPTS) gst-0-0.c integer.o tnode.o bst.o gst.o -o gst-0-0

rbt-0-0 : rbt-0-0.o integer.o tnode.o bst.o gst.o rbt.o
		gcc $(LOPTS) rbt-0-0.c integer.o tnode.o bst.o gst.o rbt.o -o rbt-0-0

rbt-4-4 : rbt-4-4.o integer.o tnode.o bst.o gst.o rbt.o
		gcc $(LOPTS) rbt-4-4.c integer.o tnode.o bst.o gst.o rbt.o -o rbt-4-4

rbt-4-6 : rbt-4-6.o integer.o tnode.o bst.o gst.o rbt.o
		gcc $(LOPTS) rbt-4-6.c integer.o tnode.o bst.o gst.o rbt.o -o rbt-4-6

rbt-4-7 : rbt-4-7.o integer.o tnode.o bst.o gst.o rbt.o
		gcc $(LOPTS) rbt-4-7.c integer.o tnode.o bst.o gst.o rbt.o -o rbt-4-7

main.o : main.c integer.h bst.h gst.h rbt.h tnode.h scanner.h string.h
		gcc $(OOPTS) main.c

scanner.o : scanner.c scanner.h
		gcc $(OOPTS) scanner.c

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

string.o : string.c string.h
		gcc $(OOPTS) string.c

tnode.o : tnode.c tnode.h
		gcc $(OOPTS) tnode.c

bst.o : bst.c bst.h
		gcc $(OOPTS) bst.c

gst.o : gst.c gst.h
		gcc $(OOPTS) gst.c

rbt.o : rbt.c rbt.h
		gcc $(OOPTS) rbt.c

bst-0-0.o : bst-0-0.c bst.h integer.h
		gcc $(OOPTS) bst-0-0.c

gst-0-0.o : gst.h bst.h integer.h gst-0-0.c
		gcc $(OOPTS) gst-0-0.c

rbt-0-0.o : bst.h gst.h rbt.h integer.h rbt-0-0.c
		gcc $(OOPTS) rbt-0-0.c

rbt-4-4.o : bst.h gst.h rbt.h integer.h rbt-4-4.c
		gcc $(OOPTS) rbt-4-4.c

rbt-4-6.o : bst.h gst.h rbt.h integer.h rbt-4-6.c
		gcc $(OOPTS) rbt-4-6.c

rbt-4-7.o : bst.h gst.h rbt.h integer.h rbt-4-7.c
		gcc $(OOPTS) rbt-4-7.c

test : all

valgrind : all

clean	:
		rm -f $(OBJS) *.o trees
