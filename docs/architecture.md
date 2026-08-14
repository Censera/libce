# Ce 1.0.0 Architecture

Ce is an extended C environment. C remains the language and machine model. Ce supplies a coherent standard-like environment around it without hiding ownership, allocation, control flow, or failure.

## Rules

Every public facility follows the same model:

- Public symbols use the `ce_` namespace.
- Public types use the `Ce` prefix.
- Owned data has an explicit release operation.
- Borrowed data uses an explicit view type and does not own storage.
- Fallible operations return `CeResult`.
- Length and capacity are distinct concepts.
- Pointer and length travel together for byte ranges.
- Allocation is visible in the contract and has deterministic cleanup.
- Platform-specific implementation stays behind platform-specific boundaries.
- Global mutable library state is not required.
- No hidden runtime, exceptions, framework machinery, or implicit initialization exists.
- Simple structs and ordinary functions are preferred over elaborate object systems.

## Public layers

```text
core
    core mem view

data
    buf str vec map set queue

text
    parse format utf8 hash sort

filesystem
    path file stream dir

system
    env time random platform process

application
    cli log diag
```

The umbrella header `ce/ce.h` exposes these layers together. Individual headers remain independently includable.

## Ownership

`CeView` is borrowed memory. `CeBuf`, `CeStr`, `CeVec`, `CeMap`, `CeSet`, `CeQueue`, and `CeFile` own their internal storage and provide explicit cleanup.

Public functions must not retain borrowed memory beyond the operation unless the contract explicitly says so. Operations that can reallocate invalidate borrowed pointers into the affected storage.

## Failure

A failed operation is observable through its return value. `CE_OK` represents success. Other results identify invalid arguments, allocation failure, overflow, I/O failure, missing objects, existing objects, or invalid data.

Diagnostics are separate from failure itself. `ce_diag_print()` reports an existing result; it does not replace error handling.

## Allocation

Ce uses the ordinary C allocator. No global allocator is hidden behind the public API. `realloc`-style operations preserve the old pointer until allocation succeeds, and size arithmetic is checked before allocation sizes are computed.

## Containers

Containers store fixed-size values. Ce does not impose element destructors, ownership callbacks, iterator frameworks, or runtime type information. When a caller stores pointers, the caller owns the pointed-to objects unless an API contract explicitly states otherwise.

`CeMap` uses caller-supplied hash and equality functions because arbitrary key types require a type-specific definition of identity. Storage remains contiguous and explicitly owned.

## Text

`CeStr` represents owned NUL-terminated bytes. `CeView` represents borrowed counted bytes. UTF-8 is handled explicitly through `ce/utf8.h`; ordinary byte strings are not silently treated as Unicode text.

## System boundaries

Filesystem, process, environment, time, random, and platform APIs wrap the platform where necessary but preserve the same public naming and failure conventions. Platform-specific behavior stays inside implementation branches instead of spreading through public interfaces.

`CeRandom` owns its deterministic generator state. No implicit global random source is created.

## Product boundary

The repository, installed files, examples, documentation, diagnostics, and API use the same terminology. Build products are generated under `build/`; installation places headers under `include/ce` and the static library under `lib`.

The product should feel like one C environment, not a bag of unrelated utilities.
