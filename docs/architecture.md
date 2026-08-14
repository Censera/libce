# Ce 1.0.0 Architecture

Ce is an extended C environment. C remains the language and machine model. Ce supplies coherent facilities around that model without hiding ownership, allocation, control flow, or failure.

## Rules

Every public facility follows the same model:

- Public symbols use the `ce_` namespace.
- Public types use the `Ce` prefix.
- Owned data exposes an explicit release operation.
- Borrowed data has a distinct view type and does not own storage.
- Fallible operations return `CeResult`.
- Length and capacity are separate values.
- Pointer and length travel together for byte ranges.
- Allocation is visible in the contract and has a deterministic cleanup path.
- Platform-specific code stays behind localized platform boundaries.
- Global mutable library state is not required.
- There is no hidden runtime, exception system, framework machinery, or automatic initialization.
- Higher-level facilities reuse lower-level ownership and error conventions instead of inventing competing models.

## Layers

```text
core
    result, checked arithmetic, allocation, borrowed views

containers
    buffer, string, vector, map, set, queue

text
    parsing, formatting, hashing, sorting

io
    files, streams, directories, paths

system
    environment, time, deterministic random state

application
    command-line handling, logging
```

The layers describe dependency direction, not class hierarchy. A module may depend on lower layers but never needs a framework object or global context to reach them.

## Ownership

`CeView` is borrowed. `CeBuf`, `CeStr`, `CeVec`, `CeMap`, `CeSet`, `CeQueue`, and `CeFile` own their internal storage. Their `*_free` operation releases that storage and leaves the object empty.

Pointers returned from an owning object are borrowed and remain valid only until an operation that can reallocate or free that object's storage. APIs document aliasing restrictions when an operation may invalidate a borrowed pointer.

## Failure

Fallible operations return `CeResult`. They do not print, terminate the process, mutate global error state, or encode failure in ordinary data values.

`CE_OK` represents success. Other result values distinguish invalid input, allocation failure, arithmetic overflow, I/O failure, missing objects, existing objects, and invalid data.

## Containers

Containers store bytes and fixed-size elements. They do not own arbitrary element graphs and therefore do not require hidden destructors, callbacks, or type registries.

`CeMap` and `CeSet` use caller-provided hash and equality functions because arbitrary key types require a runtime comparison rule. The storage remains contiguous and the dispatch surface is limited to those operations.

## System boundary

Filesystem, environment, time, and directory operations isolate platform differences in implementation files. The public API remains expressed in C types and Ce contracts.

The library does not promise to abstract every operating-system facility. A platform feature enters the public API only when its ownership, failure, and portability contract can be stated clearly.

## Product boundary

`ce/ce.h` exposes the complete 1.0.0 toolbox. Individual headers remain independently includable.

The installation layout is:

```text
include/ce/*.h
lib/libce.a
lib/pkgconfig/libce.pc
```

Nix packaging is first-class through `flake.nix` and `nix/package.nix`.

## Engineering gate

A module is not complete merely because it compiles. Its representation, ownership model, failure behavior, tests, documentation, formatting, sanitizer behavior, and static-analysis behavior must satisfy the same engineering rules as the rest of Ce.
