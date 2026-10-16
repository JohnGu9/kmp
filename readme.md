# Intro

A [memory safe] / [for no '\0' end string with known size] / [pure C implement] / [no std/system requirement] / [no VLA] [KMP](https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm) algorithm.

# Requirement

string.h (for size_t/strncmp)

# Example

```c
kmp_find_sub_string(source, source_size, pattern, pattern_size);
```

# Limitation

`kmp_find_sub_string()` doesn't relay on VLA feature. The default KMP table is fixed size. If the pattern size is bigger than the default KMP table size, the performance may decrease (but still work).

`kmp_find_sub_string_ex()` for certain performance when provide a KMP table with the same size of pattern.
