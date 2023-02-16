# Load Store Queue header
The LSQ module is located in `LoadStoreQueue.hpp`.
The rest of the files are adapted sycl utility headers.


### Utilities

| Filename                       | Description
---                              |---
| `constexpr_math.hpp`           | Defines utilities for statically computing math functions (for example, Log2 and Pow2).
| `memory_utils.hpp`             | Generic functions for streaming data from memory to a SYCL pipe and vise versa.
| `metaprogramming_utils.hpp`    | Defines various metaprogramming utilities (for example, generating a power of 2 sequence and checking if a type has a subscript operator).
| `pipe_utils.hpp`               | Utility classes for working with pipes, such as PipeArray.
| `tuple.hpp`                    | Defines a template to implement tuples.
| `unrolled_loop.hpp`            | Defines a templated implementation of unrolled loops.
| `onchip_memory_with_cache.hpp` | Class that contains an on-chip memory array with a register backed cache to achieve high performance read-modify-write loops.


### License

Code samples are licensed under the MIT license. See [License.txt](https://github.com/oneapi-src/oneAPI-samples/blob/master/License.txt) for details.
