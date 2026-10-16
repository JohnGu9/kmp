#include "kmp.h"

static size_t _get_longest_prefix_suffix(const KMP_CHAR_T *pattern, size_t pattern_size)
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

static void _kmp_table_init(KMP_SIZE_T *kmp_table, size_t kmp_table_size,
                            const KMP_CHAR_T *pattern, size_t pattern_size)
{
  const size_t size = MIN(kmp_table_size, pattern_size);
  for (size_t i = 0; i < size; i++)
  {
    kmp_table[i] = (KMP_SIZE_T)_get_longest_prefix_suffix(pattern, i + 1);
  }
}

static size_t _get_first_diff_char_position(const KMP_CHAR_T *data, const KMP_CHAR_T *pattern, size_t pattern_size)
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
                                         KMP_SIZE_T *kmp_table, const size_t kmp_table_size)
{
  if (pattern_size > 0 && pattern_size <= size)
  {
    // kmp algorithm
    _kmp_table_init(kmp_table, kmp_table_size, pattern, pattern_size);

    size_t compare_start = 0;
    for (size_t i = 0; i <= (size - pattern_size);)
    {
      size_t first_diff_char_position =
          _get_first_diff_char_position(data + i + compare_start, pattern + compare_start, pattern_size - compare_start) + compare_start;
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
        KMP_SIZE_T longest_prefix_suffix =
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
  KMP_SIZE_T kmp_table[KMP_TABLE_SIZE];
  return kmp_find_sub_string_ex(data, size, pattern, pattern_size, kmp_table, KMP_TABLE_SIZE);
}

#if KMP_AUTO == 1

#ifndef KMP_AUTO_MODE
#define KMP_AUTO_MODE KMP_AUTO_MODE_FIXED
#endif

#define KMP_AUTO_MODE_FIXED 0
#define KMP_AUTO_MODE_VLA 1
#define KMP_AUTO_MODE_HEAP 2

#if KMP_AUTO_MODE == KMP_AUTO_MODE_HEAP
#include <stdlib.h>
#endif

const KMP_CHAR_T *find_sub_string(const KMP_CHAR_T *data, const size_t size,
                                  const KMP_CHAR_T *pattern, const size_t pattern_size)
{
  if (size < pattern_size)
  {
    return NULL;
  }
  switch (pattern_size)
  {
  case 0:
    return NULL;
  case 1:
  {
    for (size_t i = 0; i < size; i++)
    {
      if (data[i] == pattern[0])
      {
        return data + i;
      }
    }
    return NULL;
  }
  case 2:
  {
    for (size_t i = 0; i <= (size - pattern_size); i++)
    {
      if (strncmp(data + i, pattern, pattern_size) == 0)
      {
        return data + i;
      }
    }
    return NULL;
  }
#if KMP_AUTO_MODE != KMP_AUTO_MODE_VLA
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  {
    KMP_SIZE_T kmp_table[8];
    return kmp_find_sub_string_ex(data, size, pattern, pattern_size, kmp_table, pattern_size);
  }
#endif
  default:
  {
#if KMP_AUTO_MODE == KMP_AUTO_MODE_VLA
    KMP_SIZE_T kmp_table[pattern_size];
    return kmp_find_sub_string_ex(data, size, pattern, pattern_size, kmp_table, pattern_size);
#elif KMP_AUTO_MODE == KMP_AUTO_MODE_HEAP
    KMP_SIZE_T *kmp_table = (KMP_SIZE_T *)malloc(sizeof(KMP_SIZE_T) * pattern_size);
    const char *result = kmp_find_sub_string_ex(data, size, pattern, pattern_size, kmp_table, pattern_size);
    free(kmp_table);
    return result;
#else // KMP_AUTO_MODE_FIXED
    return kmp_find_sub_string(data, size, pattern, pattern_size);
#endif
  }
  }
}
#endif
