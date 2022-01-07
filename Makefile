FLAGS = -Wall -pedantic-errors

compile:
	gcc -o head $(FLAGS) head.c
	gcc -o tail $(FLAGS) tail.c
	gcc -o true $(FLAGS) true.c
	gcc -o false $(FLAGS) false.c
	gcc -o wc $(FLAGS) wc.c
	gcc -o env $(FLAGS) env.c

clean:
	rm -f head
	rm -f tail
	rm -f true
	rm -f false
	rm -f wc
	rm -f env
