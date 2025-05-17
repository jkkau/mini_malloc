all:
	gcc -Wall -Werror -I./ *.c -o test_malloc

test:
	./test_malloc

clean:
	rm test_malloc