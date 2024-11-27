CC = clang
CFLAGS = -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -Werror -pedantic

.PHONY: all clean format test
all: pqtest nodetest brtest bwtest huff dehuff

pqtest: pq.o pqtest.o node.o
	$(CC) $(CFLAGS) pq.o pqtest.o node.o -o pqtest

pq.o: pq.c pq.h node.h
	$(CC) $(CFLAGS) -c pq.c

pqtest.o: pqtest.c pq.h node.h
	$(CC) $(CFLAGS) -c pqtest.c

nodetest: node.o nodetest.o
	$(CC) $(CFLAGS) node.o nodetest.o -o nodetest

nodetest.o: nodetest.c node.h
	$(CC) $(CFLAGS) -c nodetest.c

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

brtest: bitreader.o brtest.o
	$(CC) $(CFLAGS) bitreader.o brtest.o -o brtest

huff: huff.o pq.o node.o bitreader.o bitwriter.o
	$(CC) $(CFLAGS) huff.o pq.o node.o bitreader.o bitwriter.o -o huff

huff.o: huff.c pq.h node.h bitreader.h bitwriter.h
	$(CC) $(CFLAGS) -c huff.c

dehuff: dehuff.o pq.o node.o bitreader.o bitwriter.o
	$(CC) $(CFLAGS) dehuff.o pq.o node.o bitreader.o bitwriter.o -o dehuff

dehuff.o: dehuff.c pq.h node.h bitreader.h bitwriter.h
	$(CC) $(CFLAGS) -c dehuff.c


bitreader.o: bitreader.c bitreader.h
	$(CC) $(CFLAGS) -c bitreader.c

brtest.o: brtest.c bitreader.h
	$(CC) $(CFLAGS) -c brtest.c

bwtest: bitwriter.o bwtest.o
	$(CC) $(CFLAGS) bitwriter.o bwtest.o -o bwtest

bitwriter.o: bitwriter.c bitwriter.h
	$(CC) $(CFLAGS) -c bitwriter.c

bwtest.o: bwtest.c bitwriter.h
	$(CC) $(CFLAGS) -c bwtest.c


format:
	clang-format -i --style=file *.[ch]

clean:
	rm -f *.o pqtest nodetest brtest bwtest huff dehuff
