## Exercise 4.1

**Question**  
When we discussed matrix-vector multiplication, we assumed that both _m_ (rows) and _n_ (columns) were evenly divisible by _t_, the number of threads. How do the formulas for the assignments change if this is not the case?

---

**Answer**

In Section 4.3/4.4 of the book, when _m_ and _n_ are evenly divisible by _t_, the row range for thread `q` is given by:

\[
\text{first}(q) = \frac{q \times m}{t}, \quad \text{last}(q) = \frac{(q+1) \times m}{t} - 1
\]

So for example, if _m = 6_ and _t = 2_:

- Thread 0: rows 0–2
- Thread 1: rows 3–5

---

When _m_ is **not divisible by t**, we fix this by distributing the remainder:

1. Compute:
   \[
   \text{chunk} = \lfloor m/t \rfloor, \quad r = m \bmod t
   \]
   where `chunk` is the base number of rows per thread, and `r` is the remainder.

2. Assign:
   - Threads with `q < r` each get `chunk + 1` rows.
   - Threads with `q ≥ r` each get `chunk` rows.

The general formulas become:

\[
\text{first}(q) = q \cdot \text{chunk} + \min(q, r)
\]
\[
\text{last}(q) = \text{first}(q) + \big(\text{chunk} + \mathbf{1}[q<r]\big) - 1
\]

---

**Example: m = 7, t = 2**

- `chunk = floor(7/2) = 3`
- `r = 7 mod 2 = 1`

- Thread 0: `first = 0`, `last = 3` → rows 0–3
- Thread 1: `first = 4`, `last = 6` → rows 4–6

So thread 0 handles 4 rows, and thread 1 handles 3 rows. The extra row (the remainder) went to the first thread.

---
