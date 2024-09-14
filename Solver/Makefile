all: solver

solver: solver.o
	gcc -o $@ $^

solver.o: solver.c solver.h
	gcc -c $<

clean:
	rm -f solver *.o

