# cbitset — High-Performance Dynamic Bitset for C

A fast, clean, zero-dependency, pure C99 dynamic bitset with a complete feature set.

- Arbitrary size
- Constant-time `popcount`, `find_first`, `find_next`
- Safe logical operations (`&`, `|`, `^`, `~`)
- In-place left/right shifts
- Canonical representation (unused bits always zero → `memcmp` safe)
- String conversion (MSB-first, like `0b...`)
- Uses GCC/Clang builtins (`__builtin_popcountll`, `__builtin_ctzll`) for maximum speed

Perfect for:
- Compilers / interpreters
- Cryptography
- Sparse sets
- Bit-level algorithms
- Embedded systems (when you need speed)

## Why this one?

| Feature                    | cbitset | std::bitset | boost::dynamic_bitset | Most GitHub impls |
|---------------------------|---------|-------------|------------------------|-------------------|
| Dynamic size              | Yes     | No          | Yes                    | Sometimes         |
| Zero dependencies         | Yes     | C++         | Boost                  | Sometimes         |
| Pure C99                  | Yes     | No          | No                     | Rare              |
| Unused bits normalized   | Yes     | Yes         | Yes                    | Often buggy       |
| `find_next` + `ctzll`     | Yes     | No          | Yes                    | Rare              |
| Clean API + full ops      | Yes     | Yes         | Yes                    | Incomplete        |
