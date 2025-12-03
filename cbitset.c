// Copyright (c) 2025 @drclcomputers. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include <assert.h>
#include "cbitset.h"

// Checks if a bit index is within the limit
static inline void ensure_index(cbitset *bt, size_t bit) {
    assert((bit < bt->size) && "Bit index exceeds bitset size!");
}

static inline void normalize_bitset(cbitset *bt) {
    size_t extra = bt->size % 64;
    if (extra != 0) {
        size_t chunks = (bt->size + 63) / 64;
        bt->data[chunks - 1] &= ((uint64_t)1 << extra) - 1;
    }
}

// Creates a new cbitset
cbitset *create_bitset(size_t bits) {
    cbitset *bt = (cbitset *)malloc(sizeof(cbitset));
    assert(bt && "malloc failed");

    bt->size = bits;
    size_t chunks = (bits + 63) / 64;

    bt->data = (uint64_t *)calloc(chunks, sizeof(uint64_t));
    assert(bt->data && "calloc failed");

    return bt;
}

// Frees the bitset from memory
void free_bitset(cbitset *bt) {
    free(bt->data);
    free(bt);
}

// Sets a certain bit to 1
void set_bitset(cbitset *bt, size_t bit) {
    ensure_index(bt, bit);
    bt->data[bit / 64] |= (uint64_t)1 << (bit % 64);
}

// Sets a certain bit 0
void clear_bitset(cbitset *bt, size_t bit) {
    ensure_index(bt, bit);
    bt->data[bit / 64] &= ~((uint64_t)1 << (bit % 64));
}

// Flips a certain bit
void toggle_bitset(cbitset *bt, size_t bit) {
    ensure_index(bt, bit);
    bt->data[bit / 64] ^= (uint64_t)1 << (bit % 64);
}

// Returns a certain bit value
int get_bitset(cbitset *bt, size_t bit) {
    ensure_index(bt, bit);
    return (bt->data[bit / 64] >> (bit % 64)) & 1;
}

// Counts the bits of 1
size_t count_bitset(cbitset *bt) {
    size_t chunks = (bt->size + 63) / 64;
    size_t total = 0;
    for (size_t i = 0; i < chunks; i++) {
        total += __builtin_popcountll(bt->data[i]);
    }
    return total;
}

// Sets all bits to 0
void reset_all_bitset(cbitset *bt) {
    size_t chunks = (bt->size + 63) / 64;
    memset(bt->data, 0, chunks * sizeof(uint64_t));
}

// Sets all bits to 1
void set_all_bitset(cbitset *bt) {
    size_t chunks = (bt->size + 63) / 64;
    memset(bt->data, 0xFF, chunks * sizeof(uint64_t));
    
    size_t extra = bt->size % 64;
    if (extra != 0) {
        uint64_t mask = ((uint64_t)1 << extra) - 1;
        bt->data[chunks - 1] = mask;
    }
}

// Flips all bits
void flip_all_bitset(cbitset *bt) {
    size_t chunks = (bt->size + 63) / 64;
    for (size_t i = 0; i < chunks; i++) {
        bt->data[i] = ~bt->data[i];
    }
    size_t extra = bt->size % 64;
    if (extra != 0) {
        bt->data[chunks-1] &= ((1ULL << extra) - 1);
    }
}

// AND operation
void and_bitset(cbitset *a, cbitset *b) {
    assert(a->size == b->size && "bitset size mismatch in and");
    size_t chunks = (a->size + 63) / 64;
    for (size_t i = 0; i < chunks; i++)
        a->data[i] &= b->data[i];
}

// OR operation
void or_bitset(cbitset *a, cbitset *b) {
    assert(a->size == b->size && "bitset size mismatch in or");
    size_t chunks = (a->size + 63) / 64;
    for (size_t i = 0; i < chunks; i++)
        a->data[i] |= b->data[i];
}

// XOR operation
void xor_bitset(cbitset *a, cbitset *b) {
    assert(a->size == b->size && "bitset size mismatch in xor");
    size_t chunks = (a->size + 63) / 64;
    for (size_t i = 0; i < chunks; i++)
        a->data[i] ^= b->data[i];
}

// NOT operation
void not_bitset(cbitset *bt) {
    flip_all_bitset(bt); 
}

// Turns the bitset to a char array
char *bitset_to_string(cbitset *bt) {
    char *s = malloc(bt->size + 1);
    assert(s && "malloc failed");

    for (size_t i = 0; i < bt->size; i++)
        s[bt->size - 1 - i] = get_bitset(bt, i) ? '1' : '0';

    s[bt->size] = '\0';
    return s;
}

// Turns a char array into a bitset
cbitset *bitset_from_string(const char *str) {
    assert(str && "Input string is NULL");

    size_t len = 0;
    while (str[len]) len++;

    cbitset *bt = create_bitset(len);

    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if (c == '1') {
            set_bitset(bt, len - 1 - i);
        } else if (c != '0') {
            free_bitset(bt);
            assert(0 && "Invalid character in bitset string");
        }
    }

    normalize_bitset(bt);

    return bt;
}

