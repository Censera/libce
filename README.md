# Ce

Ce is an extended C environment built around explicit ownership, predictable failure, simple data structures, direct control flow, and one coherent vocabulary.

The name comes from **C** and **Censera** :p

## Status

Ce targets C11. Version 0.9.0 is the architectural foundation for a larger extended-C environment. The goal is not feature count. The goal is to provide the facilities C programmers repeatedly need without turning C into a framework or hiding the machine model.

## Design

- C remains the language and machine model.
- Public symbols use the `ce_` namespace.
- Public types use the `Ce` prefix.
- Owned and borrowed data are explicitly distinguished.
- Fallible operations return `CeResult`.
- Length and capacity remain separate concepts.
- Allocation and cleanup are deterministic.
- No global mutable library state is required.
- No hidden runtime, exceptions, framework machinery, or automatic initialization.
- Simple structs, ordinary functions, and visible control flow are preferred.
- Platform-specific details stay behind platform-specific implementation boundaries.
- Every module follows the same engineering rules.

## Modules

Foundation:

- `ce/core.h` - common result codes and result names.
- `ce/mem.h` - checked `size_t` arithmetic and memory allocation helpers.
- `ce/view.h` - borrowed byte ranges.
- `ce/buf.h` - owned growable byte buffers.
- `ce/str.h` - owned growable NUL-terminated strings.
- `ce/vec.h` - owned growable storage for fixed-size elements.
- `ce/file.h` - binary file reading, writing, appending, and ownership cleanup.

Planned higher layers include containers, text, I/O, filesystem, system facilities, hashing, randomness, sorting, command-line handling, logging, and diagnostics. A facility is added only when its representation and contract are understood well enough to follow the same rules as the foundation.

`ce/ce.h` is the umbrella header for the normal Ce toolbox. Individual headers remain independently includable.

See `docs/architecture.md` for the architectural model.

## Build

```sh
make
make test
make check
```

Install system-wide under `/usr/local`:

```sh
make install
```

Install elsewhere:

```sh
make install PREFIX=/usr
```

The build enables strict compiler warnings and treats them as errors. CI also runs formatting, tests, sanitizers, and static analysis.

## Usage

```c
#include <stdio.h>
#include <ce/ce.h>

int main(void)
{
    CeStr text;
    CeResult result = ce_str_init(&text, "Censera");

    if (result != CE_OK) {
        fprintf(stderr, "ce_str_init: %s\n", ce_result_name(result));
        return 1;
    }

    result = ce_str_append(&text, "'s Ce library");
    if (result != CE_OK) {
        fprintf(stderr, "ce_str_append: %s\n", ce_result_name(result));
        ce_str_free(&text);
        return 1;
    }

    puts(ce_str_data(&text));
    ce_str_free(&text);
    return 0;
}
```

## Engineering standard

Ce follows the project's C engineering rules. Ownership, lifetime, bounds, allocation, failure, interfaces, naming, and product behavior are treated as engineering concerns rather than cosmetic preferences.

## License

Apache License 2.0. See `LICENSE`.
