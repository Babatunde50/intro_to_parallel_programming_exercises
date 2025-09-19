#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "solution.h"

static int M, N, THREADS;
static double *X; // shared x vector

pthread_mutex_t print_mutex;
pthread_cond_t print_cond;
int next_thread_to_print = 0;

void *Pth_mat_vect_print(void *rank);
void ordered_print(long my_rank, double *local_y, int local_m);

void run_distributed(int m, int n, int thread_count)
{
    M = m;
    N = n;
    THREADS = thread_count;
    X = malloc(N * sizeof(double));
    for (int j = 0; j < N; j++)
        X[j] = 1.0;

    pthread_mutex_init(&print_mutex, NULL);
    pthread_cond_init(&print_cond, NULL);

    pthread_t threads[THREADS];
    for (long t = 0; t < THREADS; t++)
    {
        pthread_create(&threads[t], NULL, Pth_mat_vect_print, (void *)t);
    }
    for (int t = 0; t < THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&print_mutex);
    pthread_cond_destroy(&print_cond);

    free(X);
    next_thread_to_print = 0; // reset for reuse
}

void *Pth_mat_vect_print(void *rank)
{
    long my_rank = (long)rank;
    int local_m = M / THREADS;
    int my_first_row = my_rank * local_m;

    // Allocate local A and y
    double *local_A = malloc(local_m * N * sizeof(double));
    double *local_y = malloc(local_m * sizeof(double));

    // Initialize local A
    for (int i = 0; i < local_m; i++)
    {
        int global_row = my_first_row + i;
        for (int j = 0; j < N; j++)
        {
            local_A[i * N + j] = global_row + j + 1;
        }
    }

    // Compute local y
    for (int i = 0; i < local_m; i++)
    {
        local_y[i] = 0.0;
        for (int j = 0; j < N; j++)
        {
            local_y[i] += local_A[i * N + j] * X[j];
        }
    }

    // Ordered print (optional)
    // commented out for runtime experiment
    // ordered_print(my_rank, local_y, local_m);

    free(local_A);
    free(local_y);
    return NULL;
}


void ordered_print(long my_rank, double *local_y, int local_m)
{
    pthread_mutex_lock(&print_mutex);

    while (next_thread_to_print != my_rank)
    {
        pthread_cond_wait(&print_cond, &print_mutex);
    }

    for (int i = 0; i < local_m; i++)
    {
        printf("%.1f ", local_y[i]);
    }

    next_thread_to_print++;
    pthread_cond_broadcast(&print_cond);
    pthread_mutex_unlock(&print_mutex);
}