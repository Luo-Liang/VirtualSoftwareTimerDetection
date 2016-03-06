CC=gcc
CFLAGS=-Wall -Wextra
LDLIBS=-lcrypto
TARGETS=rsa_test aes_test hmac_test des_test mix

.PHONY:all
all: $(TARGETS)

ras_test: rsa_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

aes_test: aes_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

hmac_test: hmac_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

des_test: des_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

mix: mix.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY:clean
clean:
	rm -rf $(TARGETS)
