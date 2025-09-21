# Busy-Wait π Approximation and Volatile (Chapter 4 Exercise)

This project uses Pthreads programs (`pth_pi_busy1.c`, `pth_pi_busy2.c`) that estimate π using the Maclaurin series which can be downloaded from here: https://www.elsevier.com/books-and-journals/book-companion/9780128046050

```
π = 4 * (1 - 1/3 + 1/5 - 1/7 + 1/9 - …)
```

The programs demonstrate the issues with **busy-wait synchronization** and how compiler optimizations can break correctness.

---

## Busy-Wait Variants

- **Busy1 (`pth_pi_busy1.c`)**  
  Each thread computes its portion of the series and **adds each term** into the global sum under a busy-wait lock. Synchronization occurs on every iteration.

- **Busy2 (`pth_pi_busy2.c`)**  
  Each thread computes a **private partial sum**, then adds it once into the global sum under a busy-wait lock. Synchronization occurs only once per thread.

---

## The Exercise

The compiler does not know the program is multi-threaded. At high optimization levels (`-O2`), it may:
- Keep shared variables in registers.
- Reorder memory operations.

This breaks busy-waiting loops.  

Example snippet:

```c
while (flag != my_rank);  // spin
sum += y;                 // update shared
flag++;
```

If reordered, a thread could update `sum` before waiting, producing incorrect results or deadlock.

---

## Which Variables Should Be `volatile`?

- **`flag`**: Must be `volatile`.  
  Otherwise, the compiler may assume it never changes inside the loop and cache it in a register → threads spin forever.

- **`sum`**: Should also be `volatile`.  
  Without it, the compiler may cache or reorder updates, leading to incorrect results.  
  (**Note**: `volatile` does not fix data races; it only prevents the compiler from hiding updates.)

---

## Experiment Results

1. **Compile with no optimization (`-O0`)**
   ```bash
   gcc -O0 -Wall -o pth_pi_busy1 pth_pi_busy1.c -lm -lpthread
   ./pth_pi_busy1 4 100000
   ```
   - Multi-threaded and single-threaded results agree.

2. **Compile with optimization (`-O2`), without `volatile`**
   ```bash
   gcc -O2 -Wall -o pth_pi_busy1 pth_pi_busy1.c -lm -lpthread
   ./pth_pi_busy1 4 100000
   ```
   - Program may hang (deadlock) or give wrong results.  
   - Caused by compiler caching `flag`.

3. **Compile with optimization (`-O2`), with `volatile`**
   ```c
   volatile int flag;
   volatile double sum;
   ```
   ```bash
   gcc -O2 -Wall -o pth_pi_busy1 pth_pi_busy1.c -lm -lpthread
   ./pth_pi_busy1 4 100000
   ```
   - Program runs correctly again.  
   - Multi-threaded output matches single-threaded output.

---

## Conclusion

- Busy-wait synchronization is fragile under compiler optimization.  
- **Declaring `flag` and `sum` as `volatile` restores correctness.**  
- `volatile` prevents compiler optimizations from breaking the program but does **not** solve data races.  
- The proper long-term fix is to use synchronization primitives such as **mutexes** or **semaphores** instead of busy-waiting.
