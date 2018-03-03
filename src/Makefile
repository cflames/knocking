.PHONY: clean test server client all

all: clean server client

test:
	gcc -o test test_aes.c aes.c -lcrypto
	./test

server:
	gcc -o udp_server udp_server.c aes.c -lcrypto

client:
	gcc -o udp_client udp_client.c aes.c -lcrypto

clean:
	rm -f test udp_client udp_server
