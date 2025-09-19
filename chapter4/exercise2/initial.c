#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "initial.h"

static int M, N, THREADS;
static double **A;
static double *x;
static double *y;

void *Pth_mat_vect(void *rank);

void run_initial(int m, int n, int thread_count)
{
    M = m;
    N = n;
    THREADS = thread_count;
    A = malloc(M * sizeof(double *));
    for (int i = 0; i < M; i++)
    {
        A[i] = malloc(N * sizeof(double));
        for (int j = 0; j < N; j++)
        {
            A[i][j] = i + j + 1;
        }
    }
    x = malloc(N * sizeof(double));
    y = malloc(M * sizeof(double));
    for (int j = 0; j < N; j++)
        x[j] = 1.0;

    pthread_t threads[THREADS];
    for (long t = 0; t < THREADS; t++)
        pthread_create(&threads[t], NULL, Pth_mat_vect, (void *)t);
    for (int t = 0; t < THREADS; t++)
        pthread_join(threads[t], NULL);

    for (int i = 0; i < M; i++)
        free(A[i]);
    free(A);
    free(x);
    free(y);
}

void *Pth_mat_vect(void *rank)
{
    long my_rank = (long)rank;
    int local_m = M / THREADS;
    int my_first_row = my_rank * local_m;
    int my_last_row = (my_rank + 1) * local_m - 1;

    for (int i = my_first_row; i <= my_last_row; i++)
    {
        y[i] = 0.0;
        for (int j = 0; j < N; j++)
            y[i] += A[i][j] * x[j];
    }
    return NULL;
}