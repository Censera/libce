# Ce

Ce is a small, dependency-free C utility library built around explicit ownership, predictable failure, simple data structures, and boring interfaces.

The name comes from **C** and **Censera** :p

## Status

Ce targets C11 and provides a compact utility layer for programs that want useful primitives without bringing in a framework or hiding C behind abstractions.

The public API is namespaced with `ce_` because C has no namespaces. Public types use the `Ce` prefix. Internal implementation symbols remain private to their translation units.

## Design

- C11.
- No external dependencies.
- Explicit ownership and cleanup.
- Fallible operations return `CeResult`.
- Checked size arithmetic for allocation and buffer growth.
- No global mutable library state.
- No exceptions, threads, callbacks, or framework machinery in the core.
- Simple representations and predictable control flow.
- Standard C interfaces where the platform permits them.

## Modules

- `ce/core.h` - common result codes and result names.
- `ce/mem.h` - checked `size_t` arithmetic and memory allocation helpers.
- `ce/buf.h` - owned growable byte buffers.
- `ce/str.h` - owned growable NUL-terminated strings.
- `ce/file.h` - binary file reading, writing, appending, and ownership cleanup.

`ce/ce.h` is the umbrella header for the complete public API.

## Build

```sh
make
make test
```

Install system-wide under `/usr/local`:

```sh
make install
```

Install elsewhere:

```sh
make install PREFIX=/usr
```

The build enables strict compiler warnings and treats them as errors.

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
