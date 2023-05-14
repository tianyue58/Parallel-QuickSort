#include <stdio.h>
#include <omp.h>

#define MAXSIZE 1000000
#define MAXTHREADS 10

int size = MAXSIZE;
int vector[MAXSIZE];
int numThreads;
double start_time, end_time;

void quickSort(int first, int last)
{
    int pivot, index, temp, l, r;

    if (last <= first)
        return;

    index = (first + last) / 2;
    pivot = vector[index];
    l = first;
    r = last;
    while (l <= r)
    {
        if (vector[l] > pivot)
        {
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

    temp = vector[r];
    vector[index] = temp;
    vector[r] = pivot;

#pragma omp task
    quickSort(first, (r - 1));
#pragma omp task
    quickSort((register + 1), last);
}

int main(int argc, char *argv[])
{
    int i;

    size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
    if (size <= 0 || size > MAXSIZE)
        size = MAXSIZE;

    numThreads = (argc > 2) ? atoi(argv[2]) : MAXTHREADS;
    if (numThreads > MAXTHREADS)
        numThreads = MAXTHREADS;

    omp_set_num_threads(numThreads);

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

    for (i = 0; i < size - 1; i++)
        if (vector[i] > vector[i + 1])
        {
            printf("Error: Not sorted!\n");
        }
#ifdef DEBUG
    printf("Sorted vector: \n");
    for (i = 0; i < size; i++)
        printf(" %3d", vector[i]);
    printf("\n");
#endif

    printf("Quicksort takes %g seconds\n", end_time - start_time);
}
