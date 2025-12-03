// Copyright (c) 2025 @drclcomputers. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>

// struct definition
typedef struct {
    size_t size;
    uint64_t *data;
} cbitset;

// Create/Free bitset
cbitset *create_bitset(size_t bits); 
void free_bitset(cbitset *bt);

// Checks bit exist
static inline void ensure_index(cbitset *bt, size_t bit);

// Single bit operations
void set_bitset(cbitset *bt, size_t bit);
void clear_bitset(cbitset *bt, size_t bit);
void toggle_bitset(cbitset *bt, size_t bit);
int get_bitset(cbitset *bt, size_t bit);

// Multi bit operations
size_t count_bitset(cbitset *bt);
void reset_all_bitset(cbitset *bt);
void set_all_bitset(cbitset *bt);
void flip_all_bitset(cbitset *bt);

// multi bitset operations
void and_bitset(cbitset *a, cbitset *b);
void or_bitset(cbitset *a, cbitset *b);
void xor_bitset(cbitset *a, cbitset *b);
void not_bitset(cbitset *bt);

// string output/input
char *bitset_to_string(cbitset *bt);
cbitset *bitset_from_string(const char *str);

// copy and compare
cbitset *copy_bitset(const cbitset *src);
int equal_bitset(const cbitset *a, const cbitset *b);
int not_equal_bitset(const cbitset *a, const cbitset *b);

// shifting (in-place)
void shl_bitset(cbitset *bt, size_t shift);   // shift left
void shr_bitset(cbitset *bt, size_t shift);   // shift right

// // utility functions
int bitset_any(const cbitset *bt);          // returns 1 if any bit is set
int bitset_none(const cbitset *bt);         // returns 1 if no bit is set
int bitset_all(const cbitset *bt);          // returns 1 if all bits are set
int bitset_find_first(const cbitset *bt);       // index of first set bit, -1 if none
int bitset_find_next(const cbitset *bt, size_t prev_index); // next set bit after prev_index, -1 if none

