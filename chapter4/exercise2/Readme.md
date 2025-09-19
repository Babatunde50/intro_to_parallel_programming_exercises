## Question

In matrix–vector multiplication, we usually assume that the matrix `A` and the result vector `y` are **shared** among all threads.  
Exercise 4.2 asks:

- What if we instead **distribute** `A` and `y` so each thread only stores its own block?
- How should we initialize these structures?
- How can we schedule the input/output so results are printed correctly?
- How does this distribution affect runtime performance (ignoring I/O)?

---

## Our Solution

We implemented **two versions**:

1. **Shared A/y (baseline)**

   - All threads share a single global `A`, `x`, and `y`.
   - Simple to initialize and print, but may suffer **false sharing** and weaker cache locality.

2. **Distributed A/y (solution)**
   - Only `x` is global (since every row needs the full vector).
   - Each thread allocates its own block of `A` (its rows) and its own local `y`.
   - Results are printed in order using **condition variables** for synchronization.
   - This avoids false sharing and improves cache locality.

Both are exposed as functions (`run_initial()` and `run_distributed()`) and executed from a single driver (`runtime.c`).

---

## Results

We measured runtime on a MacBook Pro using `m = 6000, n = 6000, threads = 4`.  
Each version was executed multiple times.

Example runs:

Shared A/y runtime: ~0.15–0.18 s
Distributed A/y runtime: ~0.07–0.13 s

**Observation:**

- Distributing `A` and `y` improved runtime, roughly **2× faster** on average.
- The improvement comes from avoiding false sharing and giving each thread better cache locality.
- However, distributed code is more complex (extra allocations, output synchronization).

---

## How to Run

```bash
gcc -o runtime runtime.c initial.c solution.c -pthread && ./runtime
```
