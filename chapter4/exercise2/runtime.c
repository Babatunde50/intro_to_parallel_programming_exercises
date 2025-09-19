#include <stdio.h>
#include <sys/time.h>
#include "initial.h"
#include "solution.h"

double wall_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main()
{
    int m = 6000, n = 6000, threads = 4;
    double start, end;

    start = wall_time();
    run_initial(m, n, threads);
    end = wall_time();
    printf("Shared A/y runtime: %.3f seconds\n", end - start);

    start = wall_time();
    run_distributed(m, n, threads);
    end = wall_time();
    printf("Distributed A/y runtime: %.3f seconds\n", end - start);

    return 0;
}