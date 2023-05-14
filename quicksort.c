#include <stdio.h>
#include <omp.h>

#define MAXSIZE 1000000 /* Maximum size of array*/
#define MAXWORKERS 10   /* Maximum amount of worker threads */

int size = MAXSIZE;
int vector[MAXSIZE];
double start_time, end_time; /* start and end times */
int numWorkers;

/* Regular quiksort algorithm, with the only exception that
 * the recursive step is done in parallel with openmp tasks
 */
void quickSort(int first, int last)
{
    int pivot, index, temp, l, r;
    if (first >= last)
        return; // no need to sort
    // otherwise select a pivot
    index = (first + last) / 2;
    pivot = vector[index];
    l = first;
    r = last;
    while (l <= r)
    {
        if (vector[l] > pivot)
        { // swap left element with right element
            temp = vector[l];
            vector[l] = vector[r];
            vector[r] = temp;
            if (r == index)
            {
                index = l;
            }
            r--;
        }
        else
        {
            l++;
        }
    }
    // place the pivot in its place (i.e. swap with right element)
    temp = vector[r];
    vector[index] = temp;
    vector[r] = pivot;

// sort two sublists in parallel;

/* The recursive steps in quicksort execution is implemented as separate tasks */
#pragma omp task
    quickSort(first, (r - 1));
#pragma omp task
    quickSort((register + 1), last);
}

int main(int argc, char *argv[])
{
    int i;

    /* determine size */
    size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
    if (size <= 0 || size > MAXSIZE)
        size = MAXSIZE;

    numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
    if (numWorkers > MAXWORKERS)
        numWorkers = MAXWORKERS;

    omp_set_num_threads(numWorkers);

    /* initialize and print the vector to be sorted */
    for (i = 0; i < size; i++)
        vector[i] = (int)random() % MAXSIZE;
#ifdef DEBUG
    printf("initial vector: \n");
    for (i = 0; i < size; i++)
        printf(" %3d", vector[i]);
    printf("\n");
#endif

    start_time = omp_get_wtime();

/* call quickSort  */
/* The sorting is done in a parallel region */
#pragma omp parallel
    {
/* But we only want to sort the list once, so the first call
 * to quickSort is done only once thanks to the single parameter
 */
#pragma omp single
        quickSort(0, (size - 1));
    }

    end_time = omp_get_wtime();

    /* check if the vector is sorted and print the sorted vector */
    for (i = 0; i < size - 1; i++)
        if (vector[i] > vector[i + 1])
        {
            printf("The resulting vector is not sorted!\n");
            // return(1);
        }
#ifdef DEBUG
    printf("sorted vector: \n");
    for (i = 0; i < size; i++)
        printf(" %3d", vector[i]);
    printf("\n");
#endif

    printf("It took %g seconds\n", end_time - start_time);
}
