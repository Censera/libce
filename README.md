# Ce

Ce is an extended C environment built around explicit ownership, predictable failure, simple data structures, direct control flow, and one coherent vocabulary.

The name comes from **C** and **Censera**.

## Status

Ce 1.0.0 is the first complete release of the extended-C core. C remains the language and machine model. Ce supplies the facilities that belong around that model without introducing a framework, hidden runtime, exceptions, or implicit ownership.

## Design

- C11 remains the language and ABI foundation.
- Public symbols use the `ce_` namespace.
- Public types use the `Ce` prefix.
- Owned and borrowed data are explicitly distinguished.
- Fallible operations return `CeResult`.
- Length and capacity remain separate concepts.
- Allocation and cleanup are deterministic.
- No global mutable library state is required.
- Simple structs, ordinary functions, and visible control flow are preferred.
- Platform-specific code stays behind platform-specific implementation boundaries.
- Every module follows the same engineering rules.

## Modules

Core data and memory:

- `ce/core.h`
- `ce/mem.h`
- `ce/view.h`
- `ce/buf.h`
- `ce/str.h`
- `ce/vec.h`
- `ce/map.h`
- `ce/set.h`
- `ce/queue.h`

Text and binary data:

- `ce/parse.h`
- `ce/format.h`
- `ce/utf8.h`
- `ce/hash.h`
- `ce/sort.h`

Filesystem and I/O:

- `ce/path.h`
- `ce/file.h`
- `ce/stream.h`
- `ce/dir.h`

System and application support:

- `ce/env.h`
- `ce/time.h`
- `ce/random.h`
- `ce/platform.h`
- `ce/process.h`
- `ce/cli.h`
- `ce/log.h`
- `ce/diag.h`

`ce/ce.h` is the umbrella header. Every module remains independently includable.

## Build

```sh
make
make test
make check
```

Install under `/usr/local`:

```sh
make install
```

Install elsewhere:

```sh
make install PREFIX=/usr
```

Nix users can build the package directly:

```sh
nix build .#libce
```

The build uses strict warnings as errors. CI enforces formatting, tests, sanitizers, and static analysis when GitHub Actions is available.

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

    result = ce_str_append_format(&text, "'s Ce library: %d", 1);
    if (result != CE_OK) {
        fprintf(stderr, "ce_str_append_format: %s\n", ce_result_name(result));
        ce_str_free(&text);
        return 1;
    }

    puts(ce_str_data(&text));
    ce_str_free(&text);
    return 0;
}
```

## Engineering standard

Ce follows `C_rules.md`: ownership, lifetime, bounds, allocation, failure, interfaces, naming, build structure, documentation, and product behavior are treated as engineering constraints rather than decoration.

## License

Apache License 2.0. See `LICENSE`.
