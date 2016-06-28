CC      = gcc
RM      = rm -f

default: code

code: sem.c philosopher.c
	$(CC) -pthread -o code sem.c philosopher.c
	
clean:
	rm -f core *.o code
