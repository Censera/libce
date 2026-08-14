# Ce 0.9.0 Architecture

Ce 0.9.0 defines Ce as an extended C environment: C remains the language and machine model, while Ce supplies a coherent set of missing facilities without hiding ownership, allocation, control flow, or failure.

## Rules

Every public facility follows the same model:

- Public symbols use the `ce_` namespace.
- Public types use the `Ce` prefix.
- Owned data exposes an explicit release operation.
- Borrowed data has a distinct view type and does not own storage.
- Fallible operations return `CeResult`.
- Length and capacity are separate values.
- Pointer and length are passed together for byte ranges.
- Allocation is visible in the contract and has a deterministic cleanup path.
- Platform-specific code stays behind platform-specific implementation boundaries.
- No global mutable library state is required.
- No framework, hidden runtime, exception system, or implicit initialization is part of Ce.

## Foundation

The foundation is deliberately small:

- `core.h`: shared result type and result names.
- `mem.h`: checked size arithmetic and allocation primitives.
- `view.h`: borrowed byte ranges.
- `buf.h`: owned growable byte storage.
- `str.h`: owned growable NUL-terminated text.
- `vec.h`: owned growable storage for fixed-size elements.
- `file.h`: owned file contents and binary file operations.

Higher-level facilities must depend on these lower-level concepts instead of inventing competing ownership and error conventions.

## Module progression

The 0.9.0 architecture is organized into layers rather than a feature checklist:

```text
foundation
    core mem view buf str vec file

containers
    maps sets queues and other structures only when a real use exists

text
    views formatting parsing encoding

io
    streams buffered I/O directories paths

system
    environment time process terminal platform

utilities
    hashing randomness sorting searching CLI diagnostics logging
```

A module enters Ce only when its representation, ownership model, failure behavior, and interface are understood well enough to satisfy the same engineering rules as the foundation.

## Product boundary

The umbrella header, `ce/ce.h`, exposes the normal Ce toolbox. Individual headers remain independently includable so consumers do not need the entire library for one facility.

The product must retain the same vocabulary as the source: APIs, documentation, installation layout, diagnostics, and examples must describe the same concepts using the same names.
