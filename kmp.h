#ifndef KMP_H
#define KMP_H

#include <string.h>

#ifdef KMP_CONFIG
#include "kmp-config.h"
#endif

#ifndef KMP_CHAR_T
#define KMP_CHAR_T char
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief kmp algorithm
   *
   * @param data source string
   * @param size source string size
   * @param pattern pattern string
   * @param pattern_size pattern string size
   * @return the position for the first pattern found in the source string, or NULL if no pattern found in the source string
   */
  const KMP_CHAR_T *kmp_find_sub_string(const KMP_CHAR_T *data /* not nullable */,
                                        const size_t size,
                                        const KMP_CHAR_T *pattern /* not nullable */,
                                        const size_t pattern_size);

  /**
   * @brief a extension version of [kmp_find_sub_string]. Require a custom kmp_table memory.
   */
  const KMP_CHAR_T *kmp_find_sub_string_ex(const KMP_CHAR_T *data /* not nullable */, const size_t size,
                                           const KMP_CHAR_T *pattern /* not nullable */, const size_t pattern_size,
                                           size_t *kmp_table /* not nullable */, const size_t kmp_table_size);

#ifdef __cplusplus
}
#endif

#endif
