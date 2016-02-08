CC=gcc
CFLAGS=-Wall -Wextra
LDLIBS=-lcrypto

.PHONY:all
all: rsa_test

ras_test: rsa_test.c
	$(CC) $(CFLAGS) $(LIBS) $@ $^ $(LDLIBS)

.PHONY:clean
clean:
	rm -rf rsa_test
