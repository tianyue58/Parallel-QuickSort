# Parallel-QuickSort
Implement Parallel QuickSort using OpenMP

## COMPILE
    gcc -fopenmp -o quicksort quicksort.c
	
## RUN
    ./quicksort [size] [numThreads]
    
    If no parameters are passed in, the program will run with default parameters 
    where [size] = 1000000 and [numThreads] = 10.
