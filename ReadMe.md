# Setup:
The zip contains the code files- philosopher.c, sem.c, sem.h and the make file-Makefile, in order to
compile the code. 

# Steps to follow:
* To compile: $ make
* To run: $ ./code <number of philosophers> <number of iterations>
For Example: ./code 5 2
* To clean: $ make clean

# Details of implementation:
The code creates a number of philosopher processes that work in order to solve the dining philosopher
problem.
The synchronization among the processes was achieved using barrier, mutexes and semaphores. These
were implemented with the help of the Mmap function. Using the mmap function, a file was mapped to
the memory of each process and a semaphore structure containing sharable semaphore and sharable
conditional variable, was written into the file.
* The file sem.h contains the data structure implementation used to implement the barrier, mutexes and
semaphores (one for each philosopher process).
* The file sem.c contains the function definition of the that implements the data structures defined
above.
* At the end of the program, all the process exit after all the operations are complete.

# References:
1. https://computing.llnl.gov/tutorials/pthreads/man/pthread_mutexattr_init.txt
2. https://www.eecis.udel.edu/~bmiller/cis361/2011f/slides/Classics.pdf
