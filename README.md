# peruser

A regular-expression preprocessing.

At a high level, peruser does two things:
- Compile-time validation of allowed regex symbols through a string literal operator.
- Runtime transformation of regex text from infix form into a reverse-polish-like representation.

The codebase is intentionally compact, making it useful for parser experimentation, learning, and incremental improvements.

## Features

- C++20 literal-based regex input via operator"" _re
- Compile-time symbol validation for a restricted regex alphabet
- In-place runtime conversion in Regex::torpol()
- Lightweight structure that is easy to read and modify

## Requirements

- CMake 3.15 or newer
- C++20-capable compiler

## Build

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

## Run

- Linux/macOS (single-config):

```bash
./build/peruser
```

- Windows (single-config generators):

```powershell
.\build\peruser.exe
```

## Current Behavior And Limits

- Regex::torpol() assumes valid regex syntax.
- Invalid syntax can lead to undefined behavior.
- Parentheses are currently unsupported.
- The build defines __DEBUG_BUILD, so debug-only print helpers are compiled.

In short: this project currently favors a simple, fast path for valid inputs over defensive handling of invalid expressions.

## Example

Current smoke example in src/peruser.C:
- input: a|b
- output: prints the internal transformed representation

To try another regex, update src/peruser.C and rebuild.

## Development Notes

- Source files use .C extension in this repository.
- Keep parser-related declaration/implementation changes synchronized between include/rpol.h and src/rpol.C.
- Prefer portable CMake commands over machine-specific scripts.

If you are making parser changes, keep edits small and validate behavior with a quick run after each meaningful change.

## Validation Checklist

After parser or build-related changes:
- Build successfully with CMake.
- Run the peruser executable.
- Confirm output behavior for the changed regex scenario.

## License

See LICENSE.
