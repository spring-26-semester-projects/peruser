# Peruser

<!-- badges go here

[![Build]()]()

[![Unit Tests]()]()

[![Stable Release]()]()

-->

peruser is a text-search command-line tool that uses regular expression matches to find patterns in text.

It uses ![Thompson’s construction algorithm](https://en.wikipedia.org/wiki/Thompson%27s_construction) for converting regular expressions into NFAs from the backend, and outputs object code corresponding to the final NFA. The front-end executes the automata, and searches a stream of text for pattern matches, and returns with all matches (or none if there are not any).

## Build

Run the build script `scripts/build.sh` to make a successful build.

Dependencies:

```
cmake
g++
python
```

## The tool

<!-- TODO: Describe how to call tool -->
