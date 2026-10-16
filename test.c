#include "kmp.h"

#include <assert.h>
#include <stdio.h>

int main()
{
    const char test_pattern[] = "ABCDABD";
    //                              position: 15th
    //                                         | 
    const char test_source[] = "ABC ABCDAB ABCDABCDABDE";

    const char *c = kmp_find_sub_string(test_source, strlen(test_source), test_pattern, strlen(test_pattern));

    assert((c - test_source) == 15);

    printf("%p %p\n", test_source, c);
    printf("%.*s\n", (int)strlen(test_pattern), c);

    return 0;
}
