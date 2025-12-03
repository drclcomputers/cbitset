#include "cbitset.h"
#include <stdio.h>

int main() {
    cbitset *a = bitset_from_string("110101");
    cbitset *b = bitset_from_string("011011");

    or_bitset(a, b);
    char *s = bitset_to_string(a);
    printf("a | b  = %s\n", s);
    free(s);

    shl_bitset(a, 3);
    s = bitset_to_string(a);
    printf("<< 3   = %s\n", s);
    free(s);

    printf("First set bit: %d\n", bitset_find_first(a));
    printf("Next set bit : %d\n", bitset_find_next(a, 4));

    free_bitset(a);
    free_bitset(b);
    return 0;
}
