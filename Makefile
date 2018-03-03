.PHONY: clean test

test:
	gcc -o test test_aes.c aes.c -lcrypto
	./test
clean:
	rm -f test
