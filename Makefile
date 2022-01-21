LIBS = -lhiredis

all: qtt

%.o: %.c
	$(CC) -c -o $@ $<

qtt: qtt.o
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	-rm qtt *.o