// Copies bitset
cbitset *copy_bitset(const cbitset *src) {
    cbitset *cpy = malloc(sizeof(*cpy));
    assert(cpy && "malloc failed");

    cpy->size = src->size;
    size_t chunks = (src->size + 63) / 64;

    cpy->data = malloc(chunks * sizeof(uint64_t));
    assert(cpy->data && "malloc failed");

    memcpy(cpy->data, src->data, chunks * sizeof(uint64_t));

    return cpy;
}

// Checks if equal
int equal_bitset(const cbitset *a, const cbitset *b) {
    if (a->size != b->size) return 0;

    size_t chunks = (a->size + 63) / 64;
    return memcmp(a->data, b->data, chunks * sizeof(uint64_t)) == 0;
}

// Checks if not equal
int not_equal_bitset(const cbitset *a, const cbitset *b) {
    return !equal_bitset(a, b);
}

// Shifts left <<
void shl_bitset(cbitset *bt, size_t shift) {
    if (shift == 0 || bt->size == 0) return;

    size_t chunks = (bt->size + 63) / 64;
    size_t word_shift = shift / 64;
    size_t bit_shift  = shift % 64;

    if (word_shift > 0) {
        for (size_t i = chunks; i-- > word_shift; )
            bt->data[i] = bt->data[i - word_shift];
        for (size_t i = 0; i < word_shift; i++)
            bt->data[i] = 0;
    }

    if (bit_shift > 0) {
        for (size_t i = chunks; i-- > 0; ) {
            uint64_t hi = bt->data[i] << bit_shift;
            uint64_t lo = 0;
            if (i > 0)
                lo = bt->data[i - 1] >> (64 - bit_shift);
            bt->data[i] = hi | lo;
        }
    }

    size_t extra = bt->size % 64;
    if (extra != 0) {
        uint64_t mask = ((uint64_t)1 << extra) - 1;
        bt->data[chunks - 1] &= mask;
    }

    normalize_bitset(bt);
}

// Shifts right >>
void shr_bitset(cbitset *bt, size_t shift) {
    if (shift == 0 || bt->size == 0) return;

    size_t chunks = (bt->size + 63) / 64;
    size_t word_shift = shift / 64;
    size_t bit_shift  = shift % 64;

    if (word_shift > 0) {
        for (size_t i = 0; i + word_shift < chunks; i++)
            bt->data[i] = bt->data[i + word_shift];
        for (size_t i = chunks - word_shift; i < chunks; i++)
            bt->data[i] = 0;
    }

    if (bit_shift > 0) {
        for (size_t i = 0; i < chunks; i++) {
            uint64_t lo = bt->data[i] >> bit_shift;
            uint64_t hi = 0;
            if (i + 1 < chunks)
                hi = bt->data[i + 1] << (64 - bit_shift);
            bt->data[i] = lo | hi;
        }
    }

    size_t extra = bt->size % 64;
    if (extra != 0) {
        bt->data[chunks - 1] &= ((uint64_t)1 << extra) - 1;
    } 

    normalize_bitset(bt);
}

// Returns 1 if any bit is set
int bitset_any(const cbitset *bt) {
    size_t chunks = (bt->size + 63) / 64;
    for (size_t i = 0; i < chunks; i++) {
        if (bt->data[i] != 0) return 1;
    }
    return 0;
}

// Returns 1 if no bits are set
int bitset_none(const cbitset *bt) {
    return !bitset_any(bt);
}

// Returns 1 if all bits are set
int bitset_all(const cbitset *bt) {
    size_t full_chunks = bt->size / 64;
    size_t extra_bits = bt->size % 64;

    for (size_t i = 0; i < full_chunks; i++) {
        if (bt->data[i] != UINT64_MAX) return 0;
    }

    if (extra_bits) {
        uint64_t mask = ((uint64_t)1 << extra_bits) - 1;
        if ((bt->data[full_chunks] & mask) != mask) return 0;
    }

    return 1;
}

// Returns index of first set bit, -1 if none
int bitset_find_first(const cbitset *bt) {
    assert(bt->size <= INT_MAX && "Bitset too large for int return type");
    int chunks = (bt->size + 63) / 64;
    for (int i = 0; i < chunks; i++) {
        if (bt->data[i] != 0) {
            int bit = __builtin_ctzll(bt->data[i]);
            int idx = i * 64 + bit;
            return idx < (int)bt->size ? idx : -1;
        } 
    }
    return -1;
}

// Returns next set bit after prev_index, -1 if none
int bitset_find_next(const cbitset *bt, size_t prev_index) {
    assert(bt->size <= INT_MAX && "Bitset too large for int return type");
    if (prev_index >= bt->size - 1) return -1;
    int next = prev_index + 1;
    int chunk = next / 64;
    int bit = next % 64;
    int chunks = (bt->size + 63) / 64;

    uint64_t x = bt->data[chunk] & (~0ULL << bit);
    if (x != 0) {
        int local_bit = __builtin_ctzll(x);
        int idx = chunk * 64 + local_bit;
        return (idx < (int)bt->size) ? idx : -1;
    } 

    for (int i = chunk + 1; i < chunks; i++) {
        if (bt->data[i] != 0) {
            int local_bit = __builtin_ctzll(bt->data[i]);
            int idx = i * 64 + local_bit;
            return (idx < (int)bt->size) ? idx : -1;
        } 
    }

    return -1;
}

