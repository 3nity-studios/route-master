# Contributing

## Repository Structure

```
route-master
│
├── assets
│
├── cmake
│
├── source
│   │
│   ├── config
│   │   ├── include
│   │   └── src
│   │
│   ├── engine
│   │   ├── include
│   │   └── src
│   │
│   ├── platform
│   │   ├── include
│   │   └── src
│   │
│   ├── states
│   │   ├── include
│   │   └── src
│   │
│   └── utilities
│       ├── include
│       └── src
│   
└── tests
    ├── include
    └── src
```

* assets        — Images, sprites, sounds, music, fonts, multimedia content
* cmake         — CMake related files, macros, functions, variables
* docs          — Documentation pages to be built by Doxygen
* source        — Where the real deal is
    * config    — Core config of the codebase
    * engine    — Low level component of the game logic
    * states    — User interface and front-end screens using state stack
    * platform  — Platform specific code needed by the engine
    * utilities — Miscellaneous utilities needed across the project
* tests         — Testing suite

## Code Styling

Most of our style is based on `clangd-format`'s Microsoft preset with some
tweaks here and there defined in the `.clang-format` file at the root of the
repository.

Other than that, semantic conventions are established in the guide provided by
Universitat de València available at http://informatica.uv.es/iiguia/AED/laboratorio/Estilocpp.pdf

Source code files should end with a blank new line, UTF-8 and LF for the sake of
consistency.

## Branching Strategy

We sort of roll with GitHub flow, trying to keep branches as short as possible.

## Commits convention

Make sure to properly write your commit messages following Conventional Commits
using Angular's specification. This is done in order to automatically generate
a [CHANGELOG.md](CHANGELOG.md) in the near future when we finish setting up our
GitHub workflows.

## Room for improvement

Plans on expanding this document are underway, things like specifying a public
roadmap, development tracking, setting up a full blown documentation hosted on
GitHub Pages, adding relevant links and other stuff.

We were thinking of adding a PCH to the build system but it's still being
worked on.
