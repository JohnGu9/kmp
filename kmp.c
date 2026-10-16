#include "kmp.h"

static size_t get_longest_prefix_suffix(const KMP_CHAR_T *pattern, size_t pattern_size)
{
  for (size_t i = 1; i < pattern_size; i++)
  {
    if (strncmp(pattern, pattern + i, pattern_size - i) == 0)
    {
      return pattern_size - i;
    }
  }
  return 0;
}

#ifndef MIN
#define MIN(a, b) (a) < (b) ? (a) : (b)
#endif

static void kmp_table_init(size_t *kmp_table, size_t kmp_table_size,
                           const KMP_CHAR_T *pattern, size_t pattern_size)
{
  const size_t size = MIN(kmp_table_size, pattern_size);
  for (size_t i = 0; i < size; i++)
  {
    kmp_table[i] = get_longest_prefix_suffix(pattern, i + 1);
  }
}

static size_t get_first_diff_char_position(const KMP_CHAR_T *data, const KMP_CHAR_T *pattern, size_t pattern_size)
{
  for (size_t i = 0; i < pattern_size; i++)
  {
    if (data[i] != pattern[i])
    {
      return i;
    }
  }
  return pattern_size;
}

const KMP_CHAR_T *kmp_find_sub_string_ex(const KMP_CHAR_T *data, const size_t size,
                                         const KMP_CHAR_T *pattern, const size_t pattern_size,
                                         size_t *kmp_table, const size_t kmp_table_size)
{
  if (pattern_size > 0 && pattern_size <= size)
  {
    // kmp algorithm
    kmp_table_init(kmp_table, kmp_table_size, pattern, pattern_size);

    size_t compare_start = 0;
    for (size_t i = 0; i <= (size - pattern_size);)
    {
      size_t first_diff_char_position =
          get_first_diff_char_position(data + i + compare_start, pattern + compare_start, pattern_size - compare_start) + compare_start;
      if (first_diff_char_position == pattern_size)
      {
        return data + i;
      }
      else if (first_diff_char_position == 0)
      {
        i++;
        compare_start = 0;
      }
      else // 0 < first_diff_position < pattern_size
      {
        size_t longest_prefix_suffix =
            kmp_table[MIN(first_diff_char_position - 1, kmp_table_size - 1)];
        i += (first_diff_char_position - longest_prefix_suffix);
        compare_start = longest_prefix_suffix;
      }
    }
  }
  return NULL;
}

#ifndef KMP_TABLE_SIZE
#define KMP_TABLE_SIZE (sizeof(size_t) * 16)
#endif

const KMP_CHAR_T *kmp_find_sub_string(const KMP_CHAR_T *data, const size_t size,
                                      const KMP_CHAR_T *pattern, const size_t pattern_size)
{
  size_t kmp_table[KMP_TABLE_SIZE];
  return kmp_find_sub_string_ex(data, size, pattern, pattern_size, kmp_table, KMP_TABLE_SIZE);
}
