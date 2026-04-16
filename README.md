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

## Continuous Integration

This project uses GitHub Actions to validate builds on Linux (GCC) and Windows (MSVC) for every push and pull request.

**Supported platforms**: Ubuntu (GCC/Clang), Windows (MSVC)

**What CI checks**:
- CMake configuration with portable compiler flags
- Full build on both Linux and Windows
- Smoke test (executable runs successfully)

**Local reproduction**: Use the same CMake commands documented in the [Build](#build) section to verify locally before pushing.

## Dev Release Pipeline

When you push to the `dev` branch, GitHub Actions automatically:
1. Builds the project for Ubuntu (Linux) and Windows
2. Runs smoke tests to verify the binaries work
3. Publishes both binaries to a GitHub release tagged as `dev`

You can download the latest development binaries from the [Releases](../../releases/tag/dev) page under the `dev` tag.

**Binaries available**:
- `peruser-linux-x64` — Linux executable
- `peruser-windows-x64.exe` — Windows executable

The dev release is marked as a pre-release and is updated on every push to dev, so you always have the latest development build available.

## Stable Releases

Stable releases are created by pushing a version tag (e.g., `v1.0.0`, `v1.1.0`) to the repository.

**To create a release**:
```bash
git tag v1.0.0
git push origin v1.0.0
```

GitHub Actions automatically:
1. Builds binaries for Ubuntu (Linux) and Windows
2. Runs smoke tests on both platforms
3. Creates a GitHub release with the binaries attached

**Stable release binaries** are available on the [Releases](../../releases) page and are marked as stable (not pre-release).

## Validation Checklist

After parser or build-related changes:
- Build successfully with CMake.
- Run the peruser executable.
- Confirm output behavior for the changed regex scenario.

## License

See LICENSE.
