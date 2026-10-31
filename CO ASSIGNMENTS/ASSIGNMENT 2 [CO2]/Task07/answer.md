# Q7 - Diagnose, redesign and validate

## A. Limitation in Q2

Q2 finds the largest mark by scanning the entire register. One lookup costs O(n). If the result is requested repeatedly while marks stay unchanged, the same n values are compared again and again. For n=10,000 and 1,000 queries, this performs about 10,000,000 comparisons.

## B. Measurable objective

Keep the highest-mark result correct while making repeated read-only highest-mark queries O(1), and compare both designs using the same 10,000-mark data and 1,000 queries. The redesigned class must also remain correct when a mark changes.

## C. Revised design

`CachedMarkRegister` owns a copy of the marks and stores the index of the current maximum. Construction scans once (O(n)). A query returns the stored index in O(1). Updating a non-maximum mark is O(1); raising a mark above the maximum is O(1); lowering the current maximum requires a rescan in O(n). The design uses O(1) extra index storage in addition to the mark array. It deliberately does not substitute a library algorithm for the design decision.

```text
CachedMarkRegister
  - marks: vector<int>
  - highestIndex: size_t
  + highestIndex(): size_t
  + update(index, newMark): void
  - refreshHighest(): void
```

The old reference-based result can still be useful for one lookup and immediate moderation. The cached index is preferable when there are many read-only queries between updates. A returned index survives vector reallocation as a number, though its meaning must be checked if elements are inserted, removed, or reordered.

## D. Prediction before the comparison run

With 10,000 marks and 1,000 repeated queries, I expect the scan version to examine about 10 million mark values. The cached version should perform one O(n) initialization and then 1,000 O(1) lookups. Both versions should report the same maximum index and checksum.

## E. Implementation and validation

`q7_cached_highest.cpp` runs the same deterministic input against the scan and cache designs. It lowers the current maximum in a small correctness test and confirms that the cached index is recomputed. For the timed workload, both data sets update the same non-maximum entry before each query, so the scan call cannot be moved out of the loop by optimization. Both checksums are compared. This run took 6,496 microseconds for the scan and 2 microseconds for the cached queries; both indices were 5,000 and both checksums were 5,000,000. These timings exclude cache construction and should be treated as measurements of this particular run, not as universal results.

## F. Evaluation

The objective is met when both checksums and the final indices match and the measured repeated-query time is lower for the cache. The cache costs one extra index and makes updates to the current maximum slower because it rescans. If marks change often or only one query is needed, the original scan is simpler and may be preferable. Timing measurements exclude construction and are affected by system load; construction still costs O(n), so the gain depends on repeated queries between changes.
