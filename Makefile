all: qtt

%.o: %.c
	$(CC) ${CFLAGS} -c -o $@ $<

qtt: qtt.o
	$(CC) ${CFLAGS} ${LDFLAGS} -o $@ $^

.PHONY: clean
clean:
	-rm qtt *.o

