all: qtt

%.o: %.c
	$(CC) ${CFLAGS} -c -o $@ $< ${LDFLAGS}

qtt: qtt.o
	$(CC) ${CFLAGS} -o $@ $^ ${LDFLAGS}

.PHONY: clean
clean:
	-rm qtt *.o

