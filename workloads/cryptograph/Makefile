CC=gcc
CFLAGS=-Wall -Wextra
LDLIBS=-lcrypto
TARGETS=rsa_test aes_test

.PHONY:all
all: $(TARGETS)

ras_test: rsa_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

aes_test: aes_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY:clean
clean:
	rm -rf $(TARGETS)
