# Ce

Ce is a small, dependency-free C utility library built around explicit ownership, predictable failure, simple data structures, and boring interfaces.

The name comes from **C** and **Censera**.

## Status

Ce is being built as a complete C11 utility layer. The public API is intentionally small. The library does not try to replace the C standard library or hide the language.

## Design

- C11.
- No external dependencies.
- No hidden allocation.
- Explicit ownership and cleanup.
- Explicit failure through `CeResult`.
- No global mutable library state.
- No exceptions, threads, callbacks, or framework machinery in the core.
- Portable standard-C implementation where the platform permits it.
- Performance comes from simple representations and predictable control flow.

## Modules

- `ce/core.h` - result codes, sizes, and common status handling.
- `ce/mem.h` - checked size arithmetic and memory operations.
- `ce/buf.h` - owned growable byte buffer.
- `ce/str.h` - owned growable NUL-terminated string.
- `ce/file.h` - complete binary/text file loading and writing.

The umbrella header `ce/ce.h` includes the public API.

## Build

```sh
make
make test
make install PREFIX=/usr/local
```

The library is built with warnings enabled and no external runtime dependency beyond the C standard library.

## Example

```c
#include <stdio.h>
#include <ce/ce.h>

int main(void)
{
    CeStr name;
    CeResult result = ce_str_init(&name, "Censera");

    if (result != CE_OK) {
        fprintf(stderr, "ce_str_init: %s\n", ce_result_name(result));
        return 1;
    }

    result = ce_str_append(&name, "'s Ce library");
    if (result != CE_OK) {
        fprintf(stderr, "ce_str_append: %s\n", ce_result_name(result));
        ce_str_free(&name);
        return 1;
    }

    puts(ce_str_data(&name));
    ce_str_free(&name);
    return 0;
}
```

## Engineering standard

Ce follows the project's C engineering rules: ownership is explicit, allocations are deliberate, interfaces are small, undefined behavior is treated as a bug, and the product itself is held to the same cleanliness standard as the source.

## License

MIT. See `LICENSE`.
