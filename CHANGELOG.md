# Changelog

## 1.0.0

Ce establishes its first complete extended-C core.

Added explicit ownership and borrowed views; checked size arithmetic; byte buffers; strings; vectors; maps; sets; queues; deques; integer parsing; formatting; UTF-8 validation and decoding; hashing; sorting and searching; path operations; binary file operations; streams; directory traversal; environment variables; time; deterministic random state; command-line access; terminal detection; platform identification; process execution; diagnostics; logging; a coherent umbrella header; pkg-config metadata; and first-class Nix packaging.

The release enforces strict C11 compilation, formatting, tests, sanitizers, and static analysis through the repository build system and CI configuration.
