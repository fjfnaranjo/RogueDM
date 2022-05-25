# RogueDM

A simple networked RPG virtual tabletop.

## Building

RogueDM uses CMake's. You can build the project the usual way...

    cmake -B build
    cmake --build build

Building RogueDM requires CMake, the SDL2 library (with the `image`, `ttf` and
`net` projects) and the Lua 5.3 library.

## Building the docs

To generate the code documentation, issue the following command in the project
root.

    doxygen docs/doxyfile

Building the docs requires Doxygen and Graphviz.

## License

GPL-3.0 (or any later version)

Non-code resources also convered by this license unless specified.
